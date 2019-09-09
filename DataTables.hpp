#ifndef DATATABLES_HPP
#define DATATABLES_HPP

#include "ResourceIdentifier.hpp"

#include <vector>
#include <functional>

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Color.hpp>

/* The data for each kind of Cloud member. */
struct Direction
{
    Direction(float angle, float direction);
    float angle;
    float direction;
};

struct CloudData{
    int hitpoints = 1;
    float speed = 0;
    sf::Time fireInterval = sf::Time::Zero;
    Textures::ID texture;
    sf::IntRect textureRect;
    std::vector<Direction> directions;
    bool hasRollAnimation = false;
};

//function to initialize the cloud data
std::vector<CloudData>  initializeCloudData();

/* The data for each type of projectile in the game */

struct ProjectileData{
    float speed = 1;
    float damage = 1;
    Textures::ID texture;
    sf::IntRect textureRect;
};

//function to initialize the projectile data
std::vector<ProjectileData>  initializeProjectileData();

/* The data for each type of pickup in the game. */
class Cloud;
struct PickupData{
    Textures::ID texture;
    sf::IntRect textureRect;
    std::function<void(Cloud&)> action;
};
//function to initialize the pickup data
std::vector<PickupData> initializePickupData();

/* The data for each type of particle in the game. */
struct ParticleData{
    sf::Color color;
    sf::Time lifetime;
};

std::vector<ParticleData> initializeParticleData();

#endif