#include "Input.h"

GE::Input::Input()
{
}

GE::Input::~Input()
{
    _keyboard_state = NULL;
}

void GE::Input::updateKeyboard()
{
    SDL_PumpEvents();
    _keyboard_state = SDL_GetKeyboardState(NULL);
}

bool GE::Input::getKeyboardPressed(std::string key)
{
    SDL_Scancode key_press = SDL_GetScancodeFromName(key.c_str());
    if (_keyboard_state[key_press]) {
        _last_key = key;
        return true;
    }
    
    return false;
}

bool GE::Input::getKeyboardReleased(std::string key)
{
    SDL_Scancode key_release = SDL_GetScancodeFromName(key.c_str());
    if (!_keyboard_state[key_release] && _last_key == key)
    {
        return true;
    }

    return false;
}