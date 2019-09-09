#ifndef PAUSESTATE_HPP
#define PAUSESTATE_HPP

#include "Container.hpp"
#include "State.hpp"
#include <SFML/Graphics.hpp>

class PauseState:public State
{
    public:
        PauseState(StateStack& mStateStack, Context context);
        ~PauseState();
        virtual void draw() override;
        virtual bool update(sf::Time dt) override;
        virtual bool handleEvent(const sf::Event& event) override;
    private:
        GUI::Container mGUIContainer;
        MusicPlayer& mMusicPlayer;
};

#endif