#pragma once
#include <functional>
#include <SFML/Graphics.hpp>
#include "UIElement.h"

namespace Controls
{
	class Button : public UIElement {
	protected:
		sf::Text display_string_;
		std::function<void()> event_func;
	public:

		Button(const std::string& text, const std::function<void()>& callback);
		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
		void set_position(float x, float y) override;
		void set_width(float w) override;
		void set_height(float h) override;
		void set_width_height(float w, float h) override;
		void set_position_width_height(float x, float y, float w, float h) override;
		void set_display_string(std::string&);
		void set_display_string(const char *);
		void fit_to_center() override;
		std::string get_text() const override;
		void onLeftClick() override;
		~Button() = default;
	};
}
