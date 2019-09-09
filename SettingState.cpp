#include "SettingState.hpp"
#include <cstdint>
#include <functional>
#include "Utility.hpp"

void SettingState::addButtonLabel(Player::Action action, float y, std::string text, State::Context context)
{
    mBindingButtons[action] = std::make_shared<GUI::Button> ( context );
    mBindingButtons[action]->setText(text);
    mBindingButtons[action]->setPosition(100.f , y);
    mBindingButtons[action]->setToggle(true);

    mGUIContainer.pack(mBindingButtons[action]);

    mBindingLabels[action] = std::make_shared<GUI::Label> ( "" , *context.mFontHolder );
    mBindingLabels[action]->setPosition(100.f + mBindingButtons[action]->getBoundingSize().x  , y + 15.f);

    mGUIContainer.pack(mBindingLabels[action]);
}

SettingState::SettingState(StateStack& mStateStack , Context context)
: State(mStateStack,context)
, mGUIContainer()
{
    mBackgroundSprite.setTexture(context.mTextureHolder->get(Textures::TitleScreen));
    
    addButtonLabel(Player::Action::moveDown , 150.f , "Move Down" , context);
    addButtonLabel(Player::Action::moveLeft , 200.f , "Move Left" , context);
    addButtonLabel(Player::Action::moveRight , 250.f , "Move Right" , context);
    addButtonLabel(Player::Action::moveUp , 300.f , "Move Up" , context);
    addButtonLabel(Player::Action::Fire, 350.f, "Fire", context );
    addButtonLabel(Player::Action::LaunchMissile, 400.f, "Launch Missile" , context );

    updateLabels();

    auto lBackButton = std::make_shared<GUI::Button>( context );
    lBackButton->setText("Go Back.");
    lBackButton->setPosition(250.f , 50.f);
    lBackButton->setCallback( std::bind( &SettingState::requestStackPop , this) );
    
    mGUIContainer.pack(lBackButton);
}

bool SettingState::handleEvent(const sf::Event& event)
{
    bool isKeyBinding = false;
    for(std::size_t action = 0 ; action < Player::Action::ActionCount ; action++)
    {
        if(mBindingButtons[action]->isActive())
        {
            isKeyBinding = true;
            if(event.type == sf::Event::KeyReleased )
            {
                getContext().mPlayer->assignKey(static_cast<Player::Action>(action) , event.key.code);
                mBindingButtons[action]->deactivate();
            }
            break;
        }
    }

    if(isKeyBinding)
        updateLabels();
    else
    {
            mGUIContainer.handleEvent(event);
    }
    return false;
}

bool SettingState::update(sf::Time)
{
}

void SettingState::draw()
{
    sf::RenderTarget & lTarget = *( getContext().mRenderWindow ); 

    sf::RectangleShape darken_back;
    darken_back.setFillColor( sf::Color{0 , 0 , 0 , 255} );
    darken_back.setSize( static_cast<sf::Vector2f> ( lTarget.getSize() ) );

    lTarget.draw(darken_back);
    lTarget.draw(mBackgroundSprite);
    lTarget.draw(mGUIContainer);
}

void SettingState::updateLabels()
{
    Player& lPlayer = *getContext().mPlayer;

    for(std::size_t i = 0 ; i < Player::ActionCount ; i++)
    {
        sf::Keyboard::Key key = lPlayer.getAssignedKey( static_cast<Player::Action>(i));
        mBindingLabels[i]->setText( toString(key) );
    }
}