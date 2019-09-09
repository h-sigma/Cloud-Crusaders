#include "Player.hpp"
#include "DataTables.hpp"
#include "Utility.hpp"
#include <functional>
#include <algorithm>

using namespace std::placeholders;

namespace {
    std::vector<CloudData> Table = initializeCloudData();
}
//Player
struct CloudMover{ //remove this and use a lambda
    CloudMover(float vx, float vy) : velocity(vx, vy) {

    }
    void operator()(Cloud& ourCloud, sf::Time) const{
        ourCloud.setVelocity( ourCloud.getMaxSpeed() * velocity );
    }
    sf::Vector2f velocity;
};

Player::Player(sf::RenderWindow& window) : mWindow(window)
{
    mKeyBinding[sf::Keyboard::Up] = Action::moveUp;
    mKeyBinding[sf::Keyboard::Left] = Action::moveLeft ;
    mKeyBinding[sf::Keyboard::Down] = Action::moveDown ;
    mKeyBinding[sf::Keyboard::Right] = Action::moveRight ;
    mKeyBinding[sf::Keyboard::Space] = Action::Fire;
    mKeyBinding[sf::Keyboard::M] = Action::LaunchMissile;
 
    initializeActions();
    for(auto& pair : mActionBinding)
        pair.second.category = Category::Type::Player;
}

void Player::assignKey(Action action, sf::Keyboard::Key key)
{
    mKeyBinding.erase(key);
    mKeyBinding[key] = action;
}   

sf::Keyboard::Key Player::getAssignedKey(Action action)
{
    for(auto pair: mKeyBinding)
    {
        if(pair.second == action)
        return pair.first;
    }
}


void Player::handleEvent(const sf::Event& event, CommandQueue& commands)
{
    if(event.type == sf::Event::KeyPressed)
    {
        auto found = mKeyBinding.find(event.key.code);
        if( found != mKeyBinding.end() && !isRealTimeAction(found->second))
        {
            commands.push(mActionBinding[found->second]);
        }
    }

}

void Player::handleRealTimeInput( CommandQueue& commands)
{
    for(auto pair : mKeyBinding)
    {
        if(sf::Keyboard::isKeyPressed(pair.first) && isRealTimeAction(pair.second) )
        {
            commands.push( mActionBinding[pair.second] );
        }
    }

}
#include <iostream>

void Player::initializeActions()
{
    mActionBinding[Action::moveUp].action = derivedAction<Cloud>(CloudMover(0.f, -1.f));;
    mActionBinding[Action::moveLeft].action = derivedAction<Cloud>(CloudMover(-1.f , 0.f));
    mActionBinding[Action::moveDown].action = derivedAction<Cloud>(CloudMover(0.f , 1.f));
    mActionBinding[Action::moveRight].action = derivedAction<Cloud>(CloudMover(1.f , 0.f));
    mActionBinding[Action::Fire].action = derivedAction<Cloud>( [] (Cloud& a, sf::Time){  a.fire(); });
    mActionBinding[Action::LaunchMissile].action =  derivedAction<Cloud>([] (Cloud& a, sf::Time){ a.launchMissile(); });
}

bool Player::isRealTimeAction(Player::Action action) const
{
    switch(action)
    {
        case Action::moveDown:
        case Action::moveUp:
        case Action::moveRight:
        case Action::moveLeft:
        case Action::Fire:
            return true;
        case Action::LaunchMissile:
            return false;
        default:
            return false;
    }
}