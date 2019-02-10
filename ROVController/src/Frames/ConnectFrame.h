#pragma once

/**
 * @file ConnectFrame.h
 * View the available list of devices and connect to said device
 * 
 * @author dakriy
 */

#include "MainMenuFrame.h"
#include "../UI/Options.h"

namespace Frames
{
	class ConnectFrame : public MainMenuFrame
	{
	protected:
		// Creates the options
		void create_options();

		// Title frame options
		Controls::Options options_;

	public:
		/**
		 * Connect frame constructor
		 * 
		 * Sets up the options and network watcher
		 */
		ConnectFrame();

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
	};
}
