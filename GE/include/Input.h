#ifndef INPUT_H
#define INPUT_H

#include <SDL2/SDL.h>
#include <string>

namespace GE
{
	class Input
	{
        private:
            const Uint8* _keyboard_state = NULL;
            std::string _last_key = "";
        public:
            Input();
            ~Input();
            void updateKeyboard();
            bool getKeyboardPressed(std::string key);
            bool getKeyboardReleased(std::string key);
            bool getLeftMouseClicked(int* mouse_x, int* mouse_y);
            bool getRightMouseClicked(int* mouse_x, int* mouse_y);
            bool getMiddleMouseClicked(int* mouse_x, int* mouse_y);
    };
}

#endif