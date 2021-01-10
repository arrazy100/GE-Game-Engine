#include <Input.h>

GE::Input::Input()
{
    memset(_last_state, 0, _keyboard_size);
    memcpy(_keyboard_state, SDL_GetKeyboardState(NULL), _keyboard_size);
}

GE::Input::~Input()
{
}

void GE::Input::updateKeyboard()
{
    SDL_PumpEvents();
    memcpy(_last_state, _keyboard_state, _keyboard_size);
    memcpy(_keyboard_state, SDL_GetKeyboardState(NULL), _keyboard_size);
}

bool GE::Input::getKeyboardPressed(const std::string key)
{
    SDL_Scancode key_press = SDL_GetScancodeFromName(key.c_str());

    return (_keyboard_state[key_press] && _last_state[key_press]);
}

bool GE::Input::getKeyboardReleased(const std::string key)
{
    SDL_Scancode key_release = SDL_GetScancodeFromName(key.c_str());

    return (!_keyboard_state[key_release] && _last_state[key_release]);
}

bool GE::Input::getLeftMouseClicked(int* mouse_x, int* mouse_y)
{
    return (SDL_GetMouseState(mouse_x, mouse_y) & SDL_BUTTON(SDL_BUTTON_LEFT));
}

bool GE::Input::getRightMouseClicked(int* mouse_x, int* mouse_y)
{
    return (SDL_GetMouseState(mouse_x, mouse_y) & SDL_BUTTON(SDL_BUTTON_RIGHT));
}

bool GE::Input::getMiddleMouseClicked(int* mouse_x, int* mouse_y)
{
    return (SDL_GetMouseState(mouse_x, mouse_y) & SDL_BUTTON(SDL_BUTTON_MIDDLE));
}