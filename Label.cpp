#include "Label.hpp"
#include "Utility.hpp"
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

GUI::Label::Label(const std::string& text, const FontHolder& fonts)
: mText(text , fonts.get(Fonts::Label) , 16)
{
    centerOrigin(mText);
}

bool GUI::Label::isSelectable() const
{
    return false;
}

void GUI::Label::handleEvent(const sf::Event&)
{
}

void GUI::Label::draw(sf::RenderTarget& renderTarget,sf::RenderStates states) const
{
    states.transform *= getTransform();
    renderTarget.draw(mText, states);
}

void GUI::Label::setText(std::string text)
{
    mText.setString(text);
}