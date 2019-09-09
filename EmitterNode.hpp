#ifndef EMITTERNODE_HPP
#define EMITTERNODE_HPP

#include "Scene.hpp"
#include "Particle.hpp"
#include "ParticleNode.hpp"

#include <SFML/System/Time.hpp>

namespace sf
{
    class RenderTarget;
    class RenderStates;
}

class CommandQueue;

class EmitterNode : public SceneNode
{
    public:
        explicit EmitterNode(Particle::Type type);
    private:
        void emitParticles(sf::Time dt);
        virtual void updateCurrent(sf::Time dt, CommandQueue& commands);
    private:
        sf::Time mAccumulatedTime;
        Particle::Type mType;    
        ParticleNode* mParticleSystem;
};

#endif