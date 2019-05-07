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

		CORE_EVENT_FUNC_INDEX frameHook = nullptr;
		CORE_EVENT_FUNC_INDEX pressureHook = nullptr;
		CORE_EVENT_FUNC_INDEX temperatureHook = nullptr;

		double pressure = 0;
		double temp = 0;

		bool movingUp = false;
		bool movingDown = false;

		const unsigned updateFrequency = 10;
		unsigned updateCounter = 0;

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
		~ViewFrame() override;
	};
}
