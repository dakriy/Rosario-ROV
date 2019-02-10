#pragma once

/**
 * @file TextFactory.h
 * Factory for generating renderable 2d text
 *
 * @author dakriy
 */

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <array>

namespace Factory
{
	/**
	 * Different processable font types
	 * When a new font type is added,
	 * it should also be added to this list.
	 */
	enum FontTypes
	{
		DefaultFont,
		None
	};

	/**
	 * Different processable text styles
	 * When a new text style is added,
	 * it should also be added to this list.
	 */
	enum TextStyles
	{
		DefaultStyle,
		OptionStyle,
		BannerStyle,
		ConsoleStyle,
		NoStyle
	};

	class TextFactory
	{
	protected:
		/**
		 * Apply the style paramters to a renderable text element
		 * 
		 * @param text the renderable text element to display
		 * @param styles the text styles to apply
		 */
		static void apply_text_style(sf::Text& text, TextStyles styles);
	public:
		// Font list
		static std::array<sf::Font, None> font;
		
		/**
		 * Loads font and text assets
		 * 
		 * @return success
		 */
		static bool load_assets();

		/**
		 * Gets a displayble string with the desired font and styles
		 * 
		 * @param str the string to display
		 * @param style the text style
		 * @param type the font to use
		 * @return renderable text to display
		 */
		static sf::Text create_displayable_string(const char * str = "", TextStyles style = TextStyles::DefaultStyle, FontTypes type = FontTypes::DefaultFont);

		/**
		 * Gets a displayble string with the desired font and styles
		 *
		 * @param str the string to display
		 * @param style the text style
		 * @param type the font to use
		 * @return renderable text to display
		 */
		static sf::Text create_displayable_string(const std::string& str, TextStyles style = TextStyles::DefaultStyle, FontTypes type = FontTypes::DefaultFont);
	};
}
