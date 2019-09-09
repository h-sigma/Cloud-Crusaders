#include "CommandQueue.hpp"


//CommandQueue
void CommandQueue::push(const Command& cmd)
{
    mQueue.push(cmd);
}

Command CommandQueue::pop()
{
    Command temp = mQueue.front();
    mQueue.pop();
    return temp;
}

bool CommandQueue::isEmpty() const
{
    return mQueue.empty();
}



