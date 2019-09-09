#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <SFML/Graphics.hpp>
#include "Scene.hpp"
#include "ResourceHolder.hpp"
#include "ResourceIdentifier.hpp"
#include "CommandQueue.hpp"
#include "Category.hpp"


class Entity : public SceneNode
{
    public:
        explicit Entity(int hitpoints);

        void repair(int hp);
        void damage(int hp);
        void destroy();

        virtual void remove();

        int getHitpoints() const;
        virtual bool isDestroyed() const override;

        void setVelocity(sf::Vector2f velocity);
        void setVelocity(float vx , float vy);
        sf::Vector2f getVelocity() const;

        void accelerate(sf::Vector2f velocity);
        void accelerate(float vx, float vy);
        virtual void playLocalSound(CommandQueue& commands, SoundEffect::ID effect);
    protected:
        virtual void updateCurrent(sf::Time dt, CommandQueue&) override;
    private:
        int mHitpoints;
    protected:
        sf::Vector2f mVelocity;
};

#endif