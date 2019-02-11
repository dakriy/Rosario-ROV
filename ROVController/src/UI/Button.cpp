#include "Button.h"

#include "../Factories/TextFactory.h"

Controls::Button::Button(const std::string& text, const std::function<void()> & callback) : display_string_(Factory::TextFactory::create_displayable_string(text, Factory::OptionStyle))
{
	display_string_.setCharacterSize(0);
	event_func = callback;
}

void Controls::Button::draw(sf::RenderTarget& target, const sf::RenderStates states) const
{
	target.draw(box);
	target.draw(display_string_);
}

void Controls::Button::set_position(const float x, const float y)
{
	box.setPosition(x, y);
	display_string_.setPosition(x, y);
}

void Controls::Button::set_width(const float w)
{
	box.setSize(sf::Vector2f(w, box.getLocalBounds().height));
}

void Controls::Button::set_height(const float h)
{
	box.setSize(sf::Vector2f(box.getLocalBounds().width, h));
}

void Controls::Button::set_width_height(const float w, const float h)
{
	box.setSize(sf::Vector2f(w, h));
}

void Controls::Button::set_position_width_height(float x, float y, float w, float h)
{
	box.setPosition(x, y);
	display_string_.setPosition(x, y);
	box.setSize(sf::Vector2f(w, h));
}

void Controls::Button::set_display_string(std::string& s)
{
	display_string_.setString(s);
}

void Controls::Button::set_display_string(const char* s)
{
	display_string_.setString(s);
}

void Controls::Button::fit_to_center()
{
	// make sure it fits and resize height.
	display_string_.setCharacterSize(static_cast<unsigned int>(3.f * box.getSize().y / 5.f));
	const auto bounds = display_string_.getLocalBounds();
	if (bounds.width > box.getSize().x)
		display_string_.setCharacterSize(static_cast<unsigned int>((box.getSize().y * box.getSize().x) / bounds.width));

	// center
	display_string_.setPosition((box.getLocalBounds().width - display_string_.getLocalBounds().width) / 2.f + box.getGlobalBounds().left, box.getGlobalBounds().top);
}

std::string Controls::Button::get_text() const
{
	return display_string_.getString();
}

void Controls::Button::onLeftClick()
{
	event_func();
}
