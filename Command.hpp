#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <functional>
#include <SFML/System/Time.hpp>
#include "Category.hpp"

class SceneNode;

struct Command{
        Command();
        std::function<void(SceneNode&, sf::Time)> action;
        unsigned int category;
};

#endif