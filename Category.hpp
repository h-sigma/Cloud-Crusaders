#ifndef CATEGORY_HPP
#define CATEGORY_HPP

namespace Category
{
enum Type : unsigned int
{
    None = 0,
    SceneAirLayer = 1 << 0,
    SceneGroundLayer = 1 << 1,
    Player = 1 << 2,
    Ally = 1 << 3,
    Enemy = 1 << 4,
    EnemyProjectile = 1 << 5,
    AllyProjectile = 1 << 6,
    Projectile = AllyProjectile | EnemyProjectile,
    Pickup = 1 << 7,
    ParticleSystem = 1 << 8,
    SoundEffect = 1 << 9
};
}

#include <string_view>

template <typename T = std::nullptr_t>
std::string_view categoryToString(unsigned int type)
{
    switch (static_cast<Category::Type>(type))
    {
    case Category::None:
        return "None ";
    case Category::SceneAirLayer:
        return "SceneAirLayer ";
    case Category::SceneGroundLayer:
        return "SceneGroundLayer ";
    case Category::Player:
        return "Player ";
    case Category::Ally:
        return "Ally ";
    case Category::Enemy:
        return "Enemy ";
    case Category::EnemyProjectile:
        return "EnemyProjectile ";
    case Category::AllyProjectile:
        return "AllyProjectile ";
    case Category::Pickup:
        return "Pickup ";
    case Category::Projectile:
        return "Projectile";
    case Category::SoundEffect:
        return "SoundEffect";
    }
}

#endif