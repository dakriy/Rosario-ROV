#pragma once
#include <SFML/Graphics.hpp>
#include "../Interfaces/IUpdateable.h"
#include <string>

namespace Controls
{
	class UIElement : sf::Drawable, Interfaces::IUpdateable
	{
		friend class Options;
		bool leftClicked = false;
		bool middleClicked = false;
		bool rightClicked = false;
		bool lastHoverFocusState = false;
		unsigned col_span_ = 1;
	protected:
		sf::RectangleShape box;
	public:
		UIElement();

		// Used for the options class.
		bool isInFocus(sf::Vector2i mouse_pos) const;
		bool isInFocus(int x, int y) const;

		// Event functions that can be overridden, but don't need to be.
		virtual void onLeftClick() {}
		virtual void onRightClick() {}
		virtual void onMiddleClick() {}
		// User clicked somehwere else out of the scope of the UI Element.
		virtual void onLeftClickLeave() {}
		virtual void onRightClickLeave() {}
		virtual void onMiddleClickLeave() {}
		virtual void onHoverFocusEnter();
		virtual void onHoverFocusLeave();
		virtual void onHoverFocusUpdate(const sf::Time& dt) {}

		void set_col_span(unsigned span);
		unsigned get_col_span() const;

		virtual std::string get_text() const = 0;

		void update(const sf::Time& dt) override {}

		// These function need to be overridden in derived classes
		virtual void fit_to_center() = 0;
		virtual void set_width(float w) = 0;
		virtual void set_height(float h) = 0;
		virtual void set_position(float x, float y) = 0;
		virtual void set_width_height(float w, float h) = 0;
		virtual void set_position_width_height(float x, float y, float w, float h) = 0;
	};
}
