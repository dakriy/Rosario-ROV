#pragma once
#include "IFrame.h"
#include "../Globals.h"
#include "../Core/Engine.h"

/**
 * @file ViewFrame.h
 * Access the device and use it.
 *
 * @author dakriy
 */

namespace Frames
{
	class ViewFrame : public IFrame
	{
	protected:
		sf::Image image;
		sf::Texture tex;
		sf::Sprite sprite;
		bool frame = false;

		EVENT_FUNC_INDEX(Core::Event, Core::Event::EventType::Count) frameHook = nullptr;
		EVENT_FUNC_INDEX(Core::Event, Core::Event::EventType::Count) pressureHook = nullptr;
		EVENT_FUNC_INDEX(Core::Event, Core::Event::EventType::Count) temperatureHook = nullptr;

		double pressure = 0;
		double temp = 0;

	public:
		/**
		 * View Frame Constructor
		 * Currently defaulted
		 */
		ViewFrame();

		/**
		 * Draws the main menu frame
		 *
		 * @param target the render target
		 * @param states the render states
		 */
		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

		/**
		 * Updates the frame
		 * 
		 * @param dt time elapsed since last update
		 */
		void update(const sf::Time& dt) override;

		/**
		 * Returns the frame type
		 *
		 * @return TitleFrame
		 */
		FrameType get_type() const override;

		/**
		 * Default constructor because we don't need to do anything special
		 */
		~ViewFrame();
	};
}
