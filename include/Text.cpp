#include "Text.h"
#include <math.h>
#include <iostream>

/**
 * @brief
 * @param init passing GE::Init object to get current renderer
 */
GE::Text::Text(SDL_Renderer* renderer, std::string text, std::string font, int size, SDL_Color c, int style, int max_length)
{
    _renderer = renderer;
    _text = text;
    _font = TTF_OpenFont(font.c_str(), size);
    _c = c;
    _max_length = max_length;
    _size = size;
    TTF_SetFontStyle(_font, style);
    _surface = TTF_RenderText_Blended_Wrapped(_font, _text.c_str(), c, max_length);
    _texture = SDL_CreateTextureFromSurface(_renderer, _surface);
    SDL_QueryTexture(_texture, NULL, NULL, 0, 0);
}

GE::Text::~Text()
{
    TTF_CloseFont(_font);
    SDL_DestroyTexture(_texture);
    SDL_FreeSurface(_surface);
    _font = NULL;
    _texture = NULL;
    _surface = NULL;
    _renderer = NULL;
}

void GE::Text::calculateSize()
{
    TTF_SizeText(_font, _text.c_str(), &_dstrect.w, &_dstrect.h);
    if (_dstrect.w >= _max_length)
        _dstrect.w = _max_length;

    int n = _text.length() * _size / 2;
    if (n >= _max_length)
    {
        _dstrect.h += 16 * ceil(n / _max_length);
    }
    //std::cout << ceil(_text.length() * 6 / 620) << std::endl;
}

void GE::Text::changeText(std::string new_text, bool scroll)
{
    SDL_DestroyTexture(_texture);
    SDL_FreeSurface(_surface);
    _text = new_text;
    if (scroll)
    {
        _surface = TTF_RenderText_Blended_Wrapped(_font, _text.substr(0, 0).c_str(), _c, _max_length);
        _temp = _text;
        _scroll_text = true;
    }
    else
    {
        _text = new_text;
        _surface = TTF_RenderText_Blended_Wrapped(_font, _text.c_str(), _c, _max_length);
    }
    _texture = SDL_CreateTextureFromSurface(_renderer, _surface);
    calculateSize();
    _change_text = true;
}

bool GE::Text::change()
{
    return _change_text;
}

void GE::Text::scrollText(bool scroll)
{
    _temp = _text;
    _scroll_text = scroll;
}

void GE::Text::changeScrollSpeed(double scroll_speed)
{
    _scroll_speed = scroll_speed;
}

void GE::Text::draw(double x, double y, double dt)
{
    if (_scroll_text)
    {
        _text = _temp;
        if (_len == _text.length() + 1)
        {
            _change_text = false;
            _scroll_text = false;
            _len = 0;
        }
        _counter += dt;
        if (_counter >= _scroll_speed && _len <= _text.length())
        {
            changeText(_text.substr(0, _len), false);
            _len += 1;
            _counter = 0.0;
        }
    }
    else if (_change_text && !_scroll_text)
    {
        _change_text = false;
    }
    _dstrect.x = x;
    _dstrect.y = y;
    SDL_RenderCopy(_renderer, _texture, NULL, &_dstrect);
}