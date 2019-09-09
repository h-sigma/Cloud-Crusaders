#ifndef MENUSTATE_HPP
#define MENUSTATE_HPP


#include "State.hpp"
#include <SFML/Graphics.hpp>


class MenuState: public State
{
    private:
        enum Options : int{
            Play , Exit
        };
    public:
        MenuState(StateStack& mStateStack, Context context);
        virtual void draw() override;
        virtual bool update(sf::Time dt) override;
        virtual bool handleEvent(const sf::Event& event) override;
    private:
        void updateOptionText();
    private:
        std::vector<sf::Text> mOptions;
        std::size_t mOptionsIndex;
        sf::Sprite mBackgroundSprite;
};

#endif