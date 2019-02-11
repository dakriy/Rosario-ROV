#include "TitleFrame.h"
#include "../UI/Button.h"
#include "../Core/GlobalContext.h"
#include "ConnectFrame.h"
#include "ViewFrame.h"

Frames::TitleFrame::TitleFrame() : 
    options_(this, content_bounds_)
{
	escape_to_exit_hook_ = GlobalContext::get_event_handler()->add_event_callback([](const sf::Event* e) -> bool {
		if (e->key.code == sf::Keyboard::Key::Escape)
		{
			GlobalContext::get_window()->close();
			return true;
		}
		return false;
	}, sf::Event::KeyPressed);
	create_options();
}

void Frames::TitleFrame::create_options()
{
	options_.add_option(new Controls::Button(option_text_[Connect], [&]()
	{
		hide();
		pause();
		GlobalContext::get_engine()->frame_action(Core::PushFrame, new ConnectFrame);
	}));
	options_.add_option(new Controls::Button(option_text_[View], [&]()
	{
		hide();
		pause();
		GlobalContext::get_engine()->frame_action(Core::PushFrame, new ViewFrame);
	}));
	options_.add_option(new Controls::Button(option_text_[Exit], []() { GlobalContext::get_window()->close(); }));
}

void Frames::TitleFrame::draw(sf::RenderTarget& target, const sf::RenderStates states) const
{
	MainMenuFrame::draw(target, states);
	
	target.draw(options_);
}

void Frames::TitleFrame::update(const sf::Time& dt)
{
	if (!isHidden() && !isPaused()) options_.update(dt);
}

Frames::FrameType Frames::TitleFrame::get_type() const
{
	return MainMenu;
}

Frames::TitleFrame::~TitleFrame()
{
	GlobalContext::get_event_handler()->unhook_event_callback(escape_to_exit_hook_, sf::Event::KeyPressed);
}
