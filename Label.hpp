#ifndef LABEL_HPP
#define LABEL_HPP

#include "Component.hpp"
#include "ResourceHolder.hpp"
#include "ResourceIdentifier.hpp"

#include <SFML/Graphics/Text.hpp>
#include <string>

namespace sf{
    class RenderTarget;
    class RenderStates;
};

namespace GUI{

class Label : public Component 
{
    public:
        typedef std::shared_ptr<Label> Ptr;
    public:
        Label(const std::string& text, const FontHolder& fonts);
        virtual bool isSelectable() const override;
        virtual void handleEvent(const sf::Event& event) override;
        void setText(std::string text);
    private:
        virtual void draw(sf::RenderTarget& renderTarget,sf::RenderStates states) const override;
    private:
        sf::Text mText;
};

}

#endif