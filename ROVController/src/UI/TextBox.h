#pragma once
#include "UIElement.h"
#include <functional>
#include <array>
#include "../Core/GlobalContext.h"

constexpr auto TEXT_BOX_MARGIN = 20.f;
constexpr auto TEXT_BOX_MARGIN_2 = 2.f * TEXT_BOX_MARGIN;
constexpr auto CURSOR_OFFSET = 4;
constexpr auto CURSOR_WIDTH = 2;

namespace Controls
{
	class TextBox : public UIElement
	{
	protected:
		sf::RectangleShape text_box_;
		sf::RectangleShape cursor_;
		sf::Text text_;
		std::string text_buffer_;
		std::array<size_t, 2> display_string_bounds_;
		bool display_cursor_ = false;
		size_t cursor_pos_ = 0;
		EVENT_FUNC_INDEX(sf::Event, sf::Event::EventType::Count) keyboard_hook_;

	public:
		explicit TextBox(std::function<void(std::string)> onTextChange = nullptr, std::function<void(std::string)> onEnter = nullptr, const std::string& startText = "");

		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

		void onLeftClick() override;
		void onLeftClickLeave() override;

		std::string get_text() const override;

		void fit_to_center() override {}
		void set_width(float w) override;
		void set_height(float h) override;
		void set_position(float x, float y) override;
		void set_width_height(float w, float h) override;
		void set_position_width_height(float x, float y, float w, float h) override;

		~TextBox();
	};
}
