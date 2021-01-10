#include <memory>
#include <vector>
#include <string>

#include <Input.h>
#include <Sprite.h>

#include "StateMachine.h"

class menu : public StateMachine
{
    private:
        GE::Init* _init = NULL;
        double _dt = 0;

    public:
        std::string identity() override
        {
            return "Menu";
        }

        void init(GE::Init* init) override
        {
            _init = init;
        }

        void update() override
        {
            _dt = _init->getDeltaTime();

            _init->render();

            _init->present();
        }

        void clean() override
        {
            _init = NULL;
        }
};