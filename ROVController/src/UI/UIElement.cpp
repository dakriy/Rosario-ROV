#include <imgui.h>
#include "UIElement.h"

Controls::UIElement::UIElement()
{
	box.setOrigin(0, 0);
	box.setFillColor(sf::Color(0, 0, 0, 200));
}

bool Controls::UIElement::isInFocus(const sf::Vector2i mouse_pos) const
{
	return isInFocus(mouse_pos.x, mouse_pos.y);
}

bool Controls::UIElement::isInFocus(const int x, const int y) const
{
	if (ImGui::IsMouseHoveringAnyWindow())
		return false;
	return box.getGlobalBounds().contains(static_cast<float>(x), static_cast<float>(y));
}

void Controls::UIElement::onHoverFocusEnter()
{
	box.setFillColor(sf::Color(200, 200, 200, 200));
}

void Controls::UIElement::onHoverFocusLeave()
{
	box.setFillColor(sf::Color(0, 0, 0, 200));
}

void Controls::UIElement::set_col_span(unsigned span)
{
	col_span_ = span;
}

unsigned Controls::UIElement::get_col_span() const
{
	return col_span_;
}
