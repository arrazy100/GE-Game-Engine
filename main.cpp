#include <Init.h>
#include <Input.h>

#include "GameState.cpp"
#include "menu.cpp"
#include "game.cpp"

int main()
{
    std::unique_ptr<GE::Init> init(new GE::Init(640, 480));
    GameState states;
    states.push(init.get(), std::make_unique<menu>());

    std::unique_ptr<GE::Input> input(new GE::Input);

    while (init->update())
    {
        if (states.isEmpty()) break;

        input->updateKeyboard();

        if (input->getKeyboardReleased("P"))
        {
            if (states.identity() == "Menu")
                states.push(init.get(), std::make_unique<game>());
        }

        if (input->getKeyboardReleased("Q"))
        {
            if (states.identity() == "Game" || states.identity() == "Menu")
                states.pop();
        }

        states.update();
    }

    return 0;
}