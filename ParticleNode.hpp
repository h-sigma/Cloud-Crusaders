#ifndef PARTICLENODE_HPP
#define PARTICLENODE_HPP

#include "Scene.hpp"
#include "Particle.hpp"
#include "ResourceIdentifier.hpp"

#include <deque>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/Texture.hpp>

namespace sf
{
    class RenderTarget;
    class RenderStates;
    class Color;
}

class CommandQueue;

class ParticleNode : public SceneNode
{
    public:
        explicit ParticleNode(Particle::Type type, const TextureHolder& textures);
        void addParticle(sf::Vector2f position);
        Particle::Type getParticleType() const;
        virtual unsigned int getCategory() const;
    private:
        void computeVertices() const;
        void addVertex(float worldX, float worldY, float textX, float textY, sf::Color color) const;
        virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
        virtual void updateCurrent(sf::Time dt, CommandQueue& commands);
    public:
        Particle::Type mType;
        std::deque<Particle> mParticles;
        const sf::Texture& mTexture;

        mutable sf::VertexArray mVertexArray;
        mutable bool mNeedsVertexUpdate;
};

#endif