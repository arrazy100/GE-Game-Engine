#ifndef SCRIPT_H
#define SCRIPT_H

#include "Init.h"
#include "Sprite.h"
#include "Shape.h"
#include "Text.h"
#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include <algorithm>
#include <sstream>

namespace GE
{
    class Script
    {
    private:
        GE::Init *_init = NULL;
        GE::Sprite *_background = NULL;
        GE::Sprite *_character1 = NULL;
        GE::Sprite *_character2 = NULL;
        GE::Sprite *_character3 = NULL;
        GE::Sprite *_character4 = NULL;
        GE::Shape *_dialog_box = NULL;
        GE::Text *_name_text = NULL;
        GE::Text *_dialog_text = NULL;
        std::vector<std::string> _script;
        std::vector<std::string> _temp;
        int _length = 0;
        int _current_script = 0;
        bool _bg_change = false;

    public:
        Script(GE::Init *init);
        void loadScript(std::string file);
        void setBackground(GE::Sprite *background);
        void setCharacter(GE::Sprite *character, int index);
        void setDialogBox(GE::Shape *dialog_box);
        void setNameObject(GE::Text *name_text);
        void setDialogText(GE::Text *dialog_text);
        std::string currentScript();
        void parseScript();
        void next();
        void previous();
        bool changeDialog();
        bool bgChange();
        int getLength();
        std::vector<std::string> split(const std::string &s, char delimiter);
    };
} // namespace GE

#endif