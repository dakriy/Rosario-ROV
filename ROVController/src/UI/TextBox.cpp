#include "TextBox.h"
#include "../Factories/TextFactory.h"
#include <iterator>

Controls::TextBox::TextBox(std::function<void(std::string)> onTextChange, std::function<void(std::string)> onEnter, const std::string& startText) : display_string_bounds_ {0}
{
	text_box_.setFillColor(sf::Color(sf::Color::White));
	cursor_.setFillColor(sf::Color::Black);
	text_buffer_ = startText;
	text_ = Factory::TextFactory::create_displayable_string(startText);
	display_string_bounds_[0] = 0;
	display_string_bounds_[1] = std::distance(text_buffer_.begin(), text_buffer_.end());
	// Cursor pos is an absolute position.
	cursor_pos_ = text_buffer_.size();
	cursor_.setPosition(text_.findCharacterPos(cursor_pos_ - display_string_bounds_[0]));
	keyboard_hook_ = GlobalContext::get_event_handler()->add_event_callback([this, onTextChange, onEnter](const sf::Event * e) -> bool {
		auto retval = false;
		if (!display_cursor_)
			return retval;
		if (e->type == sf::Event::KeyPressed)
		{
			if (e->key.code == sf::Keyboard::Left)
			{
				if (cursor_pos_ > 0)
				{
					--cursor_pos_;
					retval = true;
				}
			}
			else if (e->key.code == sf::Keyboard::Right)
			{
				if (cursor_pos_ < text_buffer_.size())
				{
					++cursor_pos_;
					retval = true;
				}
			}
			else if (e->key.code == sf::Keyboard::Delete)
			{
				if (cursor_pos_ < text_buffer_.size())
				{
					text_buffer_.erase(text_buffer_.begin() + cursor_pos_);
					retval = true;
				}
			}
		}
		if (e->type == sf::Event::TextEntered)
		{
			if (e->text.unicode == '\b') {
				if (!text_buffer_.empty() && cursor_pos_ > 0)
				{
					text_buffer_.erase(text_buffer_.begin() + cursor_pos_ - 1);
					if (display_string_bounds_[0] > 0)
						--display_string_bounds_[0];
					if (display_string_bounds_[1] > 0)
						--display_string_bounds_[1];
					--cursor_pos_;
				}
				retval = true;
			}
			else if (e->text.unicode < 0x80 && e->text.unicode != 0x0D) {
				const auto c = static_cast<char>(e->text.unicode);

				text_buffer_.insert(cursor_pos_, 1, c);
				cursor_pos_++;
				retval = true;
			}
			else
				retval = false;
			if (retval)
			{
				if (e->text.unicode == 0x0D && onEnter)
					onEnter(text_buffer_);
				else if (onTextChange)
					onTextChange(text_buffer_);
			}
		}

		text_.setString(text_buffer_);
		// If the whole string fits in the allotted space
		if (text_.getLocalBounds().width < text_box_.getLocalBounds().width)
		{
			display_string_bounds_[0] = 0;
			display_string_bounds_[1] = std::distance(text_buffer_.begin(), text_buffer_.end());
		}
		else
		{
			// Clip the string to fit
			text_.setString(text_buffer_.substr(display_string_bounds_[0], display_string_bounds_[1] - display_string_bounds_[0]));

			// If cursor is outside on the right, shfit the string to the right
			while (cursor_pos_ > display_string_bounds_[1])
				++display_string_bounds_[1];
			// If cursor is outside on the left, shfit the string to the left
			while (cursor_pos_ < display_string_bounds_[0])
				--display_string_bounds_[0];

			text_.setString(text_buffer_.substr(display_string_bounds_[0], display_string_bounds_[1] - display_string_bounds_[0]));

			while (text_.getLocalBounds().width > text_box_.getLocalBounds().width && !text_.getString().isEmpty())
			{
				/*
				 * Get middle of the display text
				 *
				 * |-------------------------|
				 * C C C C C C C [C C CmC C C] C C C C C
				 * |-------------|
				 * middle = (top - bot) / 2 + bot
				 * middle = top / 2 + bot / 2
				 */
				const auto middle_displayable_pos = static_cast<unsigned>(display_string_bounds_[1] / 2.f + display_string_bounds_[0] / 2.f);
				// If cursor is on right half lob off of the left
				if (cursor_pos_ > middle_displayable_pos)
				{
					++display_string_bounds_[0];
					text_.setString(text_buffer_.substr(display_string_bounds_[0], display_string_bounds_[1] - display_string_bounds_[0]));
				}
				else // Else lob off the right
					text_.setString(text_buffer_.substr(display_string_bounds_[0], --display_string_bounds_[1] - display_string_bounds_[0]));
			}
		}

		cursor_.setPosition(text_.findCharacterPos(cursor_pos_ - display_string_bounds_[0]));

		return retval;
	}, sf::Event::TextEntered, sf::Event::KeyPressed);
}

void Controls::TextBox::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(box);
	target.draw(text_box_);
	if (display_cursor_)
		target.draw(cursor_);
	target.draw(text_);
}

void Controls::TextBox::onLeftClick()
{
	display_cursor_ = true;
}

void Controls::TextBox::onLeftClickLeave()
{
	display_cursor_ = false;
}

std::string Controls::TextBox::get_text() const
{
	return text_buffer_;
}

void Controls::TextBox::set_width(const float w)
{
	box.setSize(sf::Vector2f(w, box.getLocalBounds().height));

	text_box_.setSize(sf::Vector2f(w - TEXT_BOX_MARGIN_2, text_box_.getLocalBounds().height));

	cursor_.setSize(sf::Vector2f(static_cast<float>(CURSOR_WIDTH), cursor_.getLocalBounds().height));	
}

void Controls::TextBox::set_height(const float h)
{
	box.setSize(sf::Vector2f(box.getLocalBounds().width, h));
	text_box_.setSize(sf::Vector2f(text_box_.getLocalBounds().width, h - TEXT_BOX_MARGIN_2));
	// set character size
	text_.setCharacterSize(static_cast<unsigned>(text_box_.getLocalBounds().height * 0.63f));
	cursor_.setSize(sf::Vector2f(cursor_.getLocalBounds().width, text_box_.getLocalBounds().height));
}

void Controls::TextBox::set_position(const float x, const float y)
{
	box.setPosition(x, y);
	text_box_.setPosition(x + TEXT_BOX_MARGIN, y + TEXT_BOX_MARGIN);
	text_.setPosition(text_box_.getPosition());
	cursor_.setPosition(text_.findCharacterPos(cursor_pos_ - display_string_bounds_[0]));
}

void Controls::TextBox::set_width_height(const float w, const float h)
{
	this->set_width(w);
	this->set_height(h);
}

void Controls::TextBox::set_position_width_height(const float x, const float y, const float w, const float h)
{
	set_width_height(w, h);
	set_position(x, y);
}

Controls::TextBox::~TextBox()
{
	GlobalContext::get_event_handler()->unhook_event_callback_for_all_events(keyboard_hook_);
}
