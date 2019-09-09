#ifndef PROJECTILE_HPP
#define PROJECTILE_HPP

#include "Entity.hpp"
#include "CommandQueue.hpp"

class Projectile : public Entity
{
    public:
        enum Type{
            AlliedBullet, 
            EnemyBullet,
            Missile,
            TypeCount
        };
    public:
        Projectile(Type type, const TextureHolder& tholder);

        bool guideTowards(sf::Vector2f position);
        bool isGuided() const;

        virtual unsigned int getCategory() const override;
        virtual sf::FloatRect getBoundingRect() const override;
        float getMaxSpeed() const;
        float getDamage() const;
    private:
        virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;
        virtual void updateCurrent(sf::Time dt, CommandQueue& commands);
    private:
        Type mType;
        sf::Sprite mSprite;
        sf::Vector2f mTargetDirection;
};

#endif