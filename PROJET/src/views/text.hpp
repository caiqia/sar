#ifndef DEF_TEXT
#define DEF_TEXT

#include <string>
#include <map>

#include <SDL2/SDL_ttf.h>

#include "surface.hpp"
#include "font.hpp"

class Text : public Renderable
{
public:
    Text(SDL_Renderer* renderer, const std::string& text, Font* font, Color* color, const int& posX, const int& posY);
    ~Text();

    virtual Text* render();

    Text* reload();

    std::string getText() const;
    Text* setText(const std::string& text);

    Font* getFont() const;
    TTF_Font* getTTFFont() const;
    Text* setFont(Font* font);

    Color* getColor() const;
    Text* setColor(Color* color);

    int getPosX() const;
    int getPosY() const;
    virtual Text* setPosition(const int& posX, const int& posY);
    Text* centerHoriz(const int& horiz0, const int& horizMax);
    Text* centerVert(const int& vert0, const int& vertMax);
    Text* center(const int& horiz0, const int& horizMax, const int& vert0, const int& vertMax);

    int getTextWidth() const;
    static int getTextWidth(const std::string& text, const Fonts& font);
    static int getTextWidth(const std::string& text, const FontFamily& fontFamily, const int& size);
    static int getTextWidth(const std::string& text, Font* font);

    SDL_Rect getRect() const;

    SDL_Texture* getTexture() const;
    Text* setTexture(SDL_Texture* texture);

    static void flushFonts();

    Text* cropToFit(const int& width, const int& offset = 0, const std::string& trailingString = "...");

protected:
    std::string m_text;
    Color* m_color;
    Font* m_pFont = nullptr;
    int m_posX = 0;
    int m_posY = 0;
    SDL_Rect m_rect;
    SDL_Texture* m_pTexture = nullptr;
};

#endif
