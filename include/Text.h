#ifndef TEXT_H
#define TEXT_H

#include <string>
#include "Init.h"

namespace GE
{
    class Text
    {
    private:
        TTF_Font* _font = NULL;
        SDL_Renderer* _renderer = NULL;
        SDL_Surface* _surface = NULL;
        SDL_Texture* _texture = NULL;
        SDL_Rect _dstrect = {0, 0, 0, 0};
        SDL_Color _c;
        std::string _text = "";
        std::string _temp = "";
        int _max_length = 0;
        int _size = 0;
        int _len = 0;
        double _counter = 0.0;
        double _scroll_speed = 0.5;
        bool _change_text = true;
        bool _scroll_text = false;

    public:
        //constructor
        Text(SDL_Renderer* renderer, std::string text, std::string font, int size, SDL_Color c, int style, int max_length);
        ~Text();
        void calculateSize();
        void changeText(std::string new_text, bool scroll);
        bool change();
        void scrollText(bool scroll);
        void changeScrollSpeed(double scroll_speed);
        void draw(double x, double y, double dt);
    };
} // namespace GE

#endif