#pragma once
#include "MainMenuFrame.h"
#include <SFML/Graphics.hpp>
#include "../UI/Options.h"

namespace Frames
{
	class TitleFrame : public MainMenuFrame
	{
	protected:
		// Title frame options enum
        enum OptionsEnum {
			Connect,
			View,
			Exit
        };

		// Title frame options text
        const std::vector<const char*> option_text_ = {
			"Connect to a Device",
			"Access Connected Device",
			"Exit"
        };
		// Event hook for pressing escape to exit
		EVENT_FUNC_INDEX(sf::Event, sf::Event::EventType::Count) escape_to_exit_hook_ = nullptr;

		// Creates the options
		void create_options();

		// Title frame options
		Controls::Options options_;
	public:
		/**
		 * Title frame constructor
		 * 
		 * Sets up escape to exit hook and bootstraps options
		 */
		TitleFrame();

		/**
		 * Draws the frame
		 * 
		 * @param target the render target
		 * @param states the render states
		 */
		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

		/**
		 * Updates the frame
		 * 
		 * @param dt time elapsed sinse last update
		 */
		void update(const sf::Time& dt) override;

		/**
		 * Returns the frame type
		 * 
		 * @return TitleFrame
		 */
		FrameType get_type() const override;

		/**
		 * Unhooks escape to exit hook
		 */
		~TitleFrame() override;
	};
}
