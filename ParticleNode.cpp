#include "ParticleNode.hpp"
#include "DataTables.hpp"
#include "ResourceHolder.hpp"
#include "Category.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

namespace 
{
    std::vector<ParticleData> Table = initializeParticleData();
}


ParticleNode::ParticleNode(Particle::Type type, const TextureHolder& textures)
: mType(type)
, mTexture( textures.get(Textures::ID::Particle))
, mVertexArray(sf::Quads)
, mNeedsVertexUpdate(false)
{

}

void ParticleNode::addParticle(sf::Vector2f position)   
{
    Particle temParticle;
    temParticle.color = Table[mType].color;
    temParticle.lifetime = Table[mType].lifetime;
    temParticle.position = position;
    mParticles.push_back(std::move(temParticle));
}

Particle::Type ParticleNode::getParticleType() const
{
    return mType;
}

unsigned int ParticleNode::getCategory() const
{
    return Category::Type::ParticleSystem;
}

void ParticleNode::computeVertices() const
{
    sf::Vector2f size( mTexture.getSize());
    sf::Vector2f half = size / 2.f;

    mVertexArray.clear();
    
    for(const auto& particle : mParticles)
    {
        sf::Color c = particle.color;
        sf::Vector2f pos = particle.position;
        const float ratio = particle.lifetime.asSeconds() / Table[mType].lifetime.asSeconds();
        c.a = static_cast<uint8_t> (255 * std::max(0.0f, ratio));   //can't forget to keep the alpha value positive

        addVertex(pos.x - half.x, pos.y - half.y, 0.f,    0.f,    c);
        addVertex(pos.x + half.x, pos.y - half.y, size.x, 0.f,    c);
        addVertex(pos.x + half.x, pos.y + half.y, size.x, size.y, c);
        addVertex(pos.x - half.x, pos.y + half.y, 0.f,    size.y, c);
    }
}

void ParticleNode::addVertex(float worldX, float worldY, float textX, float textY, sf::Color color) const
{
    sf::Vertex tempVertex;
    tempVertex.color = color;
    tempVertex.position = {worldX, worldY};
    tempVertex.texCoords = {textX, textY};
    mVertexArray.append(tempVertex);
}

void ParticleNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    if(mNeedsVertexUpdate)
    {
        computeVertices();
        mNeedsVertexUpdate = false;
    }

    states.texture = &mTexture;
    target.draw(mVertexArray, states);
}

void ParticleNode::updateCurrent(sf::Time dt, CommandQueue&)
{
    //remove all expired particles
    while(!mParticles.empty() && mParticles.front().lifetime <= sf::Time::Zero)
        mParticles.pop_front();

    //reduce time elapsed from particles' lifetime
    for(auto& particle : mParticles)
    {
        particle.lifetime -= dt;
    }

    mNeedsVertexUpdate = true;
}   
