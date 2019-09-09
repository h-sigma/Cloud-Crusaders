#ifndef SOUND_NODE_HPP
#define SOUND_NODE_HPP

#include "Scene.hpp"
#include "SoundPlayer.hpp"

class SoundNode : public SceneNode
{
    public:
        explicit SoundNode(SoundPlayer& player);
        void playSound(SoundEffect::ID effect, sf::Vector2f position);
        virtual unsigned int getCategory() const;
    private:
        SoundPlayer& mSounds;
};

#endif