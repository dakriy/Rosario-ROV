#pragma once
#include "IFrame.h"
#include "../Globals.h"
#include "../Core/EventHandler.h"

namespace Frames
{
	class MainMenuFrame : public IFrame
	{
	protected:
		// Main Menu Frame bounds
		sf::FloatRect content_bounds_;

		// Frame background
		sf::Sprite background_;

		// Title text
		sf::Text title_;
	public:
		/**
		 * Main Menu Frame Constructor
		 * Sets background image and title text
		 * 
		 * @param text Title text
		 */
		MainMenuFrame(const char * text = APP_NAME);

		/**
		 * The backgorund texture
		 */
		static sf::Texture background_tex;

		/**
		 * Updates the bounds of the child frame
		 */
		void update_bounds();

		/**
		 * Draws the main menu frame
		 * 
		 * @param target the render target
		 * @param states the render states
		 */
		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

		/**
		 * Default constructor because we don't need to do anything special
		 */
		~MainMenuFrame() = default;
	};
}
