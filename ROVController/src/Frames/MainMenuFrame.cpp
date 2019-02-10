#include "MainMenuFrame.h"
#include "../Factories/TextFactory.h"
#include "../Core/GlobalContext.h"

sf::Texture Frames::MainMenuFrame::background_tex;

Frames::MainMenuFrame::MainMenuFrame(const char * text)
{
	update_bounds();

	background_.setTexture(background_tex);
	background_.scale(window_->getSize().x / (background_.getLocalBounds().width), window_->getSize().y / background_.getLocalBounds().height);

	title_ = Factory::TextFactory::create_displayable_string(text, Factory::BannerStyle);
	title_.setCharacterSize(static_cast<unsigned>(static_cast<float>(window_->getSize().y) / 7.f));
	// Center the text and set it 1/50th of the way down on the screen.
	title_.setPosition((window_->getSize().x - title_.getLocalBounds().width) / 2.f, window_->getSize().y / 50.f);
}

void Frames::MainMenuFrame::update_bounds()
{
	content_bounds_.left = window_->getSize().x * 1.f / 8.f;
	content_bounds_.top = window_->getSize().y * 0.25f;
	content_bounds_.width = window_->getSize().x * 7.f / 8.f - window_->getSize().x * 1.f / 8.f;
	content_bounds_.height = window_->getSize().y * 0.9f - window_->getSize().y * 0.25f;
}

void Frames::MainMenuFrame::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(background_);

	target.draw(title_);
}
