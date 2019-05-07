#include "GraphFrame.h"
#include "../Core/GlobalContext.h"
#include "../Utilities/Utilities.h"
#include "../Factories/TextFactory.h"
#include <limits>
#include <string>
#include <iomanip>
#include <sstream>
#include <imgui-SFML.h>
#include <imgui.h>

Frames::GraphFrame::GraphFrame() : linesPerScreenTarget(13, 8)
{
	windowSize = GlobalContext::get_window()->getSize();

	// Start out centered at the origin
	// Set to same scale, so that the x axis goes from -10 to 10 which is 20
	graphBounds.left = -10;
	graphBounds.width = 20;
	// X scale same as Y scale
	graphBounds.height = windowSize.y * getScaleX();
	graphBounds.top = graphBounds.height / 2.;

	grapher.updateBounds(graphBounds);

	setHooks();
}

void Frames::GraphFrame::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	drawGrid(target, states);
	grapher.draw(target, states);
}

void Frames::GraphFrame::update(const sf::Time& dt)
{
	ImGui::Begin("Access");
	if (ImGui::Button("Main Menu"))
	{
		GlobalContext::get_engine()->frame_action(Core::FrameAction::PopFrame);
	}
	ImGui::End();

    ImGui::Begin("Math Module");
    ImGui::InputTextWithHint("Input Expression", "Enter math relation here", expression, IM_ARRAYSIZE(expression));

    if (expr && !expr->isEquation())
    {
        ImGui::Text("= %f", expr->compute(0,0));
    }

    if (exception) {
        ImGui::Text("%s", exception);
    }

    if (ImGui::Button("Calculate")) {
        delete expr;
        expr = nullptr;
        try {
			exception = nullptr;
            expr = calc.parse(expression);
            grapher.setExpression(expr);
        } catch(const char * ex)
        {
            exception = ex;
			expr = nullptr;
        }
    }

    if (ImGui::Button("Reset Zoom")) {
        setZoom(sf::Vector2<double>(-10, 10), sf::Vector2<double>(-10, 10));
    }

    ImGui::End();
    grapher.update();
}

void Frames::GraphFrame::drawGrid(sf::RenderTarget& target, sf::RenderStates states) const
{
    // Draw lines
    sf::Vertex line[2];

    auto scaleX = calculateGridScale(graphBounds.width / linesPerScreenTarget.x);
    auto scaleY = calculateGridScale(graphBounds.height / linesPerScreenTarget.y);

    for (auto x = RoundToNearest(graphBounds.left, scaleX); x < graphBounds.left + graphBounds.width; x += scaleX)
    {
        auto screen = convertToScreenCoords(graphBounds, sf::Vector2<double>(x, 0.));
        line[0] = sf::Vertex(sf::Vector2f(screen.x, static_cast<float>(windowSize.y)));
        line[1] = sf::Vertex(sf::Vector2f(screen.x, 0));
        line[0].color = line[1].color = sf::Color(128, 128, 128);
        target.draw(line, 2, sf::Lines);

        // Draw line number
        std::ostringstream num;
        num << std::scientific << std::setprecision(1) << x;
        std::string strNum = num.str();
        auto text = Factory::TextFactory::create_displayable_string(strNum);
        // For now just place it on the bottom
        text.setPosition(screen.x, static_cast<float>(windowSize.y - text.getCharacterSize()));
        text.setFillColor(sf::Color::White);
        target.draw(text);
    }

    for (auto y = RoundToNearest(graphBounds.top, scaleY); y > graphBounds.top - graphBounds.height; y -= scaleY)
    {
        auto screen = convertToScreenCoords(graphBounds, sf::Vector2<double>(0., y));
        line[0] = sf::Vertex(sf::Vector2f(static_cast<float>(windowSize.x), screen.y));
        line[1] = sf::Vertex(sf::Vector2f(0, screen.y));
        line[0].color = line[1].color = sf::Color(128, 128, 128);
        target.draw(line, 2, sf::Lines);

        // Draw line number
        std::ostringstream num;
        num << std::scientific << std::setprecision(1) << y;
        std::string strNum = num.str();
        auto text = Factory::TextFactory::create_displayable_string(strNum);
        // For now just place it on the bottom
        text.setPosition(0, screen.y);
        text.setFillColor(sf::Color::White);
        target.draw(text);
    }
}

double Frames::GraphFrame::calculateGridScale(double targetScale) const
{
    // Got to be multiple of 1,2, or 5
    // Zoom in scale down
    // Zoom out scale up

    // Draw vertical lines
    // Calculate scale factor for each tick on the graph
    auto inv = false;
    if (targetScale < 1.)
    {
        targetScale = 1. / targetScale;
        inv = true;
    }

    auto exponent = GetNumberOfDigits(static_cast<unsigned long long>(targetScale)) - 1;

    // Initially set to some large number so it will get overwritten
    auto scaleReal = std::numeric_limits<double>::max();

    for (auto factor : graphIntervals)
    {
        auto fac = pow(10, exponent) * factor;
        if (error(fac, targetScale) < error(scaleReal, targetScale))
        {
            scaleReal = fac;
        }
    }

    if (inv)
    {
        // Invert it back again if we need to.
        scaleReal = 1. / scaleReal;
    }

    return scaleReal;
}


void Frames::GraphFrame::zoomRelative(int x, int y, float amount)
{
	assert(x >= 0 && static_cast<unsigned>(x) <= windowSize.x && "X is out of window range");
	assert(y >= 0 && static_cast<unsigned>(y) <= windowSize.y && "Y is out of window range");
	double percentLeftZoom = static_cast<double>(x - static_cast<int>(windowSize.x) / 2.f) / static_cast<double>(windowSize.x);
	double percentTopZoom = static_cast<double>(y - static_cast<int>(windowSize.y) / 2.f) / static_cast<double>(windowSize.y);

	auto totalX = amount * graphBounds.width * zoomTickAmount;
	auto totalY = amount * graphBounds.height * zoomTickAmount;

	// Calculate number of percents of screensize to grow or shrink by
	graphBounds.width -= totalX;
	graphBounds.height -= totalY;
	graphBounds.left -= percentLeftZoom * totalX;
	graphBounds.top -= percentTopZoom * totalY;
    grapher.updateBounds(graphBounds);
}



void Frames::GraphFrame::setZoom(sf::Vector2<double> xRange, sf::Vector2<double> yRange)
{
	assert(xRange.x < xRange.y && "X lower bound is larger than upper bound");
	assert(yRange.x < yRange.y && "Y lower bound is larger than upper bound");
	graphBounds.top = yRange.y;
	graphBounds.left = xRange.x;
	graphBounds.height = yRange.y - yRange.x;
	graphBounds.width = xRange.y - xRange.x;
    grapher.updateBounds(graphBounds);
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


