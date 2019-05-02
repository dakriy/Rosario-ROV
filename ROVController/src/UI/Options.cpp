#include "Options.h"
#include <cmath>
#include "../Core/GlobalContext.h"


void Controls::Options::position_options()
{
	const auto row_height = bounds_.height / static_cast<float>(rows_);

	const auto col_width = bounds_.width / static_cast<float>(cols_);

	iterate_over_visible_elements([&](unsigned index, unsigned i, unsigned j)
	{
		options_[index]->set_position_width_height(
			bounds_.left + col_width * j,								// start x changes with column
			bounds_.top + (i - row_scroll_position_) * row_height,		// start y changes with row
			col_width,													// width changes with number of columns
			row_height													// height changes with number of rows
		);
		options_[index]->fit_to_center();
	});
}

Controls::Options::Options(Frames::IFrame* p, const unsigned options_per_box, const unsigned columns) : 
parent_(p), 
cols_(columns), 
rows_(options_per_box)
{
	bounds_.left = bounds_.top = bounds_.width = bounds_.height = 0;
	set_hooks();
}

Controls::Options::Options(Frames::IFrame * p, const float x, const float y, const float w, const float h, const unsigned options_per_box, const unsigned columns) : 
parent_(p), 
cols_(columns), 
rows_(options_per_box)
{
	bounds_.left = x;
	bounds_.top = y;
	bounds_.width = w;
	bounds_.height = h;
	set_hooks();
}

Controls::Options::Options(Frames::IFrame * p, const sf::Vector2f top, const sf::Vector2f bottom, const unsigned options_per_box, const unsigned columns) : 
parent_(p), 
cols_(columns), 
rows_(options_per_box)
{
	bounds_.left = top.x;
	bounds_.top = top.y;
	bounds_.width = bottom.x - top.x;
	bounds_.height = bottom.y - top.y;
	set_hooks();
}


Controls::Options::Options(Frames::IFrame * p, const sf::FloatRect bounds, const unsigned options_per_box, const unsigned columns) : 
parent_(p), 
cols_(columns), 
rows_(options_per_box)
{
	bounds_ = bounds;
	set_hooks();
}

void Controls::Options::set_hooks()
{
	mouse_hook_ = GlobalContext::get_event_handler()->add_event_callback([this](const sf::Event *e)->bool {
		if (this->parent_->isHidden() || this->parent_->isPaused())
			return false;

		iterate_over_visible_elements([&](unsigned index, unsigned& i, unsigned& j)
		{
			if (!options_[index]->isInFocus(e->mouseButton.x, e->mouseButton.y)) {
				if (options_[index]->leftClicked)
				{
					options_[index]->leftClicked = false;
					options_[index]->onLeftClickLeave();
				}
				if (options_[index]->middleClicked)
				{
					options_[index]->middleClicked = false;
					options_[index]->onMiddleClickLeave();
				}
				if (options_[index]->rightClicked)
				{
					options_[index]->rightClicked = false;
					options_[index]->onRightClickLeave();
				}
				return;
			}
			if (e->mouseButton.button == sf::Mouse::Left)
			{
				options_[index]->onLeftClick();
				options_[index]->leftClicked = true;
			}

			if (e->mouseButton.button == sf::Mouse::Middle)
			{
				options_[index]->onMiddleClick();
				options_[index]->middleClicked = true;
			}

			if (e->mouseButton.button == sf::Mouse::Right)
			{
				options_[index]->onRightClick();
				options_[index]->rightClicked = true;
			}
		});

		return true;
	}, sf::Event::MouseButtonPressed);

	scroll_hook_ = GlobalContext::get_event_handler()->add_event_callback([this](const sf::Event *e)->bool {
		if (this->parent_->isHidden() || this->parent_->isPaused())
			return false;
		if (e->type == sf::Event::MouseWheelScrolled)
		{
			if (!bounds_.contains(
					static_cast<float>(e->mouseWheelScroll.x),
					static_cast<float>(e->mouseWheelScroll.y)) || e->mouseWheelScroll.wheel != sf::Mouse::VerticalWheel)
				return false;
			// Scrolled up
			if (e->mouseWheelScroll.delta > 0 && this->row_scroll_position_ > 0)
				--this->row_scroll_position_;
			// Scrolled down
			if (e->mouseWheelScroll.delta < 0 && static_cast<int>(this->row_scroll_position_) < static_cast<int>(std::ceil(static_cast<float>(options_.size()) / static_cast<float>(cols_))) - static_cast<int>(rows_))
				++this->row_scroll_position_;
			// idek
			if (e->mouseWheelScroll.delta == 0)
				return false;
			position_options();
			return true;
		}
		return false;
	}, sf::Event::MouseWheelScrolled);
}

void Controls::Options::add_option(std::unique_ptr<UIElement> element)
{
	options_.push_back(move(element));
	position_options();
}

void Controls::Options::add_option(UIElement* element)
{
	options_.emplace_back(element);
	position_options();
}

void Controls::Options::remove_options()
{
	options_.clear();
}

void Controls::Options::draw(sf::RenderTarget& target, const sf::RenderStates states) const
{
	iterate_over_visible_elements([&](unsigned index, unsigned, unsigned)
	{
		target.draw(*options_[index]);
	});
}

void Controls::Options::update(const sf::Time& dt)
{
	mouse_on_option_ = false;
	iterate_over_visible_elements([&](unsigned index, unsigned, unsigned)
	{
		const auto current_focus_state = options_[index]->isInFocus(sf::Mouse::getPosition());
		// Avoid doing the work of checking the mouse on the other options if we already found where the mouse is at.
		if (!mouse_on_option_ && current_focus_state)
		{
			if (!options_[index]->lastHoverFocusState)
				options_[index]->onHoverFocusEnter();

			options_[index]->onHoverFocusUpdate(dt);

			mouse_on_option_ = true;
		}
		else
		{
			if (options_[index]->lastHoverFocusState)
				options_[index]->onHoverFocusLeave();
		}

		options_[index]->lastHoverFocusState = current_focus_state;
		options_[index]->update(dt);
	});
}

void Controls::Options::set_position(const float x, const float y)
{
	bounds_.left = x;
	bounds_.top = y;

	position_options();
}

sf::Vector2f Controls::Options::get_position() const
{
	return sf::Vector2f(bounds_.left, bounds_.top);
}

sf::Vector2f Controls::Options::get_size() const
{
	return sf::Vector2f(bounds_.width, bounds_.height);
}

void Controls::Options::set_width_height(const float w, const float h)
{
	bounds_.width = w;
	bounds_.height = h;

	position_options();
}

void Controls::Options::set_position_width_height(const float x, const float y, const float w, const float h)
{
	// Avoid repositioning the options until it's done.
	bounds_.left = x;
	bounds_.top = y;
	set_width_height(w, h);
}

Controls::Options::~Options()
{
	GlobalContext::get_event_handler()->unhook_event_callback_for_all_events(mouse_hook_);
	GlobalContext::get_event_handler()->unhook_event_callback_for_all_events(scroll_hook_);
}
