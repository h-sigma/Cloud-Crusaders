#include "Container.hpp"
#include <cassert>
#include <limits>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

GUI::Container::Container()
: mChildren()
, mSelectedChild(-1)
{
}

bool GUI::Container::hasSelection() const
{
    assert( mChildren.size() <= std::numeric_limits<int>::max() );
    return mSelectedChild > -1 && mSelectedChild < static_cast<int>(mChildren.size()) ;
}

void GUI::Container::pack(GUI::Component::Ptr component)
{
    mChildren.push_back(component);
    if(!hasSelection() && component->isSelectable())
    {
        select(mChildren.size() - 1);
    }
}

void GUI::Container::select(int index)
{
    if(mChildren[index]->isSelectable())
    {
        if(hasSelection())
            mChildren[mSelectedChild]->deselect();
        mChildren[index]->select();
        mSelectedChild = index;
    }
}

void GUI::Container::selectNext()
{
    if(!hasSelection())
        return;
    int next = mSelectedChild;

    //find next selectable component
    do
        next = (next + 1) % mChildren.size();
    while (!mChildren[next]->isSelectable());

    select(next);
}

void GUI::Container::selectPrevious()
{
    if(!hasSelection())
        return;
    int prev = mSelectedChild;

    //find previous selectable component
    do
        prev = (prev + mChildren.size() - 1) % mChildren.size();
    while (!mChildren[prev]->isSelectable());

    select(prev);
}

void GUI::Container::handleEvent(const sf::Event& event)
{
    if(hasSelection() && mChildren[mSelectedChild]->isActive())
    {
        mChildren[mSelectedChild]->handleEvent(event);
    }
    else if(event.type == sf::Event::KeyReleased)
    {
        if(event.key.code == sf::Keyboard::Up || event.key.code == sf::Keyboard::W)
        {
            selectPrevious();
        }
        else if(event.key.code == sf::Keyboard::Down || event.key.code == sf::Keyboard::S)
        {
            selectNext();
        }
         else if(event.key.code == sf::Keyboard::Return || event.key.code == sf::Keyboard::BackSpace)
        {
            if(hasSelection())
                mChildren[mSelectedChild]->activate();
        }
    }
}

bool GUI::Container::isSelectable() const
{
    return false;
}

void GUI::Container::draw(sf::RenderTarget& target, sf::RenderStates states) const 
{
    states.transform *= getTransform();
    for(const Component::Ptr& child : mChildren)
        target.draw(*child, states);
        
}