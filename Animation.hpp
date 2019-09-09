#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Time.hpp>

#include "ResourceHolder.hpp"
#include "ResourceIdentifier.hpp"

namespace sf{
    class RenderStates;
    class RenderTarget;
};

class Animation : public sf::Drawable, public sf::Transformable
{
    public:
        //constructors
        Animation();
        Animation(const sf::Texture& texture);

        void update(sf::Time dt);

        //setters
        void setTexture(const sf::Texture& texture);
        void setRepeated(bool val);
        void setFrameSize(sf::Vector2i size);
        void setDuration(sf::Time dt);
        void setNumFrames(std::size_t num);

        //operations
        void restart();

        //getters
        bool isFinished() const;
        const sf::Texture*  getTexture() const;
        bool getRepeated() const;
        sf::Vector2i  getFrameSize() const;
        sf::Time  getDuration() const;
        std::size_t getNumFrames() const;
        sf::FloatRect getLocalBounds() const;
        sf::FloatRect getGlobalBounds() const;
    private:
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    private:
        sf::Sprite mSprite;
        sf::Vector2i mFrameSize;
        std::size_t mNumFrames;
        std::size_t mCurrentFrame;
        sf::Time mDuration;
        sf::Time mElapsed;
        bool mRepeat;
};

#endif