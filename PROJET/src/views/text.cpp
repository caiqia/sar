#include <SDL2/SDL_ttf.h>
#include <iostream>

#include "text.hpp"
#include "../entities/log.hpp"
#include "../entities/error.hpp"
using namespace std;

//Text Object used to manipulate and display texts
//Inherits from Renderable

//Loads a text object based on the desired text, font, color and position
Text::Text(SDL_Renderer* renderer, const string& text, Font* font, Color* color, const int& posX, const int& posY) : Renderable(renderer)
{
    this->setText(text);
    this->setFont(font);
    this->setColor(color);
    this->setPosition(posX, posY);

    this->reload();
}

//frees the SDL texture object from ram
Text::~Text()
{
    SDL_DestroyTexture(this->getTexture());
}

//Displays the Text
Text* Text::render()
{
    Renderable::render();
    SDL_RenderCopy(this->getRenderer(), this->getTexture(), NULL, &this->m_rect);
    return this;
}

//Reloads the SDL texture, for example when the text has changed
Text* Text::reload()
{
    SDL_Color color = {this->getColor()->getR(), this->getColor()->getG(), this->getColor()->getB()};
    SDL_Surface* surface;

    if(!( surface = TTF_RenderUTF8_Blended(this->getTTFFont(), this->getText().c_str(), color)))
    {
        string m_error = TTF_GetError();

        Log::addEntry(1, "/!\\ Could not create surface from text \"" + this->getText() + "\" : " + m_error);
    }
    else
    {
        SDL_Texture* newTexture = SDL_CreateTextureFromSurface(this->getRenderer(), surface);

        if(newTexture == NULL)
        {
            string m_error = SDL_GetError();

            Log::addEntry(1, "/!\\ Could not create texture from text surface \"" + this->getText() + "\" : " + m_error);
        }
        else
        {
            SDL_Texture* formerTexture = this->getTexture();

            this->setTexture(newTexture);

            if(formerTexture != nullptr)
            {
                SDL_DestroyTexture(formerTexture);
            }

            int w, h;
            SDL_QueryTexture(this->getTexture(), NULL, NULL, &w, &h);
            this->m_rect.h = h;
            this->m_rect.w = w;
        }
        SDL_FreeSurface(surface);

    }
    return this;
}

string Text::getText() const
{
    return this->m_text;
}

Text* Text::setText(const string& text)
{
    this->m_text = text;
    return this;
}

Font* Text::getFont() const
{
    return this->m_pFont;
}

TTF_Font* Text::getTTFFont() const
{
    return this->m_pFont->getFont();
}

Text* Text::setFont(Font* font)
{
    this->m_pFont = font;
    return this;
}

Color* Text::getColor() const
{
    return this->m_color;
}

Text* Text::setColor(Color* color)
{
    this->m_color = color;
    return this;
}

int Text::getPosX() const
{
    return this->m_posX;
}

int Text::getPosY() const
{
    return this->m_posY;
}

Text* Text::setPosition(const int& posX, const int& posY)
{
    this->m_posX = posX;
    this->m_posY = posY;
    this->m_rect.x = posX;
    this->m_rect.y = posY;
    return this;
}

//center the text horizontaly between two X values
Text* Text::centerHoriz(const int& horiz0, const int& horizMax)
{
    if(horizMax > horiz0)
    {
        int w, h, span, posX;
        SDL_QueryTexture(this->getTexture(), NULL, NULL, &w, &h);
        span = horizMax - horiz0;
        posX = horiz0 + span / 2 - w / 2;
        this->setPosition(posX, this->getPosY());
    }
    else
    {
        Log::addEntry(1, "/!\\ To center \"" + this->getText() + "\", horiz0 mus be bigger then horizMax in function centerHoriz");
    }

    return this;
}


//Center the text vertically between two Y value
Text* Text::centerVert(const int& vert0, const int& vertMax)
{
    if(vertMax > vert0)
    {
        int w, h, span, posY;
        SDL_QueryTexture(this->getTexture(), NULL, NULL, &w, &h);
        span = vertMax - vert0;
        posY = vert0 + span / 2 - h / 2;
        this->setPosition(this->getPosX(), posY);
    }
    else
    {
        Log::addEntry(1, "/!\\ To center \"" + this->getText() + "\", vert0 mus be bigger then vertMax in function centerVert");

    }
}

//centers the text between two X values and two Y values
Text* Text::center(const int& horiz0, const int& horizMax, const int& vert0, const int& vertMax)
{
    this->centerHoriz(horiz0, horizMax);
    this->centerVert(vert0, vertMax);
    return this;
}

int Text::getTextWidth() const
{
    return this->m_rect.w;
}

//Gets the width of a string in a particular font by simulating a render
int Text::getTextWidth(const std::string& text, const Fonts& font)
{
    if(Font::getFontFromMap(font)->getFont() != nullptr)
    {
        return Text::getTextWidth(text, Font::getFontFromMap(font));
    }
    return 0;
}

int Text::getTextWidth(const std::string& text, const FontFamily& fontFamily, const int& size)
{
    if(Font::getFontFromMap(fontFamily, size)->getFont() != nullptr)
    {
        return Text::getTextWidth(text, Font::getFontFromMap(fontFamily, size));
    }
    return 0;
}

int Text::getTextWidth(const std::string& text, Font* font)
{
    int w = 0;
    if(TTF_SizeUTF8(font->getFont(), text.c_str(), &w, NULL))
    {
        Log::addEntry(5, "Coud not read width from font");
        Log::addEntry(5, TTF_GetError());
        return 0;
    }
    else
    {
        return w;
    }
}

SDL_Rect Text::getRect() const
{
    return this->m_rect;
}

SDL_Texture* Text::getTexture() const
{
    return this->m_pTexture;
}

Text* Text::setTexture(SDL_Texture* texture)
{
    this->m_pTexture = texture;
    return this;
}

void Text::flushFonts()
{
    for(auto& ent : Font::fontMap)
    {
        delete(ent.second);
    }
}

Text* Text::cropToFit(const int& width, const int& offset, const string& trailingString)
{
    string text = this->getText();
    bool modified = false;

    if(Text::getTextWidth(text, this->getFont()) > width)
    {
        while(Text::getTextWidth(text + trailingString, this->getFont()) > width && this->getText().size() >= 0)
        {
            text.resize(text.size() - 1);
        }
        modified = true;
    }

    if(modified)
    {
        text += trailingString;
    }
    this->setText(text);
    return this;
}
