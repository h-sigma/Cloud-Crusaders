#include "SpriteNode.hpp"
#include "Category.hpp"

//SpriteNode
SpriteNode::SpriteNode( const sf::Texture& texture) 
: SceneNode(Category::None)
, sprite(texture)
{
}

SpriteNode::SpriteNode( const sf::Texture& texture , const sf::IntRect& rect) : sprite(texture, rect)
{
}

void SpriteNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(sprite , states);
}
