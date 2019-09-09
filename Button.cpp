#include <SFML/Window/Event.hpp>
#include "Button.hpp"
#include "Utility.hpp"

GUI::Button::Button(State::Context context)
: mCallback(nullptr)
, mSprite()
, mText("" , context.mFontHolder->get(Fonts::ButtonNormal) , 16)
, mNormalTexture( context.mTextureHolder->get(Textures::Buttons) , sf::IntRect{0, 0, 200, 50} )
, mSelectedTexture( context.mTextureHolder->get(Textures::Buttons), sf::IntRect{0, 51, 200, 50} )
, mPressedTexture( context.mTextureHolder->get(Textures::Buttons), sf::IntRect{0, 101, 200, 50} )
, mIsToggle(false)
, spriteSize(mNormalTexture.getLocalBounds().width, mNormalTexture.getLocalBounds().height)
, mSoundPlayer(*context.mSoundPlayer)
{
    mSprite = mNormalTexture;
    mText.setPosition( sf::Vector2f{100.f, 25.f});
}

sf::Vector2f GUI::Button::getBoundingSize() const
{
    return spriteSize;
}

bool GUI::Button::isSelectable() const 
{
    return true;
}

void GUI::Button::select() 
{
    Component::select();
    mSprite = mSelectedTexture;
}

void GUI::Button::deselect() 
{
    Component::deselect();
    mSprite = mNormalTexture;
}

void GUI::Button::activate() 
{
    Component::activate();
    if(mIsToggle)
        mSprite = mPressedTexture;
    if(mCallback)
        mCallback();
    if(!mIsToggle)
        deactivate();
    mSoundPlayer.play(SoundEffect::Button);
}

void GUI::Button::deactivate() 
{
    Component::deactivate();
    if(mIsToggle)
    {
        if(isSelected())
            mSprite = mSelectedTexture ;
        else
            mSprite = mNormalTexture;
    }
}

void GUI::Button::handleEvent(const sf::Event&)
{
}

void GUI::Button::draw(sf::RenderTarget& target, sf::RenderStates states) const 
{
    states.transform *= getTransform();
    target.draw(mSprite , states);
    target.draw(mText , states);
}

void GUI::Button::setText(const std::string& text) 
{
    mText.setString(text);
    centerOrigin(mText);
}

void GUI::Button::setToggle(bool flag)
{
    mIsToggle = flag;
}

void GUI::Button::setCallback(GUI::Button::Callback callback)
{
    mCallback = std::move(callback);
}