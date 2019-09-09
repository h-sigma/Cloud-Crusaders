#ifndef UTILITY_HPP
#define UTILITY_HPP


#include <SFML/Window/Event.hpp>

#include <string>
#include <type_traits>
#include <cmath>

class Animation;

namespace sf
{
    class Sprite;
    class Text;
}

std::string toString(sf::Keyboard::Key key);

void centerOrigin(sf::Sprite& sprite);

void centerOrigin(sf::Text& text);

void centerOrigin(Animation& animation);

float toRadian(float degree);

float toDegree(float radian);

float magnitude(sf::Vector2f vector);

sf::Vector2f unitVector(sf::Vector2f vector);

int randomInt(int exclusiveMax);


#endif