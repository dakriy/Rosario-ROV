#include "TextFactory.h"
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>

std::array<sf::Font, Factory::None> Factory::TextFactory::font;

void Factory::TextFactory::apply_text_style(sf::Text& t, TextStyles style)
{
	// Apply proper style
	// All styles are defined here
	// @todo find a better place to define styles, maybe have a styles class?
	switch(style)
	{
	case DefaultStyle:
		t.setFillColor(sf::Color::Black);
		break;
	case OptionStyle:
		t.setFillColor(sf::Color::White);
		break;
	case BannerStyle:
		t.setFillColor(sf::Color::Black);
		t.setStyle(sf::Text::Bold | sf::Text::Italic);
		break;
	case ConsoleStyle:
		t.setFillColor(sf::Color::White);
		break;
	case NoStyle:
	default:
		break;
	}
}

bool Factory::TextFactory::load_assets()
{
	// Load all assets and return success or failure
	return font[DefaultFont].loadFromFile("Resources/Papyrus.ttf");
}

sf::Text Factory::TextFactory::create_displayable_string(const char* str, const TextStyles style, FontTypes type)
{
	// Can't be the count type
	if (type == None)
		type = DefaultFont;

	// Build an sf text
	sf::Text t(
		str,
		font[type]
	);

	// Apply the proper text styles
	apply_text_style(t, style);

	// Return displayable string
	return t;
}

sf::Text Factory::TextFactory::create_displayable_string(const std::string& str, const TextStyles style, FontTypes type)
{
	// Can't be the count type
	if (type == None)
		type = DefaultFont;

	// Build an sf text
	sf::Text t(
		str,
		font[type]
	);

	// Apply proper text styles
	apply_text_style(t, style);

	// Return displayable string
	return t;
}
