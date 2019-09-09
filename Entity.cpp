#include "Entity.hpp"

Entity::Entity(int hitpoints)
: mHitpoints(hitpoints)
{
}

void Entity::remove()
{
    destroy();
}

void Entity::repair(int hp)
{
    mHitpoints += hp;
}

void Entity::damage(int hp)
{
    if(hp < 0)
        return;
    mHitpoints -= hp;
}

void Entity::destroy()
{
    mHitpoints = 0;
}

int Entity::getHitpoints() const
{
    return mHitpoints;
}

bool Entity::isDestroyed() const
{
    return mHitpoints <= 0;
}

void Entity::setVelocity(sf::Vector2f velocity)
{
    mVelocity = velocity;
}
void Entity::setVelocity(float vx , float vy)
{
    mVelocity.x = vx;
    mVelocity.y = vy;
}
sf::Vector2f Entity::getVelocity() const
{
    return mVelocity;
}

void Entity::playLocalSound(CommandQueue&, SoundEffect::ID )
{
}

void Entity::accelerate(sf::Vector2f velocity)
{
	mVelocity += velocity;
}

void Entity::accelerate(float vx, float vy)
{
	mVelocity.x += vx;
	mVelocity.y += vy;
}


void Entity::updateCurrent(sf::Time dt, CommandQueue&)
{
    move(mVelocity * dt.asSeconds());
}
