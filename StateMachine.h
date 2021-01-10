#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include <Init.h>

class StateMachine
{
    public:
        virtual std::string identity() = 0;
        virtual void init(GE::Init* init) = 0;
        virtual void clean() = 0;
        virtual void update() = 0;
};

#endif