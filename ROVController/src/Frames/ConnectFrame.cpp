#include "ConnectFrame.h"
#include "../UI/Button.h"
#include "../Core/GlobalContext.h"

void Frames::ConnectFrame::create_options()
{
	options_.add_option(new Controls::Button("Back", []()
	{
		GlobalContext::get_engine()->frame_action(Core::PopFrame);
	}));
}

Frames::ConnectFrame::ConnectFrame() :
	MainMenuFrame("Connect to a Device"),
	options_(this, content_bounds_)
{
	GlobalContext::get_network()->search_for_devices();
	create_options();
}

void Frames::ConnectFrame::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	MainMenuFrame::draw(target, states);

	target.draw(options_);
}

void Frames::ConnectFrame::update(const sf::Time& dt)
{
	auto t = GlobalContext::get_network()->get_devices();
	options_.remove_options();

	for (auto& p : t)
	{
		options_.add_option(new Controls::Button(p.second, [&]()
		{
			
		}));
	}
	create_options();

	if (!isHidden() && !isPaused()) options_.update(dt);
}

Frames::FrameType Frames::ConnectFrame::get_type() const
{
	return Connect;
}

Frames::ConnectFrame::~ConnectFrame()
{
	GlobalContext::get_network()->stop_search_for_devices();
}
