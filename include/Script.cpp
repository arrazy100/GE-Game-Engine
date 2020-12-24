#include "Script.h"

/**
 * @brief
 * @param init passing GE::Init object to get current renderer
 */
GE::Script::Script(GE::Init *init) : _init(init)
{
}

void GE::Script::loadScript(std::string filename)
{
    std::ifstream file(filename);
    std::string s;
    while (std::getline(file, s))
    {
        _script.push_back(s);
        _length++;
    }
    file.close();
}

void GE::Script::setBackground(GE::Sprite *background)
{
    _background = background;
}

void GE::Script::setCharacter(GE::Sprite *character, int index)
{
    switch (index)
    {
    case 1:
        _character1 = character;
        break;
    case 2:
        _character2 = character;
        break;
    case 3:
        _character3 = character;
        break;
    case 4:
        _character4 = character;
        break;
    default:
        std::cout << "Karakter maksimal hanya ada 4" << std::endl;
        break;
    }
}

void GE::Script::setDialogBox(GE::Shape *dialog_box)
{
    _dialog_box = dialog_box;
}

void GE::Script::setNameObject(GE::Text *name_text)
{
    _name_text = name_text;
}

void GE::Script::setDialogText(GE::Text *dialog_text)
{
    _dialog_text = dialog_text;
}

std::string GE::Script::currentScript()
{
    return _script.at(_current_script);
}

void GE::Script::parseScript()
{
    if (currentScript() == "")
        _dialog_text->changeText(currentScript(), false);
    if (currentScript().at(0) == '[' && currentScript().at(currentScript().length() - 1) == ']')
    {
        _bg_change = true;
        if (currentScript().substr(1, currentScript().length() - 2) == "end")
        {
            _character1->fadeOut(255.0);
            _background->loadImage("bg/black.png");
            _name_text->changeText("", false);
            _dialog_text->changeText("", false);
        }
        else
        {
            _background->loadImage("bg/" + currentScript().substr(1, currentScript().length() - 2));
        }
        _bg_change = false;
    }
    else if (currentScript().find('=') != std::string::npos)
    {
        std::string temp = currentScript();
        temp.erase(std::remove_if(temp.begin(), temp.end(), isspace), temp.end());
        _temp = split(temp, '=');
        if (_temp.at(0) == "tokoh")
        {
            if (_temp.at(1) == "null")
            {
                _name_text->changeText("", false);
            }
            else
                _name_text->changeText(_temp.at(1), false);
        }
        else if (_temp.at(0) == "fade_in")
        {
            _character1->fadeIn(60.0);
            _character1->loadImage("res/" + _temp.at(1));
        }
        else if (_temp.at(0) == "fade_out")
        {
            _character1->fadeOut(60.0);
        }
        else
            _dialog_text->changeText(currentScript(), true);
    }
    else
    {
        _dialog_text->changeText(currentScript(), true);
    }
}

void GE::Script::next()
{
    if (_current_script < _length)
    {
        parseScript();
        _current_script++;
    }
}

void GE::Script::previous()
{
    if (_current_script > _length)
    {
        _current_script--;
    }
}

int GE::Script::getLength()
{
    return _length;
}

bool GE::Script::bgChange()
{
    return _bg_change;
}

bool GE::Script::changeDialog()
{
    return _dialog_text->change();
}

std::vector<std::string> GE::Script::split(const std::string &s, char delimiter)
{
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter))
    {
        tokens.push_back(token);
    }
    return tokens;
}