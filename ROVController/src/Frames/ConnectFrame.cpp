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
	create_options();
}

void Frames::ConnectFrame::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	MainMenuFrame::draw(target, states);

	target.draw(options_);
}

void Frames::ConnectFrame::update(const sf::Time& dt)
{
	if (!isHidden() && !isPaused()) options_.update(dt);
}

Frames::FrameType Frames::ConnectFrame::get_type() const
{
	return Connect;
}
