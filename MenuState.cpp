#include "MenuState.hpp"
#include "ResourceHolder.hpp"
#include "Button.hpp"
#include "Utility.hpp"

MenuState::MenuState(StateStack& mStateStack, Context context) 
: State(mStateStack,context)
, mGUIContainer()
{

    context.mMusicPlayer->play(Music::MenuTheme);
    //create play button
    auto playButton = std::make_shared<GUI::Button>(context);

    playButton->setText("Play!");
    playButton->setPosition(250.f , 200.f);
    playButton->setCallback( [this](){
        requestStackPop();
        requestStackPush(States::Game);
    });
       
    
    mGUIContainer.pack(playButton);

    //create exit button
    auto exitButton = std::make_shared<GUI::Button>(context);
    exitButton->setText("Exit...");
    exitButton->setPosition(playButton->getPosition() + sf::Vector2f(0.f , 40.f));
    exitButton->setCallback( [this](){
        requestStackPop();
    });
    mGUIContainer.pack(exitButton);

    //load background
    context.mTextureHolder->load(Textures::ID::TitleScreen, "assets/Title.png");
    const sf::Texture& temp = context.mTextureHolder->get(Textures::ID::TitleScreen);
    mBackgroundSprite.setTexture(temp);

}

void MenuState::draw()
{
    sf::RenderWindow& window = *(getContext().mRenderWindow);
    window.setView(window.getDefaultView());
    window.draw(mBackgroundSprite);
    window.draw(mGUIContainer);
}

bool MenuState::handleEvent(const sf::Event& event)
{
    mGUIContainer.handleEvent(event);
    return false;
}

bool MenuState::update(sf::Time)
{
    return true;
}
