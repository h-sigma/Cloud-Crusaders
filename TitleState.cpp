#include "TitleState.hpp"
#include "Utility.hpp"

TitleState::TitleState(StateStack& StateStack, Context context) : State(StateStack,context) , mText(), mShowText(true),mTextEffectTime(sf::Time::Zero)
{
    mText.setFont(context.mFontHolder->get(Fonts::ID::FreeFont));
    mText.setString( "Press Any Key." );
    centerOrigin(mText);
    
    mText.setPosition((context.mRenderWindow->getView().getSize() / 2.f ) );

    context.mTextureHolder->load(Textures::ID::TitleScreen, "assets/Title.png");
    const sf::Texture& temp = context.mTextureHolder->get(Textures::ID::TitleScreen);
    mSprite.setTexture(temp);
}

bool TitleState::handleEvent(const sf::Event& event)
{
    if(event.type == sf::Event::KeyPressed)
    {
        requestStackPop();
        requestStackPush(States::Menu);
    }
    return true;
}

bool TitleState::update(sf::Time dt)
{
    mTextEffectTime += dt;
    if(mTextEffectTime >= sf::seconds(0.5f))
    {
        mShowText = !mShowText;
        mTextEffectTime = sf::Time::Zero;
    }
    return true;
}

void TitleState::draw()
{
    sf::RenderWindow& window = *(getContext().mRenderWindow);
    window.draw(mSprite);
    if(mShowText)
    window.draw(mText);
}
