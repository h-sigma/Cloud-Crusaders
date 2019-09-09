#ifndef STATESTACK_HPP
#define STATESTACK_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <functional>
#include <map>
#include "StateIdentifiers.hpp"
#include "ResourceHolder.hpp"
#include "State.hpp"

class StateStack{
    public:
        enum class Action{
            Pop, Push, Clear
        };
    public:
        explicit StateStack(State::Context context);
        template<typename T> void registerState(States stateID);

        void draw();
        void update(sf::Time dt);
        void handleEvent(const sf::Event& event);

        void pushState(States stateID);
        void popState();
        void clearStates();

        bool isEmpty() const;
    private:
        State::Ptr createState(States stateID);
        void applyPendingChanges();
    private:
        struct PendingChange{
            StateStack::Action action;
            States stateID;
        };
    private:
        State::Context mContext;
        std::vector<State::Ptr> mStack;
        std::vector<PendingChange> mPendingList;
        std::map<States, std::function<State::Ptr()> > mFactories; 
};

template<typename T> void StateStack::registerState(States stateID)
{
    mFactories[stateID] = [this](){
        return std::make_unique<T>(*this, mContext);
    };
}

#endif