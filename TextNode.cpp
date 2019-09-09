#include "TextNode.hpp"
#include "Utility.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>


TextNode::TextNode(const FontHolder& fh, const std::string& text)
{
    mText.setFont( fh.get(Fonts::Main) );
    mText.setCharacterSize(20);
    setString(text);
}

void TextNode::setString(const std::string& text)
{
    mText.setString(text);
    centerOrigin(mText);
}

void TextNode::setColor(sf::Color color)
{
    mText.setFillColor(color);
}

void TextNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(mText, states);
}