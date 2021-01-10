#include <memory>
#include <vector>

#include "StateMachine.h"

class GameState
{
    private:
        std::vector<std::unique_ptr<StateMachine>> _states;
    public:
        ~GameState()
        {
            while (!_states.empty())
            {
                pop();
            }
        }

        std::string identity()
        {
            if (_states.back())
                return _states.back()->identity();
        }

        void push(GE::Init* init, std::unique_ptr<StateMachine> state)
        {
            state->init(init);
            _states.push_back(std::move(state));
        }

        void pop()
        {
            if (!_states.empty())
            {
                _states.back()->clean();
                _states.pop_back();
                return;
            }
        }

        void update()
        {
            if (!_states.empty())
            {
                _states.back()->update();
            }
        }

        bool isEmpty()
        {
            return (_states.empty());
        }
};