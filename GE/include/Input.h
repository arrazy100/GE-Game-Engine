#ifndef INPUT_H
#define INPUT_H

#include <SDL2/SDL.h>
#include <string>

namespace GE
{
	class Input
	{
        private:
            size_t _keyboard_size = sizeof(Uint8) * SDL_NUM_SCANCODES;
            Uint8 _keyboard_state[SDL_NUM_SCANCODES];
            Uint8 _last_state[SDL_NUM_SCANCODES];
        public:
            Input();
            ~Input();
            void updateKeyboard();
            bool getKeyboardPressed(const std::string key);
            bool getKeyboardReleased(const std::string key);
            bool getLeftMouseClicked(int* mouse_x, int* mouse_y);
            bool getRightMouseClicked(int* mouse_x, int* mouse_y);
            bool getMiddleMouseClicked(int* mouse_x, int* mouse_y);
    };
}

#endif