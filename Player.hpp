#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <map>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include "CommandQueue.hpp"
#include "Cloud.hpp"

class Player{
    public:
        enum Action : int {
            moveUp , moveLeft, moveDown, moveRight, Fire, LaunchMissile, ActionCount
        };
    public:
        Player(sf::RenderWindow& window);
        void assignKey(Action action, sf::Keyboard::Key key);
        sf::Keyboard::Key getAssignedKey(Action action);
        void handleEvent(const sf::Event& event, CommandQueue& commands);
        void handleRealTimeInput(CommandQueue& commands);
    private:
        void initializeActions();
        bool isRealTimeAction(Player::Action action) const;
    private:
        sf::RenderWindow& mWindow;
        std::map<sf::Keyboard::Key, Action> mKeyBinding;
        std::map<Action,Command> mActionBinding;
};

#endif