#ifndef DEF_SURFACE
#define DEF_SURFACE

#include <SDL2/SDL.h>
#include <map>

enum NostalColors
{
    MAIN_BLUE,
    MAIN_BLUE_SEMI_TRANSPARENT,
    MAIN_BLUE_QUARTER_TRANSPARENT,
    DARK_BLUE,
    DARK_BLUE_SEMI_TRANSPARENT,
    DARK_BLUE_THIRD_TRANSPARENT,
    DARK_BLUE_QUARTER_TRANSPARENT,
    LIGHT_BLUE,
    LIGHT_BLUE_SEMI_TRANSPARENT,
    LIGHT_BLUE_THIRD_TRANSPARENT,
    LIGHT_BLUE_QUARTER_TRANSPARENT,
    GREY,
    GREY_SEMI_TRANSPARENT,
    GREY_THIRD_TRANSPARENT,
    GREY_QUARTER_TRANSPARENT,
    WHITE,
    WHITE_SEMI_TRANSPARENT,
    WHITE_THIRD_TRANSPARENT,
    WHITE_QUARTER_TRANSPARENT
};

//Classe Color, gère les couleurs, contiens les données RGBA

class Color
{
public:
    Color(const int& r, const int& g, const int& b, const int& a = 255);

    int getR() const;
    void setR(const int& r);
    int getG() const;
    void setG(const int& g);
    int getB() const;
    void setB(const int& b);
    int getA() const;
    void setA(const int& a);

    int compare(Color* color);

    static Color* getColorFromMap(const NostalColors& color);

    static std::map<NostalColors, Color*> nostalColorMap;

private:
    int m_r;
    int m_g;
    int m_b;
    int m_a;
};

//Super Classe Renderable, classe mère pour tous les objets qui peuvent être affichés et disposent donc d'une fonction render()

class Renderable
{
public:
    Renderable(SDL_Renderer* renderer);

    virtual Renderable* render() = 0;

    SDL_Renderer* getRenderer() const;
    Renderable* setRenderer(SDL_Renderer* renderer);

protected:
    SDL_Renderer* m_pRenderer;
};

//Classe surface, classe mère pour toutes les surfaces dessinées

class Surface : public Renderable
{
public:
    Surface(SDL_Renderer* renderer, Color* color, const int& width, const int& height, const int& posX, const int& posY);

    virtual Surface* render() = 0;

    Color* getColor() const;
    virtual Surface* setColor(Color* color);

    int getWidth() const;
    int getHeight() const;
    virtual Surface* setSize(const int& width, const int& height);

    int getPosX() const;
    int getPosY() const;
    virtual Surface* setPosition(const int& posX, const int& posY);

    bool isRenderable() const;

protected:
    Color* m_pColor;
    int m_width;
    int m_height;
    int m_posX;
    int m_posY;
};

//Classe line, Objet permetant de dessiner des lignes

class Line : public Renderable
{
public:
    Line(SDL_Renderer* renderer, Color* color, const int& x1, const int& y1, const int& x2, const int& y2);
    Line(SDL_Renderer* renderer, Color* color, const int& x1, const int& y1, const int& x2, const int& y2, const int& width);

    Line* render();

    Color* getColor() const;
    virtual Line* setColor(Color* color);

    int getX1() const;
    int getY1() const;
    Line* setOrigin(const int& x1, const int& y1);

    int getX2() const;
    int getY2() const;
    Line* setDestination(const int& x2, const int& y2);

    int getWidth() const;
    Line* setWidth(const int& width);

protected:
    Color* m_pColor;
    int m_x1;
    int m_y1;
    int m_x2;
    int m_y2;
    int m_width;
};

//Classe rectangle, Objet permetant de dessiner des rectangles

class Rectangle : public  Surface
{
public:
    Rectangle(SDL_Renderer* renderer, Color* color, const int& width, const int& height, const int& posX, const int& posY);

    virtual Rectangle* render();

    virtual Rectangle* setSize(const int& width, const int& height);

    virtual Rectangle* setPosition(const int& posX, const int& posY);

    SDL_Rect getRect() const;

protected:
    SDL_Rect m_rect;
};

//Classe Ellipse, objet permetant de dessiner des ellipses et cercles

class Ellipse : public Surface
{
public:
    Ellipse(SDL_Renderer* renderer, Color* color, const int& width, const int& height, const int& posX, const int& posY);

    virtual Ellipse* render();

protected:

};

enum CircleQuarter
{
    TOP_RIGHT,
    BOTTOM_RIGHT,
    TOP_LEFT,
    BOTTOM_LEFT
};

//Classe QuarterCircle, objet permetant de dessiner des quarts de cercle

class QuarterCircle : public Surface
{
public:
    QuarterCircle(SDL_Renderer* renderer, Color* color, const int& radius, const int& posX, const int& posY, const CircleQuarter& quarter);

    virtual QuarterCircle* render();

    CircleQuarter getQuarter() const;
    QuarterCircle* setQuarter(const CircleQuarter& quarter);

protected:
    CircleQuarter m_quarter;

};

enum RoundedAngles
{
    TOP_ANGLES,
    RIGHT_ANGLES,
    BOTTOM_ANGLES,
    LEFT_ANGLES,
    ALL_ANGLES
};

//Classe RoundedRectangle, objet permettant de dessiner des rectangles aux bords arrondis

class RoundedRectangle : public Surface
{
public:
    RoundedRectangle(SDL_Renderer* renderer, Color* color, const int& width, const int& height, const int& posX, const int& posY, const RoundedAngles& roundedAngles = RoundedAngles::ALL_ANGLES, const int& slopePercentage = 25);

    virtual RoundedRectangle* render();

    virtual RoundedRectangle* setSize(const int& width, const int& height);

    virtual RoundedRectangle* setPosition(const int& posX, const int& posY);

    virtual RoundedRectangle* setColor(Color* color);

    RoundedRectangle* setRoundedAngles(const RoundedAngles& roundedAngles);
    RoundedAngles getRoundedAngles() const;

    RoundedRectangle* setSlopePercentage(const int& slopePercentage);
    int getSlopePercentage() const;

protected:
    RoundedAngles m_roundedAngles;
    int m_slopePercentage;
    Rectangle rect1;
    Rectangle rect2;
    Rectangle rect3;
    QuarterCircle corn1;
    QuarterCircle corn2;
    QuarterCircle corn3;
    QuarterCircle corn4;
};

#endif
