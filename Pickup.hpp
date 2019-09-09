#ifndef PICKUP_HPP
#define PICKUP_HPP

#include "Entity.hpp"
#include "ResourceIdentifier.hpp"

class Cloud;
namespace sf{
    class RenderStates;
    class RenderTarget;
}

#include <SFML/Graphics/Sprite.hpp>

class Pickup : public Entity
{
    public:
        enum Type {
            HealthRefill,
            MissileRefill,
            FireSpread,
            FireRate,
            TypeCount
        };
    public:
        explicit Pickup(Type type, const TextureHolder& textures);
        virtual unsigned int getCategory() const override;
        virtual sf::FloatRect getBoundingRect() const;
        void apply(Cloud& cloud) const;
    private:
        virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;
    private:
        Type mType;
        sf::Sprite mSprite;
};

#endif