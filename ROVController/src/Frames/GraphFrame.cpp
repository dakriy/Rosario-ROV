#include "GraphFrame.h"
#include "../Core/GlobalContext.h"
#include "../Utilities.h"
#include <limits>

Frames::GraphFrame::GraphFrame() : linesPerScreenTarget(13, 7)
{
	windowSize = GlobalContext::get_window()->getSize();

	// Start out centered at the origin
	// Set to same scale, so that the x axis goes from -10 to 10 which is 20
	graphBounds.left = -10;
	graphBounds.width = 20;
	// X scale same as Y scale
	graphBounds.height = windowSize.y * getScaleX();
	graphBounds.top = graphBounds.height / 2.;

	setHooks();
}

void Frames::GraphFrame::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	// Got to be multiple of 1,2, or 5
	// Zoom in scale down
	// Zoom out scale up

	// Draw vertical lines
	// Calculate scale factor for each tick on the graph
	auto scaleX = graphBounds.width / linesPerScreenTarget.x;
	auto inv = false;
	if (scaleX < 1.)
	{
		scaleX = 1. / scaleX;
		inv = true;
	}

	auto exponent = GetNumberOfDigits(scaleX) - 1;

	// Initially set to some large number so it will get overwritten
	auto scaleXreal = std::numeric_limits<double>::max();

	for (auto factor : graphIntervals)
	{
		auto fac = pow(10, exponent) * factor;
		if (error(fac, scaleX) < error(scaleXreal, scaleX))
		{
			scaleXreal = fac;
		}		
	}

	if (inv)
	{
		// Invert it back again if we need to.
		scaleXreal = 1. / scaleXreal;
	}

	sf::Vertex line[2];
	for (auto x = RoundToNearest(graphBounds.left, scaleXreal); x < graphBounds.left + graphBounds.width; x += scaleXreal)
	{
		line[0] = sf::Vertex(sf::Vector2f(x / getScaleX(), windowSize.y));
		line[1] = sf::Vertex(sf::Vector2f(x / getScaleX(), 0));
		line[0].color = line[1].color = sf::Color(128, 128, 128);
		target.draw(line, 2, sf::Lines);
	}
}

void Frames::GraphFrame::update(const sf::Time& dt)
{
}

void Frames::GraphFrame::zoomRelative(int x, int y, float amount)
{
	assert(x >= 0 && x <= windowSize.x && "X is out of window range");
	assert(y >= 0 && y <= windowSize.y && "Y is out of window range");
	double percentLeftZoom = x / windowSize.x;
	double percentTopZoom = y / windowSize.y;

	auto totalX = amount * graphBounds.width * zoomTickAmount;
	auto totalY = amount * graphBounds.height * zoomTickAmount;

	// Calcualte number of percents of screensize to grow or shink by
	graphBounds.width -= totalX;
	graphBounds.height -= totalY;
	graphBounds.left -= percentLeftZoom * totalX;
	graphBounds.top -= percentTopZoom * totalY;
}



void Frames::GraphFrame::setZoom(sf::Vector2<double> xRange, sf::Vector2<double> yRange)
{
	assert(xRange.x < xRange.y && "X lower bound is larger than upper bound");
	assert(yRange.x < yRange.y && "Y lower bound is larger than upper bound");
	graphBounds.top = yRange.y;
	graphBounds.left = xRange.x;
	graphBounds.height = yRange.y - yRange.x;
	graphBounds.width = xRange.y - xRange.x;
}

double Frames::GraphFrame::getScaleX() const
{
	return graphBounds.width / windowSize.x;
}

double Frames::GraphFrame::getScaleY() const
{
	return graphBounds.height / windowSize.y;
}

bool Frames::GraphFrame::xAxisVisible() const
{
	return graphBounds.top > 0 && graphBounds.top < graphBounds.height;
}

bool Frames::GraphFrame::yAxisVisible() const
{
	return graphBounds.left > 0 && graphBounds.left < graphBounds.width;
}

Frames::FrameType Frames::GraphFrame::get_type() const
{
	return Graph;
}

Frames::GraphFrame::~GraphFrame()
{
	GlobalContext::get_event_handler()->unhook_event_callback(resizeHook, sf::Event::Resized);
	GlobalContext::get_event_handler()->unhook_event_callback(scrollHook, sf::Event::MouseWheelScrolled);
}

void Frames::GraphFrame::setHooks()
{
	resizeHook = GlobalContext::get_event_handler()->add_event_callback([&](const sf::Event *) {
		windowSize = GlobalContext::get_window()->getSize();
		return false;
	}, sf::Event::Resized);

	scrollHook = GlobalContext::get_event_handler()->add_event_callback([&](const sf::Event *e)
	{
		if (e->mouseWheelScroll.wheel == sf::Mouse::Wheel::VerticalWheel)
		{
			zoomRelative(e->mouseWheelScroll.x, e->mouseWheelScroll.y, e->mouseWheelScroll.delta);
			return true;
		}
		return false;
	}, sf::Event::MouseWheelScrolled);
}


