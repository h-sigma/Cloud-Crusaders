#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP


#include "StateStack.hpp"
#include <SFML/Graphics.hpp>

#include "World.hpp"

class GameState : public State
{
    public:
        GameState(StateStack& StateStack, Context context);

        virtual bool handleEvent(const sf::Event& event) override;
        virtual bool update(sf::Time dt) override;
        virtual void draw() override;
    private:
        World mWorld;
        Player& mPlayer;
};

#endif