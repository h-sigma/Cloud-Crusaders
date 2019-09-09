#include "Projectile.hpp"
#include "Utility.hpp"
#include "Category.hpp"
#include "DataTables.hpp"
#include "EmitterNode.hpp"

#include <cassert>
#include <cmath>

namespace
{
    const std::vector<ProjectileData> Table = initializeProjectileData();
}

Projectile::Projectile(Type type, const TextureHolder& tholder)
: Entity(1)
, mType(type)
, mSprite( tholder.get(Table[type].texture), Table[type].textureRect)
{
    if(type == Projectile::Type::AlliedBullet || type == Projectile::Type::EnemyBullet)
        mSprite.setScale(1.f / 3.f, 1.f / 3.f);
    if(isGuided())
    {
        std::unique_ptr<EmitterNode> smokeEmitter = std::make_unique<EmitterNode>(Particle::Type::Smoke);
        smokeEmitter->setPosition(0.f, getBoundingRect().height / 2.f);
        attachChild(std::move(smokeEmitter));

        std::unique_ptr<EmitterNode> propellantEmitter = std::make_unique<EmitterNode>(Particle::Type::Propellant);
        propellantEmitter->setPosition(0.f, getBoundingRect().height / 2.f);
        attachChild(std::move(propellantEmitter));
    }
    centerOrigin(mSprite);
}

bool Projectile::guideTowards(sf::Vector2f position)
{
    assert(isGuided());
    mTargetDirection = unitVector(position - getWorldPosition());
}   

bool Projectile::isGuided() const
{
    return mType == Type::Missile;
}

unsigned int Projectile::getCategory() const
{
    if(mType == Type::EnemyBullet)
        return Category::EnemyProjectile;
    else
        return Category::AllyProjectile;
}

sf::FloatRect Projectile::getBoundingRect() const
{
    return getWorldTransform().transformRect(mSprite.getGlobalBounds());
}

float Projectile::getMaxSpeed() const
{
    return Table[mType].speed;
}

float Projectile::getDamage() const
{
    return Table[mType].damage;
}

void Projectile::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(mSprite, states);
}

void Projectile::updateCurrent(sf::Time dt, CommandQueue& commands)
{
    if(isGuided())
    {
        const float approachRate = 200.f;
        sf::Vector2f newVelocity = unitVector( approachRate * mTargetDirection * dt.asSeconds() + getVelocity());
        newVelocity *= getMaxSpeed();

        float angle = std::atan2(newVelocity.y , newVelocity.x);

        setRotation(toDegree(angle) + 90.f);
        setVelocity( newVelocity);
    }

    Entity::updateCurrent(dt, commands);
}   
