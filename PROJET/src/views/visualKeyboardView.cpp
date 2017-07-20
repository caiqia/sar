#include "visualKeyboardView.hpp"
#include "view.hpp"
#include "../controllers/visualKeyboardController.hpp"

#include <algorithm>
#include <iostream>

using namespace std;

const int KEYBOARD_ANGLE_SLOPE = 10;
const int KEY_ANGLE_SLOPE = 20;
const NostalColors KEYBOARD_BACKGROUND_COLOR = NostalColors::WHITE;
const NostalColors KEYS_COLOR = NostalColors::LIGHT_BLUE;
const NostalColors KEYS_CONTENT_COLOR = NostalColors::WHITE;
const NostalColors KEYS_HILIGHT_COLOR = NostalColors::MAIN_BLUE;


VisualKeyboardView::VisualKeyboardView(View* view)
: AbstractView(view, ViewsID::VISUAL_KEYBOARD_VIEW)
{
}

VisualKeyboardView::~VisualKeyboardView()
{

}

VisualKeyboardView* VisualKeyboardView::render(const ViewData& viewData)
{
    AbstractView::render(viewData);

    if(this->getBackground() != nullptr)
    {
        this->getView()->addToRender(this->getBackground());
    }

    if(this->getInputBar() != nullptr)
    {
        this->getView()->addToRender(this->getInputBar());
    }

    if(this->getInputTextObject() != nullptr)
    {
        if(viewData.visualKeyboardInput != this->getInputText())
        {
            this->setInputText(viewData.visualKeyboardInput);
        }
        this->getView()->addToRender(this->getInputTextObject());
    }

    this->renderKeyboard(viewData);

    return this;
}

VisualKeyboardView* VisualKeyboardView::loadImages()
{
    
    AbstractView::loadImages();

    this->setBackground(new RoundedRectangle(this->getView()->getRenderer(), Color::getColorFromMap(KEYBOARD_BACKGROUND_COLOR), this->getMeasure("keyboardWidth"), this->getMeasure("keyboardHeight"), this->getMeasure("keyboardX"), this->getMeasure("keyboardY"), RoundedAngles::ALL_ANGLES, KEYBOARD_ANGLE_SLOPE));

    this->setInputBar(new RoundedRectangle(this->getView()->getRenderer(), Color::getColorFromMap(KEYS_COLOR), this->getMeasure("inputBarWidth"), this->getMeasure("inputBarHeight"), this->getMeasure("keyboardUsableX"), this->getMeasure("keyboardUsableY"), RoundedAngles::ALL_ANGLES, KEY_ANGLE_SLOPE));

    int fontSize = Font::getBiggestFontToFit(FontFamily::ROBOTO, "", this->getMeasure("inputContentWidth"), this->getMeasure("inputContentHeight"));
    this->setInputText(new Text(this->getView()->getRenderer(), "", Font::getFontFromMap(FontFamily::ROBOTO, fontSize), Color::getColorFromMap(KEYS_CONTENT_COLOR), this->getMeasure("inputContentX"), this->getMeasure("inputContentY")));
    this->getInputTextObject()->centerVert(this->getMeasure("inputContentY"), this->getMeasure("inputContentYMax"));


    for(const auto& ent1 : this->getView()->getController()->viewData.keyMap)
    {
        for(const auto& ent2 : ent1.second)
        {
            for(const auto& ent3 : ent1.second)
            {
                if(ent2.first != ent3.first && ent2.second == ent3.second && !isDoublerKey(ent1.first, ent2.first))
                {
                    this->addDoubledKey(ent1.first, ent2.first, ent3.first);
                }
            }
        }
    }

    for(const auto& ent1 : this->getView()->getController()->viewData.keyMap)
    {
        for(const auto& ent2 : ent1.second)
        {
            if(!isDoublerKey(ent1.first, ent2.first))
            {
                int width, height;
                if(isDoubledKey(ent1.first, ent2.first))
                {
                    int keyDiff = this->getDoubledKey(ent1.first, ent2.first) - ent2.first;
                    if(keyDiff == 11)
                    {
                        width = this->getMeasure("rowWidth");
                        height = (this->getMeasure("rowHeight") * 2) + this->getMeasure("keyboardVerticalPadding");
                    }
                    else
                    {
                        width = (this->getMeasure("rowWidth") * 2) + this->getMeasure("keyboardHorizontalPadding");
                        height = this->getMeasure("rowHeight");
                    }
                }
                else
                {
                    width = this->getMeasure("rowWidth");
                    height = this->getMeasure("rowHeight");
                }
                this->addKeyBackground(ent1.first, ent2.first, new RoundedRectangle(this->getView()->getRenderer(), Color::getColorFromMap(KEYS_COLOR), width, height, 0, 0, RoundedAngles::ALL_ANGLES, KEY_ANGLE_SLOPE));
            }
        }
    }

    fontSize = Fonts::ROBOTOLAST;
    for(const auto& ent1 : this->getView()->getController()->viewData.keyMap)
    {
        for(const auto& ent2 : ent1.second)
        {
            if(!isSpecialKey(ent2.second))
            {
                int currentFont = Font::getBiggestFontToFit(FontFamily::ROBOTO, ent2.second, this->getMeasure("keyContentWidth"), this->getMeasure("keyContentHeight"));
                if(currentFont < fontSize)
                {
                    fontSize = currentFont;
                }
            }
        }
    }

    for(const auto& ent1 : this->getView()->getController()->viewData.keyMap)
    {
        for(const auto& ent2 : ent1.second)
        {
            if(this->getKeys().count(ent2.second) == 0)
            {
                int usedFontSize = fontSize;
                std::string content;
                if(this->isSpecialKey(ent2.second))
                {
                    content = this->getSpecialCharacter(ent2.second);
                    usedFontSize = Font::getBiggestFontToFit(FontFamily::ROBOTO, content, this->getMeasure("keyContentWidth"), this->getMeasure("keyContentHeight"));
                }
                else
                {
                    content = ent2.second;
                }
                Text* text = new Text(this->getView()->getRenderer(), content, Font::getFontFromMap(FontFamily::ROBOTO, usedFontSize), Color::getColorFromMap(KEYS_CONTENT_COLOR), this->getMeasure("keyboardUsableX"), this->getMeasure("keyUsableY"));
                this->addKey(ent2.second, text);
            }
        }
    }

    return this;
}

VisualKeyboardView* VisualKeyboardView::destroyImages()
{
    AbstractView::destroyImages();

    if(this->getBackground() != nullptr)
    {
        this->setBackground(nullptr);
    }

    if(this->getInputBar() != nullptr)
    {
        this->setInputBar(nullptr);
    }

    if(this->getInputTextObject() != nullptr)
    {
        this->setInputText(nullptr);
    }

    for(auto& ent : this->getKeys())
    {
        if(ent.second != nullptr)
        {
            delete ent.second;
        }
    }
    this->m_keys.clear();

    for(auto& ent1 : this->getKeyBackgrounds())
    {
        for(auto& ent2 : ent1.second)
        {
            if(ent2.second != nullptr)
            {
                delete ent2.second;
            }
        }
    }
    this->m_keyBackgrounds.clear();

    return this;
}

VisualKeyboardView* VisualKeyboardView::calculateMeasures()
{
    AbstractView::calculateMeasures();

    this->addMeasure("keyboardWidth", this->getMeasure("compensatedWidth") / 2);
    this->addMeasure("keyboardHeight", this->getMeasure("compensatedHeight") / 4);
    this->addMeasure("keyboardX", this->getMeasure("compensatedX") + (this->getMeasure("compensatedWidth") / 2) - (this->getMeasure("keyboardWidth") / 2));
    this->addMeasure("keyboardY", this->getMeasure("compensatedY") + (this->getMeasure("compensatedHeight") / 2) - (this->getMeasure("keyboardHeight") / 2));

    this->addMeasure("keyboardHorizontalPadding", this->getMeasure("keyboardWidth") / 89);
    this->addMeasure("keyboardVerticalPadding", this->getMeasure("keyboardHeight") / 17);
    this->addMeasure("keyboardUsableWidth", this->getMeasure("keyboardWidth") - (2 * this->getMeasure("keyboardHorizontalPadding")));
    this->addMeasure("keyboardUsableHeight", this->getMeasure("keyboardHeight") - (2 * this->getMeasure("keyboardVerticalPadding")));
    this->addMeasure("keyboardUsableX", this->getMeasure("keyboardX") + this->getMeasure("keyboardHorizontalPadding"));
    this->addMeasure("keyboardUsableXMax", this->getMeasure("keyboardUsableX") + this->getMeasure("keyboardUsableWidth"));
    this->addMeasure("keyboardUsableY", this->getMeasure("keyboardY") + this->getMeasure("keyboardVerticalPadding"));
    this->addMeasure("keyboardUsableYMax", this->getMeasure("keyboardUsableY") + this->getMeasure("keyboardUsableHeight"));

    this->addMeasure("rowHeight", (this->getMeasure("keyboardUsableHeight") - (3 * this->getMeasure("keyboardVerticalPadding"))) / 4);
    this->addMeasure("rowWidth", (this->getMeasure("keyboardUsableWidth") - (10 * this->getMeasure("keyboardHorizontalPadding"))) / 11);

    this->addMeasure("inputBarWidth", this->getMeasure("keyboardUsableWidth"));
    this->addMeasure("inputBarHeight", this->getMeasure("rowHeight"));
    this->addMeasure("inputPadding", this->getMeasure("inputBarHeight") / 5);
    this->addMeasure("inputContentWidth", this->getMeasure("inputBarWidth") - (2 * this->getMeasure("inputPadding")));
    this->addMeasure("inputContentHeight", this->getMeasure("inputBarHeight") - (2 * this->getMeasure("inputPadding")));
    this->addMeasure("inputContentX", this->getMeasure("keyboardUsableX") + this->getMeasure("inputPadding"));
    this->addMeasure("inputContentY", this->getMeasure("keyboardUsableY") + this->getMeasure("inputPadding"));
    this->addMeasure("inputContentXMax", this->getMeasure("inputContentX") + this->getMeasure("inputContentWidth"));
    this->addMeasure("inputContentYMax", this->getMeasure("inputContentY") + this->getMeasure("inputContentHeight"));

    this->addMeasure("keyUsableY", this->getMeasure("keyboardUsableY") + this->getMeasure("inputBarHeight") + this->getMeasure("keyboardVerticalPadding"));
    this->addMeasure("keyHorizontalPadding", this->getMeasure("rowWidth") / 5);
    this->addMeasure("keyVerticalPadding", this->getMeasure("rowHeight") / 5);
    this->addMeasure("keyContentWidth", this->getMeasure("rowWidth") - (2 * this->getMeasure("keyHorizontalPadding")));
    this->addMeasure("keyContentHeight", this->getMeasure("rowHeight") - (2 * this->getMeasure("keyVerticalPadding")));
    this->addMeasure("keyContentXMaxMargin", this->getMeasure("keyHorizontalPadding") + this->getMeasure("keyContentWidth"));
    this->addMeasure("keyContentYMaxMargin", this->getMeasure("keyVerticalPadding") + this->getMeasure("keyContentHeight"));

    this->addMeasure("doubleKeyHeight", (this->getMeasure("rowHeight") * 2) + this->getMeasure("keyboardVerticalPadding"));
    this->addMeasure("doubleKeyContentHeight", this->getMeasure("doubleKeyHeight") - (2 * this->getMeasure("keyVerticalPadding")));

    this->addMeasure("doubleKeyWidth", (this->getMeasure("rowWidth") * 2) + this->getMeasure("keyboardHorizontalPadding"));
    this->addMeasure("doubleKeyContentWidth", this->getMeasure("doubleKeyWidth") - (2 * this->getMeasure("keyHorizontalPadding")));

    return this;
}

VisualKeyboardView* VisualKeyboardView::renderKeyboard(const ViewData& viewData)
{
    if(viewData.keyMap.count(viewData.currentPannel) > 0)
    {
        int col = 0;
        int row = 0;
        for(const auto& ent : viewData.keyMap.at(viewData.currentPannel))
        {
            if(!this->isDoublerKey(viewData.currentPannel, ent.first))
            {
                int posX, posY;
                if(this->getKeyBackground(viewData.currentPannel, ent.first) != nullptr)
                {
                    RoundedRectangle* keyBackground = this->getKeyBackground(viewData.currentPannel, ent.first);
                    posX = this->getMeasure("keyboardUsableX") + ((this->getMeasure("rowWidth") + this->getMeasure("keyboardHorizontalPadding")) * col);
                    posY = this->getMeasure("keyUsableY") + ((this->getMeasure("rowHeight") + this->getMeasure("keyboardVerticalPadding")) * row);
                    keyBackground->setPosition(posX, posY);

                    if(ent.first == viewData.currentKey && keyBackground->getColor() == Color::getColorFromMap(KEYS_COLOR))
                    {
                        keyBackground->setColor(Color::getColorFromMap(KEYS_HILIGHT_COLOR));
                    }
                    else if(ent.first != viewData.currentKey && keyBackground->getColor() == Color::getColorFromMap(KEYS_HILIGHT_COLOR) && ent.second != "SHIFT")
                    {
                        keyBackground->setColor(Color::getColorFromMap(KEYS_COLOR));
                    }

                    if(ent.second == "SHIFT")
                    {
                        if(viewData.shiftActive && keyBackground->getColor() == Color::getColorFromMap(KEYS_COLOR))
                        {

                            keyBackground->setColor(Color::getColorFromMap(KEYS_HILIGHT_COLOR));
                        }
                        else if(!viewData.shiftActive && keyBackground->getColor() == Color::getColorFromMap(KEYS_HILIGHT_COLOR) && ent.first != viewData.currentKey)
                        {
                            keyBackground->setColor(Color::getColorFromMap(KEYS_COLOR));
                        }
                    }

                    this->getView()->addToRender(keyBackground);
                }

                if(this->getKey(ent.second) != nullptr)
                {
                    posX = posX + this->getMeasure("keyHorizontalPadding");
                    posY = posY + this->getMeasure("keyVerticalPadding");

                    int posXMax, posYMax;
                    Keys doubledKey = this->getDoubledKey(viewData.currentPannel, ent.first);
                    if(doubledKey == Keys::KEYNULL)
                    {
                        posXMax = posX + this->getMeasure("keyContentWidth");
                        posYMax = posY + this->getMeasure("keyContentHeight");
                    }
                    else
                    {
                        int keyDiff = doubledKey - ent.first;
                        if(keyDiff == 11)
                        {
                            posXMax = posX + this->getMeasure("keyContentWidth");
                            posYMax = posY + this->getMeasure("doubleKeyContentHeight");
                        }
                        else
                        {
                            posXMax = posX + this->getMeasure("doubleKeyContentWidth");
                            posYMax = posY + this->getMeasure("keyContentHeight");
                        }
                    }

                    this->getKey(ent.second)->center(posX, posXMax, posY, posYMax);
                    this->getView()->addToRender(this->getKey(ent.second));
                }
            }
            else
            {
                Keys doubledKey = this->getDoubledKey(viewData.currentPannel, ent.first);
                if(doubledKey != Keys::KEYNULL)
                {
                    if(this->getKeyBackground(viewData.currentPannel, doubledKey) != nullptr)
                    {
                        RoundedRectangle* keyBackground = this->getKeyBackground(viewData.currentPannel, doubledKey);
                        if(ent.first == viewData.currentKey && keyBackground->getColor() == Color::getColorFromMap(KEYS_COLOR))
                        {
                            keyBackground->setColor(Color::getColorFromMap(KEYS_HILIGHT_COLOR));
                        }
                    }
                }
            }


            ++col;
            if(col == 11)
            {
                ++row;
                col = 0;
            }
        }
    }

    return this;
}

VisualKeyboardView* VisualKeyboardView::addKey(const string& character, Text* key)
{
    if(this->m_keys.count(character) > 0)
    {
        if(this->m_keys.at(character) != nullptr)
        {
            delete this->m_keys.at(character);
        }
        this->m_keys.erase(character);
    }

    this->m_keys.insert(pair<string, Text*>(character, key));

    return this;
}

Text* VisualKeyboardView::getKey(const string& character) const
{
    if(this->m_keys.count(character) > 0)
    {
        return this->m_keys.at(character);
    }
    return nullptr;
}

map<string, Text*> VisualKeyboardView::getKeys() const
{
    return this->m_keys;
}

VisualKeyboardView* VisualKeyboardView::setBackground(RoundedRectangle* background)
{
    if(this->m_pBackground != nullptr)
    {
        delete this->m_pBackground;
    }
    this->m_pBackground = background;
    return this;
}

RoundedRectangle* VisualKeyboardView::getBackground() const
{
    return this->m_pBackground;
}

VisualKeyboardView* VisualKeyboardView::setInputBar(RoundedRectangle* inputBar)
{
    if(this->m_pInputBar != nullptr)
    {
        delete this->m_pInputBar;
    }
    this->m_pInputBar = inputBar;
    return this;
}

RoundedRectangle* VisualKeyboardView::getInputBar() const
{
    return this->m_pInputBar;
}

VisualKeyboardView* VisualKeyboardView::setInputText(Text* text)
{
    if(this->m_pInputText != nullptr)
    {
        delete this->m_pInputText;
    }
    this->m_pInputText = text;
    return this;
}

VisualKeyboardView* VisualKeyboardView::setInputText(const string& text)
{
    if(this->getInputTextObject() != nullptr)
    {
        int fontSize = Font::getBiggestFontToFit(FontFamily::ROBOTO, text, this->getMeasure("inputContentWidth"), this->getMeasure("inputContentHeight"));
        this->getInputTextObject()->setText(text);
        this->getInputTextObject()->setFont(Font::getFontFromMap(FontFamily::ROBOTO, fontSize));
        this->getInputTextObject()->reload();
        this->getInputTextObject()->centerVert(this->getMeasure("inputContentY"), this->getMeasure("inputContentYMax"));
    }
    return this;
}

string VisualKeyboardView::getInputText() const
{
    if(this->getInputTextObject() != nullptr)
    {
        return this->getInputTextObject()->getText();
    }
    return "";
}

Text* VisualKeyboardView::getInputTextObject() const
{
    return this->m_pInputText;
}

VisualKeyboardView* VisualKeyboardView::addKeyBackground(const KeyboardPannel& pannel, const Keys& key, RoundedRectangle* image)
{
    if(this->m_keyBackgrounds.count(pannel) > 0)
    {
        if(this->m_keyBackgrounds.at(pannel).count(key) > 0)
        {
            if(this->m_keyBackgrounds.at(pannel).at(key) != nullptr)
            {
                delete this->m_keyBackgrounds.at(pannel).at(key);
            }
        }
    }
    this->m_keyBackgrounds[pannel][key] = image;
    return this;
}

RoundedRectangle* VisualKeyboardView::getKeyBackground(const KeyboardPannel& pannel, const Keys& key) const
{
    if(this->m_keyBackgrounds.count(pannel) > 0)
    {
        if(this->m_keyBackgrounds.at(pannel).count(key) > 0)
        {
            return this->m_keyBackgrounds.at(pannel).at(key);
        }
    }
    return nullptr;
}

map<Keys, RoundedRectangle*> VisualKeyboardView::getKeyBackgrounds(const KeyboardPannel& pannel) const
{
    if(this->m_keyBackgrounds.count(pannel) > 0)
    {
        return this->m_keyBackgrounds.at(pannel);
    }
    map<Keys, RoundedRectangle*> mapR;
    return mapR;
}

map<KeyboardPannel, map<Keys, RoundedRectangle*>> VisualKeyboardView::getKeyBackgrounds() const
{
    return this->m_keyBackgrounds;
}

bool VisualKeyboardView::isSpecialKey(const string& character) const
{
    if(this->m_specialKeysConverter.count(character) > 0)
    {
        return true;
    }
    return false;
}

string VisualKeyboardView::getSpecialCharacter(const string& character) const
{
    if(this->isSpecialKey(character))
    {
        return this->m_specialKeysConverter.at(character);
    }
    return "";
}

VisualKeyboardView* VisualKeyboardView::addDoubledKey(const KeyboardPannel& pannel, const Keys& key1, const Keys& key2)
{
    this->m_doubledKeys[pannel][key1] = key2;
    this->m_doubledKeysReverser[pannel][key2] = key1;
    return this;
}

Keys VisualKeyboardView::getDoubledKey(const KeyboardPannel& pannel, const Keys& key) const
{
    if(this->m_doubledKeys.count(pannel) > 0)
    {
        if(this->m_doubledKeys.at(pannel).count(key) > 0)
        {
            return this->m_doubledKeys.at(pannel).at(key);
        }
    }

    if(this->m_doubledKeysReverser.count(pannel) > 0)
    {
        if(this->m_doubledKeysReverser.at(pannel).count(key) > 0)
        {
            return this->m_doubledKeysReverser.at(pannel).at(key);
        }
    }
    return Keys::KEYNULL;
}

bool VisualKeyboardView::isDoublerKey(const KeyboardPannel& pannel, const Keys& key) const
{
    if(this->m_doubledKeysReverser.count(pannel) > 0)
    {
        if(this->m_doubledKeysReverser.at(pannel).count(key) > 0)
        {
            return true;
        }
    }
    return false;
}

bool VisualKeyboardView::isDoubledKey(const KeyboardPannel& pannel, const Keys& key) const
{
    if(this->m_doubledKeys.count(pannel) > 0)
    {
        if(this->m_doubledKeys.at(pannel).count(key) > 0)
        {
            return true;
        }
    }
    return false;
}
