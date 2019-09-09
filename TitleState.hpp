#ifndef TITLESTATE_HPP
#define TITLESTATE_HPP

#include "State.hpp"
#include <SFML/Graphics.hpp>


class TitleState : public State
{
    public:
        TitleState(StateStack& StateStack, Context context);

        virtual bool handleEvent(const sf::Event& event) override;
        virtual bool update(sf::Time dt) override;
        virtual void draw() override;
    private:
        sf::Sprite mSprite;
        sf::Text mText;
        bool mShowText;
        sf::Time mTextEffectTime;
};

#endif