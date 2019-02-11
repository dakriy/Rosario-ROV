#pragma once

/**
 * @file IFrame.h
 * Class definition for frame interface.
 * It defines the different types of frames and has
 * all basic operations of frames defined on it
 *
 * @author dakriy
 */

#include <SFML/Graphics.hpp>
#include "../Interfaces/IUpdateable.h"

namespace Frames
{
	/**
	 * Defines all of the different frame types,
	 * or 'screens'
	 */
	enum FrameType
	{
		MainMenu,
		Connect,
		View,
		Credits,

		// FrameType Count, Leave last
		Count
	};

	class IFrame : public Interfaces::IUpdateable, public sf::Drawable
	{
		// Flag for the engine that tells if the frame is hidden
		bool hidden = false;
		// Flag for the engine that tells if the frame is paused
		bool paused = false;
	protected:
		// Window pointer
		sf::RenderWindow * window_;
	public:
		/**
		 * IFrame Constructor,
		 * Sets the window from the global context
		 */
		IFrame();

		/**
		 * Shows the frame
		 */
		void show();

		/**
		 * Hides the frame
		 */
		void hide();

		/**
		 * Pauses the frame
		 */
		void pause();

		/**
		 * Plays the frame
		 */
		void unpause();

		/**
		 * Is the frame hidden?
		 */
		bool isHidden() const;

		/**
		 * Is the frame paused?
		 */
		bool isPaused() const;

		/**
		 * Virtual default deconstructor
		 * as this is an interface
		 */
		virtual ~IFrame() = default;

		/**
		 * Returns the frame type if it is ever needed
		 * Is used in equality
		 */
		virtual FrameType get_type() const = 0;

		/**
		 * Two frames are equal if they are the same type
		 * 
		 * @param lhs left hand side
		 * @param rhs right hand side
		 */
		friend bool operator==(const IFrame & lhs, const IFrame & rhs);

		/**
		 * Two frames are not equal if the equal operator returns false
		 * 
		 * @param lhs left hand side
		 * @param rhs right hand side
		 */
		friend bool operator!=(const IFrame & lhs, const IFrame & rhs);
	};
}
