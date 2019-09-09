#include "Application.hpp"

const sf::Time Application::TimePerFrame = sf::seconds(1.f / 60.f);

Application::Application()
    : mWindow(sf::VideoMode(640, 480), "Test", sf::Style::Close), mPlayer(mWindow), mContext(mWindow, mTextureHolder, mFontHolder, mPlayer, mMusicPlayer, mSoundPlayer), mStateStack(mContext)
{
    mStateStack.registerState<TitleState>(States::Title);
    mStateStack.registerState<MenuState>(States::Menu);
    mStateStack.registerState<GameState>(States::Game);
    mStateStack.registerState<PauseState>(States::Pause);
    mStateStack.registerState<SettingState>(States::Settings);

    mFontHolder.load(Fonts::ID::FreeFont, "assets/Sansation_Regular.ttf");
    mFontHolder.load(Fonts::ID::ButtonNormal, "assets/Sansation_Regular.ttf");
    mFontHolder.load(Fonts::ID::Label, "assets/Sansation_Regular.ttf");
    mFontHolder.load(Fonts::ID::Main, "assets/Sansation_Regular.ttf");

    mTextureHolder.load(Textures::ID::Buttons, "assets/Buttons.png");
    mTextureHolder.load(Textures::ID::None, "assets/None.png");

    mStateStack.pushState(States::Title);
}

void Application::run()
{
    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;

    while (mWindow.isOpen())
    {
        sf::Time dt = clock.restart();
        timeSinceLastUpdate += dt;
        while (timeSinceLastUpdate > TimePerFrame)
        {
            timeSinceLastUpdate -= TimePerFrame;

            processInput();
            update(TimePerFrame);

            // Check inside this loop, because stack might be empty before update() call
            if (mStateStack.isEmpty())
                mWindow.close();
        }

        render();
    }
}

void Application::processInput()
{
    sf::Event event;
    while (mWindow.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            mWindow.close();
        mStateStack.handleEvent(event);
    }
}

void Application::update(sf::Time dt)
{
    mStateStack.update(dt);
}

void Application::render()
{
    mWindow.clear();

    mStateStack.draw();

    mWindow.setView(mWindow.getDefaultView());

    mWindow.display();
}