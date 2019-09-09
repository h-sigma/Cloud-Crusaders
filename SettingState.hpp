#ifndef SETTINGSTATE_HPP
#define SETTINGSTATE_HPP

#include <array>

#include "State.hpp"
#include "Button.hpp"
#include "Label.hpp"
#include "Player.hpp"
#include "Container.hpp"
#include "ResourceIdentifier.hpp"

#include <SFML/Graphics/Sprite.hpp>

class SettingState: public State
{
    public:
        SettingState(StateStack& mStateStack , Context context);
        virtual bool handleEvent(const sf::Event& event) override;
        virtual bool update(sf::Time dt) override;
        virtual void draw() override;
    private:
        void updateLabels();
        void addButtonLabel(Player::Action action, float y, std::string text, State::Context context);
    private:
        GUI::Container mGUIContainer;
        sf::Sprite mBackgroundSprite;
        std::array<GUI::Button::Ptr , Player::Action::ActionCount> mBindingButtons;
        std::array<GUI::Label::Ptr , Player::Action::ActionCount> mBindingLabels;
        
};

#endif