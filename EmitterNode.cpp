#include "EmitterNode.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include "CommandQueue.hpp"

EmitterNode::EmitterNode(Particle::Type type)
: mAccumulatedTime(sf::Time::Zero)
, mType(type)
, mParticleSystem(nullptr)
{
}

void EmitterNode::emitParticles(sf::Time dt)
{
    const float emissionRate = 30.f;
    sf::Time timeInterval = sf::seconds(1.f) / emissionRate;

    mAccumulatedTime += dt;

    while(mAccumulatedTime > timeInterval)
    {
        mAccumulatedTime -= timeInterval;
        mParticleSystem->addParticle(getWorldPosition());
    }
}

void EmitterNode::updateCurrent(sf::Time dt, CommandQueue& commands) 
{
    if(mParticleSystem)
    {
        emitParticles(dt);
    }
    else
    {
        auto finder = [this](ParticleNode& container, sf::Time){
            if(container.mType == mType)
                mParticleSystem = &container;
        };

        Command finding;
        finding.category = Category::ParticleSystem;
        finding.action = derivedAction<ParticleNode> (finder);
        commands.push(finding);
    }
    
}