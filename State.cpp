#include "State.hpp"
#include "StateStack.hpp"

State::Context::Context(sf::RenderWindow& renderWindow, TextureHolder& textureHolder, FontHolder& fontHolder, Player& player, MusicPlayer& musicPlayer, SoundPlayer& soundPlayer)
    : mRenderWindow(&renderWindow) 
    , mTextureHolder(&textureHolder) 
    , mFontHolder(&fontHolder) 
    , mPlayer(&player)
    , mMusicPlayer(&musicPlayer)
    , mSoundPlayer(&soundPlayer)
{
}

//State
State::State(StateStack& stateStack, Context context) : mStack(&stateStack) , mContext(context)
{
}

State::Context State::getContext() const
{
    return mContext;
}

State::~State(){}

void State::requestStackPush(States stateID)
{
    mStack->pushState(stateID);
}

void State::requestStackPop()
{
    mStack->popState();
}

void State::requestStackClear()
{
    mStack->clearStates();
}