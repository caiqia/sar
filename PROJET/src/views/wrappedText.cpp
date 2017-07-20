#include "wrappedText.hpp"

#include <iostream>
#include <sstream>
#include <algorithm>
#include <iterator>

using namespace std;

WrappedText::WrappedText(SDL_Renderer* renderer, const string& text, FontFamily fontFamily, Color* color, const int& width, const int& height, const int& posX, const int& posY)
: Renderable(renderer)
{
    this->setText(text);
    this->setFontFamily(fontFamily);
    this->setColor(color);
    this->setSize(width, height);
    this->setPosition(posX, posY);

    this->createTextObjectsFromString();
}

WrappedText::~WrappedText()
{
    for(auto& ent : this->getTextLines())
    {
        if(ent != nullptr)
        {
            delete ent;
        }
    }
}

WrappedText* WrappedText::render()
{
    Renderable::render();

    for(auto& ent : this->getTextLines())
    {
        if(ent != nullptr)
        {
            ent->render();
        }
    }
}

WrappedText* WrappedText::createTextObjectsFromString()
{
    for(auto& ent : this->getTextLines())
    {
        if(ent != nullptr)
        {
            delete ent;
        }
    }

    this->m_textLines.clear();

    istringstream iss(this->getText());

    vector<string> words{istream_iterator<string>{iss},
                         istream_iterator<string>{}};

    int fontSize = 0;
    int smallestFont = 0;

    switch(this->getFontFamily())
    {
        case FontFamily::ROBOTO :
        {
            fontSize = ROBOTOLAST;
            smallestFont = ROBOTOFIRST;
            break;
        }
        case FontFamily::AQUINO :
        {
            fontSize = AQUINOLAST;
            smallestFont = AQUINOFIRST;
            break;
        }
    }

    bool fits = false;
    vector<string> lines;

    while(!fits)
    {
        int fontHeight, lineSkipHeight, possbileLines;

        Font* testFont = Font::getFontFromMap(this->getFontFamily(), fontSize);

        if(testFont != nullptr)
        {

            fontHeight = Font::getMaxHeight(testFont);
            lineSkipHeight = Font::getLineSkip(testFont);
        }

        possbileLines = this->getHeight() / lineSkipHeight;

        if(possbileLines > 0)
        {
            int wordCount = words.size();
            int i = 0;

            lines.clear();
            while(lines.size() < possbileLines)
            {
                bool lineOK = false;
                string line = "";
                string toAdd = "";

                while(!lineOK  && words.size() > i)
                {
                    toAdd = "";

                    if(line != "")
                    {
                        toAdd += " ";
                    }

                    toAdd += words[i];

                    if(this->getWidth() > Text::getTextWidth( line + toAdd, this->getFontFamily(), fontSize))
                    {
                        ++i;
                        line += toAdd;
                    }
                    else
                    {
                        lineOK = true;
                    }

                }
                lines.push_back(line);
            }

            if(i == words.size())
            {
                this->setFontSize(fontSize);
                fits = true;
            }

            else if(fontSize != smallestFont)
            {
                fontSize = Font::getSmallerFont(this->getFontFamily(), fontSize);
            }
            else
            {
                fits = true;
            }


        }
        else if(fontSize != smallestFont)
        {
            fontSize = Font::getSmallerFont(this->getFontFamily(), fontSize);
        }
        else
        {
            fits = true;
        }
    }

    int i = 0;
    for(auto& ent : lines)
    {
        if(ent != "")
        {
            Font* finalFont = Font::getFontFromMap(this->getFontFamily(), fontSize);
            int lineHeight = Font::getLineSkip(finalFont);
            int posY = this->getPosY() + (i * lineHeight);
            Text* txtLine = new Text(this->getRenderer(), ent, finalFont, this->getColor(), this->getPosX(), posY);

            if(txtLine != nullptr)
            {
                this->m_textLines.push_back(txtLine);
            }

            ++i;
        }
    }


    return this;
}

WrappedText* WrappedText::centerVert(const int& posY, const int& posYMax)
{
    int numberLines = this->getTextLines().size();
    int lineHeight = Font::getLineSkip(Font::getFontFromMap(this->getFontFamily(), this->getFontSize()));

    if(numberLines != 0)
    {
        int totalHeight = lineHeight * numberLines;
        int availableHeight = posYMax - posY;

        int newPosY = posY + (availableHeight / 2)  - (totalHeight / 2);
        this->setPosition(this->getPosX(), newPosY);
    }

    int i = 0;
    for(auto& ent : this->getTextLines())
    {
        if(ent != nullptr)
        {
            ent->setPosition(ent->getPosX(), this->getPosY() + (i * lineHeight));
            ++i;
        }
    }
    return this;
}

WrappedText* WrappedText::centerHoriz(const int& posX, const int& posXMax)
{
    this->setPosition(posX, this->getPosY());

    for(auto & ent : this->getTextLines())
    {
        if(ent != nullptr)
        {
            ent->centerHoriz(posX, posXMax);
        }
    }
    return this;
}

WrappedText* WrappedText::center(const int& posX, const int& posXMax, const int& posY, const int& posYMax)
{
    this->centerHoriz(posX, posXMax);
    this->centerVert(posY, posYMax);
    return this;
}

string WrappedText::getText() const
{
    return this->m_text;
}

WrappedText* WrappedText::setText(const string& text)
{
    this->m_text = text;
    this->createTextObjectsFromString();
    return this;
}

FontFamily WrappedText::getFontFamily() const
{
    return this->m_fontFamily;
}

WrappedText* WrappedText::setFontFamily(FontFamily fontFamily)
{
    this->m_fontFamily = fontFamily;
    return this;
}

int WrappedText::getFontSize() const
{
    return this->m_fontSize;
}

WrappedText* WrappedText::setFontSize(const int& fontSize)
{
    this->m_fontSize = fontSize;
    return this;
}

Color* WrappedText::getColor() const
{
    return this->m_pColor;
}

WrappedText* WrappedText::setColor(Color* color)
{
    this->m_pColor = color;

    for(auto& ent : this->getTextLines())
    {
        if(ent != nullptr)
        {
            ent->setColor(color);
        }
    }

    return this;
}

int WrappedText::getPosX() const
{
    return this->m_posX;
}

int WrappedText::getPosY() const
{
    return this->m_posY;
}

WrappedText* WrappedText::setPosition(const int& posX, const int& posY)
{
    this->m_posX = posX;
    this->m_posY = posY;

    return this;
}

int WrappedText::getWidth() const
{
    return this->m_width;
}

int WrappedText::getHeight() const
{
    return this->m_height;
}

WrappedText* WrappedText::setSize(const int& width, const int& height)
{
    this->m_width = width;
    this->m_height = height;

    return this;
}

std::vector<Text*> WrappedText::getTextLines() const
{
    return this->m_textLines;
}
