#include "Animation.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>


Animation::Animation()
: mSprite()
, mFrameSize()
, mCurrentFrame(0u)
, mNumFrames(0u)
, mDuration(sf::Time::Zero)
, mElapsed(sf::Time::Zero)
, mRepeat(false)
{

}

Animation::Animation(const sf::Texture& texture)
: mSprite(texture)
, mFrameSize()
, mCurrentFrame(0u)
, mNumFrames(0u)
, mDuration(sf::Time::Zero)
, mElapsed(sf::Time::Zero)
, mRepeat(false)
{

}


void Animation::setTexture(const sf::Texture& texture)
{
    mSprite.setTexture(texture);
}

void Animation::update(sf::Time dt)
{
    sf::Time timePerFrame = mDuration / static_cast<float>(mNumFrames);
    mElapsed += dt;

    sf::Vector2i textureBounds (mSprite.getTexture()->getSize());
    sf::IntRect textureRect = mSprite.getTextureRect();

    if(mCurrentFrame == 0)
        textureRect = sf::IntRect(0, 0, mFrameSize.x , mFrameSize.y);

    //enough time has elapsed to change to next frame
    while((mElapsed <= timePerFrame) && (mCurrentFrame <= mNumFrames || mRepeat))
    {
        textureRect.left += mFrameSize.x;

        if(textureRect.left + textureRect.width >= textureBounds.x)
        {
            textureRect.left = 0;
            textureRect.top += mFrameSize.y;
        }

        mElapsed -= timePerFrame;
        if(mRepeat)
        {
            mCurrentFrame = (mCurrentFrame+1) % mNumFrames;
            if(mCurrentFrame == 0)
                textureRect = sf::IntRect(0, 0, mFrameSize.x , mFrameSize.y);
        }
        else
        {
            mCurrentFrame++;
        }
        mSprite.setTextureRect(textureRect);
    }   
}

void Animation::setRepeated(bool val)
{
    mRepeat = val;
}

void Animation::setFrameSize(sf::Vector2i size)
{
    mFrameSize = size;
}

void Animation::setDuration(sf::Time dt)
{
    mDuration = dt;
}

void Animation::setNumFrames(std::size_t num)
{
    mNumFrames = num;
}

void Animation::restart()
{
    mCurrentFrame = 0;
}

bool Animation::isFinished() const
{
    return mCurrentFrame >= mNumFrames;
}

const sf::Texture* Animation::getTexture() const
{
    return mSprite.getTexture();
}
bool Animation::getRepeated() const
{
    return mRepeat;
}

sf::Vector2i  Animation::getFrameSize() const
{
    return mFrameSize;
}

sf::Time Animation::getDuration() const
{
    return mDuration;
}

std::size_t Animation::getNumFrames() const
{
    return mNumFrames;
}


sf::FloatRect Animation::getLocalBounds() const
{
    return sf::FloatRect( getOrigin() , static_cast<sf::Vector2f>(getFrameSize()));
}

sf::FloatRect Animation::getGlobalBounds() const
{
    return getTransform().transformRect(getLocalBounds());
}

void Animation::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();  
    target.draw(mSprite, states);
}