#include "Scene.hpp"
#include "CommandQueue.hpp"
#include "Utility.hpp"

#include <algorithm>

//SceneNode
SceneNode::SceneNode(Category::Type category) 
: mChildren()
, mParent(nullptr)
, mDefaultCategory(category)
{
}

void SceneNode::attachChild(Ptr child)
{
    child->mParent = this;
    mChildren.push_back(std::move(child));
}

SceneNode::Ptr SceneNode::detachChild(const SceneNode& node)
{
    auto found = std::find_if( mChildren.begin() , mChildren.end() , [&node](const Ptr& p)->bool {  return p.get() == &node;    });
    assert(found != mChildren.end());
    Ptr result = std::move(*found);
    (*result).mParent = nullptr;
    mChildren.erase(found);
    return result;
}

unsigned int SceneNode::getCategory() const
{
    return mDefaultCategory;
}

sf::FloatRect SceneNode::getBoundingRect() const
{
    return sf::FloatRect{};
}

sf::Transform SceneNode::getWorldTransform() const
{
    sf::Transform transform = sf::Transform::Identity;

    for(const SceneNode* node = this ; node != nullptr ; node = node->mParent )
    {
        transform *= node->getTransform();
    }

    return transform;
}

sf::Vector2f SceneNode::getWorldPosition() const
{
    return getWorldTransform() * sf::Vector2f();
}

void SceneNode::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    drawCurrent(target, states);

    for(const Ptr& child : mChildren)
    {
        child->draw(target, states);
    }
}

void SceneNode::onCommand(const Command& cmd, sf::Time dt)
{
    if(cmd.category == getCategory())
        cmd.action(*this, dt);

    for(Ptr& child : mChildren)
        child->onCommand(cmd,dt);
}

void SceneNode::drawCurrent(sf::RenderTarget&, sf::RenderStates) const
{
    //by default draw nothing
}

void SceneNode::update(sf::Time dt, CommandQueue& commands)
{
    updateCurrent(dt, commands);
    updateChildren(dt, commands);
}

void SceneNode::updateChildren(sf::Time dt, CommandQueue& commands)
{
    for(const auto& child : mChildren)
    {
        child->update(dt, commands);
    }
}

void SceneNode::updateCurrent(sf::Time, CommandQueue&)
{
    //by default do nothing
}

bool SceneNode::isDestroyed() const
{
    return false; //normal scenenodes aren't deleted
}

bool SceneNode::isMarkedForRemoval() const
{
    return isDestroyed();
}


void SceneNode::findCollidingPairs(std::vector<SceneNode*>::iterator it, std::vector<SceneNode*>& sceneMembers, std::vector<Pair>& collisionPairs )
{
    for(auto from = it; from < sceneMembers.end(); from++ )
    {
        for(auto compareto = from; compareto < sceneMembers.end(); compareto++)
        {
            SceneNode* & first = *from; 
            SceneNode* & second = *compareto;
            if(first != second && !first->isDestroyed() && !second->isDestroyed() && collision(*first, *second))
            {
                collisionPairs.push_back(std::minmax(first, second));
            }
        }
    }
}

void SceneNode::addToSceneCollision(std::vector<SceneNode*>& sceneMembers)
{
    sceneMembers.push_back(this);
    for(auto& child : mChildren)
        child->addToSceneCollision(sceneMembers);
}

void SceneNode::removeWrecks()
{
    mChildren.erase( std::remove_if(mChildren.begin(), mChildren.end(), std::mem_fn(&SceneNode::isMarkedForRemoval) ), mChildren.end());

    std::for_each(mChildren.begin(), mChildren.end(), std::mem_fn(&SceneNode::removeWrecks));
}

float distance(const SceneNode& lhs, const SceneNode& rhs)
{
	return magnitude(lhs.getWorldPosition() - rhs.getWorldPosition());
}


// void print(sf::FloatRect rect){
//     std::cout << "top: " << rect.top << " , left: " << rect.left << " , width: " << rect.width << " , height: " << rect.height << std::endl;
// }
bool collision(const SceneNode& lhs, const SceneNode& rhs)
{
//     std::cout << categoryToString(lhs.getCategory()) ;
//     std::cout << categoryToString(rhs.getCategory()) << std::endl;
//     print(lhs.getBoundingRect());
//     print(rhs.getBoundingRect());
    return lhs.getBoundingRect().intersects(rhs.getBoundingRect());
}

bool matchesCategories(SceneNode::Pair& colliders, Category::Type type1, Category::Type type2)
{
    unsigned int category1 = colliders.first->getCategory();
    unsigned int category2 = colliders.second->getCategory();
    
    if(type1 & category1 && type2 & category2)
    {
        return true;
    }
    else if(type1 & category2 && type2 & category1)
    {
        std::swap(colliders.first, colliders.second);
        return true;
    }
    else
    {
        return false;
    }
}