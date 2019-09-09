#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "Component.hpp"
#include "ResourceHolder.hpp"
#include "ResourceIdentifier.hpp"
#include "State.hpp"

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include <vector>
#include <memory>
#include <string>
#include <functional>

namespace sf{
    class Event;
}

namespace GUI
{

class Button : public Component 
{
    public:
        typedef std::shared_ptr<Button> Ptr;
        typedef std::function<void()> Callback;
    public:
        Button(State::Context context);

        sf::Vector2f getBoundingSize() const;

        virtual bool isSelectable() const override;
        virtual void select() override;
        virtual void deselect() override;
        virtual void activate() override;
        virtual void deactivate() override;
        virtual void handleEvent(const sf::Event& event);
        
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    public:
        void setText(const std::string& text);
        void setToggle(bool toggle);
        void setCallback(Callback callback);
    private:
        Callback mCallback;
        sf::Sprite mSprite;
        sf::Text mText;
        sf::Sprite mNormalTexture;
        sf::Sprite mSelectedTexture;
        sf::Sprite mPressedTexture;
        bool mIsToggle;
        sf::Vector2f spriteSize;
        SoundPlayer& mSoundPlayer;
};

}

#endif