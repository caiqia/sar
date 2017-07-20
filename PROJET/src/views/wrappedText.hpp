#ifndef DEF_WRAPPEDTEXT
#define DEF_WRAPPEDTEXT

#include "text.hpp"
#include "font.hpp"
#include "surface.hpp"

#include <vector>

class WrappedText : public Renderable
{
public:
    WrappedText(SDL_Renderer* renderer, const std::string& text, FontFamily fontFamily, Color* color, const int& width, const int& height, const int& posX, const int& posY);
    ~WrappedText();

    virtual WrappedText* render();

    WrappedText* createTextObjectsFromString();

    WrappedText* centerVert(const int& posY, const int& posYMax);
    WrappedText* centerHoriz(const int& posX, const int& posXMax);
    WrappedText* center(const int& posX, const int& posXMax, const int& posY, const int& posYMax);

    std::string getText() const;
    WrappedText* setText(const std::string& text);

    FontFamily getFontFamily() const;
    WrappedText* setFontFamily(FontFamily fontFamily);

    int getFontSize() const;
    WrappedText* setFontSize(const int& fontSize);

    Color* getColor() const;
    WrappedText* setColor(Color* color);

    int getPosX() const;
    int getPosY() const;
    WrappedText* setPosition(const int& posX, const int& posY);

    int getWidth() const;
    int getHeight() const;
    WrappedText* setSize(const int& width, const int& height);

    std::vector<Text*> getTextLines() const;

protected:
    std::string m_text;
    Color* m_pColor;
    FontFamily m_fontFamily;
    int m_fontSize;
    int m_posX = 0;
    int m_posY = 0;
    int m_width = 0;
    int m_height = 0;

    std::vector<Text*> m_textLines;
};

#endif
