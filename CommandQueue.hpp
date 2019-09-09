#ifndef COMMANDQUEUE_HPP
#define COMMANDQUEUE_HPP

#include <functional>
#include <queue>
#include <cassert>

#include "Scene.hpp"
#include "Command.hpp"

class CommandQueue
{
    public:
        void push(const Command& cmd);
        Command pop();
        bool isEmpty() const;
    private:
        std::queue<Command> mQueue;
};

template<typename GameObject, typename Function> std::function<void(SceneNode&,sf::Time)> derivedAction(Function fn)
{
    return [=](SceneNode&  node, sf::Time dt) {
        //check if cast is safe
        assert(dynamic_cast<GameObject*>(&node) != nullptr);

        //downcast node and invoke function on it 
        fn(static_cast<GameObject&> (node), dt);
    };
}

#endif