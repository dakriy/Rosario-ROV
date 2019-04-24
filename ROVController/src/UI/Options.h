#pragma once

/**
 * @file Options.h
 * Class definition for an options box
 * 
 * @author dakriy
 * @todo Support variable length UIElements
 */

#include "../Interfaces/IUpdateable.h"
#include <SFML/Graphics.hpp>
#include "UIElement.h"
#include "../Core/EventHandler.h"
#include "../Frames/IFrame.h"
#include <vector>
#include <memory>

namespace Controls
{
	class Options : public sf::Drawable, public Interfaces::IUpdateable
	{
	protected:
		// The UI Elements/options list
		std::vector<std::unique_ptr<UIElement>> options_;

		// Bounds of the options element
		sf::FloatRect bounds_;

		// Mouse is over an option
		bool mouse_on_option_ = false;

		/**
		 * Resizes and positions only the visible options to where they are supposed to be
		 * Should be called on any resize, or scroll event.
		 * Probably not *extermely* fast
		 */
		void position_options();

		// mouse hook pointer so it can be unhooked when the object is destroyed
		EVENT_FUNC_INDEX(sf::Event, sf::Event::EventType::Count) mouse_hook_ = nullptr;

		// scroll hook pointer so it can be unhooked when the object is destroyed
		EVENT_FUNC_INDEX(sf::Event, sf::Event::EventType::Count) scroll_hook_ = nullptr;

		// Parent frame that we are being displayed on
		Frames::IFrame * parent_;

		// Number of UI Elemnt columns
		unsigned cols_;

		// Number of UI Element rows
		unsigned rows_;

		// Current scroll index
		unsigned row_scroll_position_ = 0;

		/**
		 * This function will iterate over all of the currently visible elements
		 * and call the passed in lambda with the indexing information for 
		 * the current element.
		 * 
		 * action arguments
		 * 1. Current option index
		 * 2. i current row
		 * 3. j currnet column
		 * 
		 * @param action
		 */
		template<typename Functor>
		void iterate_over_visible_elements(const Functor& action) const
		{
			// Iterate over all of the visible rows based on scroll position
			for (auto i = row_scroll_position_; i < rows_ + row_scroll_position_; ++i)
			{
				// iterate over columns, making sure we don't extend past the number of elements that exist
				for (unsigned j = 0; j < cols_ && i * cols_ + j < static_cast<unsigned>(options_.size()); ++j)
				{
					// Call the passed in function with the action arguments
					action(i * cols_ + j, i, j);
				}
			}
		}

		/**
		 * Set keyboard and mouse hooks for the instance.
		 * Should only be called once in the constructor.
		 */
		void set_hooks();

	public:
		/**
		 * Options constructor.
		 * 
		 * @param p Parent frame
		 * @param options_per_box how many rows of options to display at once
		 * @param columns how many columns there are
		 */
		explicit Options(Frames::IFrame * p, unsigned options_per_box = 6, unsigned columns = 1);

		/**
		 * Options constructor.
		 *
		 * @param p Parent frame
		 * @param x X position
		 * @param y Y position
		 * @param w Width
		 * @param h Height
		 * @param options_per_box how many rows of options to display at once
		 * @param columns how many columns there are
		 */
		explicit Options(Frames::IFrame * p, float x, float y, float w, float h, unsigned options_per_box = 6, unsigned columns = 1);

		/**
		 * Options constructor.
		 *
		 * @param p Parent frame
		 * @param top Top left coordinates
		 * @param bottom Bottom Right coordinates
		 * @param options_per_box how many rows of options to display at once
		 * @param columns how many columns there are
		 */
		explicit Options(Frames::IFrame * p, sf::Vector2f top, sf::Vector2f bottom, unsigned options_per_box = 6, unsigned columns = 1);

		/**
		 * Options constructor.
		 *
		 * @param p Parent frame
		 * @param bounds Options bounds
		 * @param options_per_box how many rows of options to display at once
		 * @param columns how many columns there are
		 */
		explicit Options(Frames::IFrame * p, sf::FloatRect bounds, unsigned options_per_box = 6, unsigned columns = 1);

		/**
		 * Adds an option onto the end of the option display to be rendered.
		 * 
		 * @param element Pointer to the new UI Element
		 */
		void add_option(std::unique_ptr<UIElement> element);

		/**
		  * Adds an option onto the end of the option display to be rendered.
		  * 
		  * @param element Pointer to the new UI Element
		  */
		void add_option(UIElement * element);

		/**
		 * Clears the options list
		 */
		void remove_options();

		/**
		 * The draw method for an options instance. Overloads from sf::Drawable
		 * 
		 * @param target rendertarget instance to draw on
		 * @param states render states to use
		 */
		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

		/**
		 * The update method for an options instance. Overloads from Interfaces::IUpdateable
		 * 
		 * @param dt the change in time that has passed since the last call
		 */
		void update(const sf::Time& dt) override;

		/**
		 * Sets the top left position of the overall options instance on the screen.
		 * 
		 * @param x the new left location
		 * @param y the new top location
		 */
		void set_position(float x, float y);

		/**
		 * Returns the current position in px of the options box on the screen
		 * from the top left of the screen (I think. Change this if it's wrong)
		 * 
		 * @return a position vector containing the coordiantes
		 */
		sf::Vector2f get_position() const;

		/**
		 * Returns the current width and height in px of the options instance.
		 * (I think, change this if it's wrong)
		 * 
		 * @return A size vector containing width and height
		 */
		sf::Vector2f get_size() const;

		/**
		 * Sets the width and height of the options box, also resizes all
		 * elements, so use sparingly
		 * Probably not optimized
		 * 
		 * @param w The new width in px
		 * @param h the new height in px
		 */
		void set_width_height(float w, float h);

		/**
		 * Set position, and size in one go. Just makes calls to the underlying
		 * functions set_width_height and set_position
		 * 
		 * @param x left
		 * @param y top
		 * @param w width
		 * @param h height
		 */
		void set_position_width_height(float x, float y, float w, float h);

		/**
		 * Destructor
		 */
		~Options();
	};
}
