#include "GameState.hpp"

GameState::GameState(StateStack& StateStack, Context context) : State(StateStack,context), mWorld(context) , mPlayer(*(context.mPlayer))
{
    context.mMusicPlayer->play(Music::MissionTheme);
}


bool GameState::handleEvent(const sf::Event& event)
{
    if(event.type == sf::Event::KeyPressed)
    {
        if(event.key.code == sf::Keyboard::Escape)
        {
            requestStackPush(States::Pause);
            return false;
        }
    }

    CommandQueue& commands = mWorld.getCommandQueue();
    mPlayer.handleEvent(event, commands);
    mPlayer.handleRealTimeInput(commands);
    return false;
}
void GameState::draw()
{   
    mWorld.draw();
}
bool GameState::update(sf::Time dt)
{
    mWorld.update(dt);
    return false;
}
