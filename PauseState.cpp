#include "PauseState.hpp"
#include "Button.hpp"
#include "Utility.hpp"

PauseState::PauseState(StateStack& mStateStack, Context context) 
: State(mStateStack,context)
, mMusicPlayer(*context.mMusicPlayer)
{
    mMusicPlayer.setPaused(true);

    auto mContinueBotton = std::make_shared<GUI::Button> (context);
    mContinueBotton->setText( "Continue" );
    mContinueBotton->setPosition( 250.f , 200.f );
    mContinueBotton->setCallback( [this]() {
        requestStackPop();
    });
    mGUIContainer.pack(mContinueBotton);

    auto lSettingsButtons = std::make_shared<GUI::Button> (context);
    lSettingsButtons->setText( "Settings" );
    lSettingsButtons->setPosition( mContinueBotton->getPosition().x , mContinueBotton->getPosition().y + 40.f );
    lSettingsButtons->setCallback( [this]() {
        requestStackPush(States::Settings);
    });
    mGUIContainer.pack(lSettingsButtons);

    auto mMenuButton = std::make_shared<GUI::Button> (context);
    mMenuButton->setText( "Exit to Menu" );
    mMenuButton->setPosition( mContinueBotton->getPosition().x , mContinueBotton->getPosition().y + 80.f );
    mMenuButton->setCallback( [this]() {
        requestStackClear();
        requestStackPush(States::Menu);
    });
    mGUIContainer.pack(mMenuButton);
}

PauseState::~PauseState()
{
    mMusicPlayer.setPaused(false);
}

bool PauseState::update(sf::Time) 
{
    return false; //don't let the game state update
}

bool PauseState::handleEvent(const sf::Event& event)
{
    mGUIContainer.handleEvent(event);
    return false;
}

void PauseState::draw()
{
    sf::RenderWindow& rw = *(getContext().mRenderWindow);

    rw.setView(rw.getDefaultView());

    sf::RectangleShape back;
    back.setFillColor(sf::Color(0, 0, 0, 150));
    back.setSize( static_cast<sf::Vector2f> ( rw.getSize() ) );
    
    rw.draw(back);
    rw.draw(mGUIContainer);
}

