#ifndef CONTAINER_HPP
#define CONTAINER_HPP

#include "Component.hpp"
#include <vector>

namespace GUI{

class Container : public Component 
{   
    public:
        typedef std::shared_ptr<Container> Ptr;
    public:
        Container();
        virtual bool isSelectable() const override;
        virtual void handleEvent(const sf::Event& event) override;

        void pack(Component::Ptr component);
        bool hasSelection() const;
    private:
        virtual void draw(sf::RenderTarget&, sf::RenderStates) const override;
    protected:
        void select(int index);
        void selectNext();
        void selectPrevious();
    protected:
        std::vector<Component::Ptr> mChildren;
        int mSelectedChild;
};

}

#endif