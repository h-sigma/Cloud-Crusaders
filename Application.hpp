#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <SFML/Graphics.hpp>

#include <cassert>
#include <map>
#include "GameState.hpp"
#include "PauseState.hpp"
#include "TitleState.hpp"
#include "MenuState.hpp"
#include "SettingState.hpp"
#include "ResourceHolder.hpp"
#include "ResourceIdentifier.hpp"
#include "MusicPlayer.hpp"
#include "Player.hpp"
#include "SoundPlayer.hpp"

class Application{
    public:
        Application();
        void run();
        void processInput();
        void update(sf::Time dt);
        void render();
    private:
        sf::RenderWindow mWindow;
        Player mPlayer;
        State::Context mContext;
        TextureHolder mTextureHolder;
        FontHolder mFontHolder;
        MusicPlayer mMusicPlayer;
        SoundPlayer mSoundPlayer;

        StateStack mStateStack;

        static const sf::Time TimePerFrame;
};



#endif