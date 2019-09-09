#ifndef SCENE_HPP
#define SCENE_HPP

#include "Category.hpp"
#include "Command.hpp"

class CommandQueue;

#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Time.hpp>

#include <functional>
#include <vector>
#include <memory>
#include <cassert>
#include <set>

class SceneNode : public sf::Transformable , public sf::Drawable , private sf::NonCopyable
{
    public:
        using Ptr = std::unique_ptr<SceneNode>;
        using Pair = std::pair<SceneNode*, SceneNode*>;
    public:
        SceneNode(Category::Type category = Category::Type::None);
        void attachChild(Ptr child);
        Ptr detachChild(const SceneNode& node);
        
        void update(sf::Time dt, CommandQueue& commands);
        void onCommand(const Command& cmd, sf::Time dt);

        virtual unsigned int getCategory() const;
        sf::Transform getWorldTransform() const;
        sf::Vector2f getWorldPosition() const;
        virtual sf::FloatRect getBoundingRect() const;

        void findCollidingPairs(std::vector<SceneNode*>::iterator it, std::vector<SceneNode*>& sceneMembers, std::vector<Pair>& collisionPairs );
        void addToSceneCollision(std::vector<SceneNode*>& sceneMembers);
        void removeWrecks();

        virtual bool isDestroyed() const;
        virtual bool isMarkedForRemoval() const;
    private:
        void draw(sf::RenderTarget& target, sf::RenderStates states) const;
        virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;

        virtual void updateCurrent(sf::Time dt, CommandQueue& commands);
        void updateChildren(sf::Time dt, CommandQueue& commands);
    private:
        std::vector<Ptr> mChildren;
        SceneNode* mParent;
    protected:
        Category::Type mDefaultCategory;
};

float distance(const SceneNode& lhs, const SceneNode& rhs);
bool collision(const SceneNode& lhs, const SceneNode& rhs);
bool matchesCategories(SceneNode::Pair& colliders, Category::Type type1, Category::Type type2);

#endif