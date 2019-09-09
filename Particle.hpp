#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Time.hpp>

struct Particle
{
    enum Type
    {
        Smoke,
        Propellant,
        ParticleCount
    };
    sf::Vector2f position;
    sf::Color color;
    sf::Time lifetime;
};


#endif