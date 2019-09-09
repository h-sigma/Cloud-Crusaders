#ifndef TEXTNODE_HPP
#define TEXTNODE_HPP

#include "Scene.hpp"
#include "ResourceHolder.hpp"
#include "ResourceIdentifier.hpp"

#include <string>
#include <SFML/Graphics/Text.hpp>

namespace sf{
    class RenderTarget;
    class RenderStates;
}

class TextNode : public SceneNode
{
    public:
        using Ptr = std::unique_ptr<TextNode>;
    public:
        explicit TextNode(const FontHolder& fh, const std::string& text);
        void setString(const std::string& str);
        void setColor(sf::Color color);
    private:
        virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;
    private:
        sf::Text mText;
};

#endif