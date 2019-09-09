#ifndef STATE_HPP
#define STATE_HPP

#include "ResourceHolder.hpp"
#include "ResourceIdentifier.hpp"
#include "StateIdentifiers.hpp"
#include "MusicPlayer.hpp"
#include "SoundPlayer.hpp"

class Player;
class StateStack;

namespace sf{
    class RenderWindow;
    class Event;
    class Time;
}

class State{
    public:
        typedef std::unique_ptr<State> Ptr;
        struct Context{
                Context(sf::RenderWindow& renderWindow, TextureHolder& textureHolder, FontHolder& fontHolder, Player& player, MusicPlayer& musicPlayer, SoundPlayer& soundPlayer);
            sf::RenderWindow* mRenderWindow;
            TextureHolder* mTextureHolder;
            FontHolder* mFontHolder;
            Player* mPlayer;
            MusicPlayer* mMusicPlayer;
            SoundPlayer* mSoundPlayer;
        };
    public:
            State(StateStack& stateStack, Context context);
        virtual ~State();

        virtual bool handleEvent(const sf::Event& event) = 0;
        virtual bool update(sf::Time dt) = 0;
        virtual void draw() = 0;
    protected:
        Context getContext() const;
        
        void requestStackPush(States stateID);
        void requestStackPop();
        void requestStackClear();

    private:
        StateStack* mStack;
        Context mContext;
};

#endif