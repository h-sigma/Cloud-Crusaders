#ifndef SPRITENODE_HPP
#define SPRITENODE_HPP

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include "Scene.hpp"



class SpriteNode : public SceneNode {
    public:
        explicit SpriteNode (const sf::Texture& texture);
        explicit SpriteNode (const sf::Texture& texture, const sf::IntRect& rect);
    private:
        virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;
    private:
        sf::Sprite sprite;
};

#endif