#ifndef MULTIPLAYER_GAMESTATE_HPP
#define MULTIPLAYER_GAMESTATE_HPP

#include "State.hpp"
#include "Player.hpp"
#include "World.hpp"
#include "GameServer.hpp"

#include <SFML/System/Clock.hpp>
#include <SFML/Network/IpAddress.hpp>
#include <SFML/Network/Packet.hpp>
#include <SFML/Network/TcpSocket.hpp>


class MultiplayerGameState : public State
{
    public:
        MultiplayerGameState(StateStack& stack, State::Context context, bool isHost);

    private:
        using PlayerPtr = std::unique_ptr<Player>;
    private:
        World mWorld;
        sf::RenderWindow& mWindow;
        TextureHolder& mTextureHolder;
        std::map<int, PlayerPtr> mPlayers;
        std::vector<sf::Int32> mLocalPlayerIdentifiers;
        sf::TcpSocket mSocket;
        bool mConnected;
        std::unique_ptr<GameServer> mGameServer;
        sf::Clock mTickClock;
        std::vector<std::string>    mBroadcasts;
        sf::Text    mBroadcastText;
        sf::Time    mBroadcastElapsedTime;
};

#endif