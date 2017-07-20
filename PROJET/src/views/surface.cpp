#include <iostream>
#include <set>

#include "surface.hpp"
#include "../entities/log.hpp"
#include "../entities/error.hpp"

using namespace std;

//A map of common colors that can be accessed with more ease
map<NostalColors, Color*> Color::nostalColorMap = {{NostalColors::MAIN_BLUE, new Color(57, 94, 127)},
                                    {NostalColors::MAIN_BLUE_SEMI_TRANSPARENT, new Color(57, 94, 127, 128)},
                                    {NostalColors::MAIN_BLUE_QUARTER_TRANSPARENT, new Color(57, 94, 127, 64)},
                                    {NostalColors::DARK_BLUE, new Color(71, 98, 127)},
                                    {NostalColors::DARK_BLUE_SEMI_TRANSPARENT, new Color(71, 98, 127, 128)},
                                    {NostalColors::DARK_BLUE_THIRD_TRANSPARENT, new Color(71, 98, 127, 85)},
                                    {NostalColors::DARK_BLUE_QUARTER_TRANSPARENT, new Color(71, 98, 127, 64)},
                                    {NostalColors::LIGHT_BLUE, new Color(56, 136, 220)},
                                    {NostalColors::LIGHT_BLUE_SEMI_TRANSPARENT, new Color(56, 136, 220, 128)},
                                    {NostalColors::LIGHT_BLUE_THIRD_TRANSPARENT, new Color(56, 136, 220, 85)},
                                    {NostalColors::LIGHT_BLUE_QUARTER_TRANSPARENT, new Color(56, 136, 220, 64)},
                                    {NostalColors::GREY, new Color(128, 128, 128)},
                                    {NostalColors::GREY_SEMI_TRANSPARENT, new Color(128, 128, 128, 128)},
                                    {NostalColors::GREY_THIRD_TRANSPARENT, new Color(128, 128, 128, 85)},
                                    {NostalColors::GREY_QUARTER_TRANSPARENT, new Color(128, 128, 128, 64)},
                                    {NostalColors::WHITE, new Color(255, 255, 255)},
                                    {NostalColors::WHITE_SEMI_TRANSPARENT, new Color(255, 255, 255, 128)},
                                    {NostalColors::WHITE_THIRD_TRANSPARENT, new Color(255, 255, 255, 85)},
                                    {NostalColors::WHITE_QUARTER_TRANSPARENT, new Color(255, 255, 255, 64)}};

/*********
 * Color *
 *********/

 //This object is used to manipulate and represent colors

//Color is here defined by it's RGB value
Color::Color(const int& r, const int& g, const int& b, const int& a)
{
    if(r >= 0 && r <= 255 && g >= 0 && g <= 255 && b >= 0 && b <= 255 && a >= 0 && a <= 255)
    {
        this->setR(r);
        this->setG(g);
        this->setB(b);
        this->setA(a);
    }
    else
    {

        Log::addEntry(1, "/!\\ Could not create color : RGB values must be between 0 and 255 ! Given : " + to_string(r) + ", " + to_string(g) + ", " + to_string(b));
    }
}

int Color::getR() const
{
    return this->m_r;
}

void Color::setR(const int& r)
{
    this->m_r = r;
    return;
}

int Color::getG() const
{
    return this->m_g;
}

void Color::setG(const int& g)
{
    this->m_g = g;
    return;
}

int Color::getB() const
{
    return this->m_b;
}

void Color::setB(const int& b)
{
    this->m_b = b;
    return;
}

int Color::getA() const
{
    return this->m_a;
}

void Color::setA(const int& a)
{
    this->m_a = a;
    return;
}

//Color equality comparator based on their RGB value
int Color::compare(Color* color)
{
    if(this->getR() == color->getR() && this->getG() == color->getG() && this->getB() == color->getB())
        return 1;
    else
        return 0;
    return -1;
}

//Static function to get a color object from a NostalColors enum
Color* Color::getColorFromMap(const NostalColors& color)
{
    if(Color::nostalColorMap.count(color) > 0)
    {
        return Color::nostalColorMap.at(color);
    }
    return nullptr;
}

/*************
* Renderable *
**************/

//this class is virtual and should not be instanciated
//It offers the basic methods and attributes for all objects that can be rendered

Renderable::Renderable(SDL_Renderer* renderer)
{
    this->setRenderer(renderer);
    return;
}

Renderable* Renderable::render()
{
    return this;
}

SDL_Renderer* Renderable::getRenderer() const
{
    return this->m_pRenderer;
}

Renderable* Renderable::setRenderer(SDL_Renderer* render)
{
    this->m_pRenderer = render;
    return this;
}

/***********
 * Surface *
 ***********/

 //This class is virtual and inherits from Renderable, do not instanciate
 //It is common to all the drawn surfaces

Surface::Surface(SDL_Renderer* renderer, Color* color, const int& width, const int& height, const int& posX, const int& posY) : Renderable(renderer)
{
    this->setColor(color);
    this->setSize(width, height);
    this->setPosition(posX, posY);
    return;
}

Surface* Surface::render()
{
    Renderable::render();
    SDL_SetRenderDrawColor(this->getRenderer(), this->getColor()->getR(), this->getColor()->getG(), this->getColor()->getB(),this->getColor()->getA());
    SDL_SetRenderDrawBlendMode(this->getRenderer(), SDL_BLENDMODE_BLEND);
    return this;
}

Color* Surface::getColor() const
{
    return this->m_pColor;
}

Surface* Surface::setColor(Color* color)
{
    this->m_pColor = color;
    return this;
}

int Surface::getWidth() const
{
    return this->m_width;
}

int Surface::getHeight() const
{
    return this->m_height;
}

Surface* Surface::setSize(const int& width, const int& height)
{
    this->m_width = width;
    this->m_height = height;
    return this;
}

int Surface::getPosX() const
{
    return this->m_posX;
}

int Surface::getPosY() const
{
    return this->m_posY;
}

Surface* Surface::setPosition(const int& posX, const int& posY)
{
    this->m_posX = posX;
    this->m_posY = posY;
    return this;
}

bool Surface::isRenderable() const
{
    if(this->getWidth() > 0 && this->getHeight() > 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

/***************
 * Classe Line *
 ***************/
 //inherits from Renderable
 // used to draw lines

Line::Line(SDL_Renderer* renderer, Color* color, const int& x1, const int& y1, const int& x2, const int& y2) : Renderable(renderer)
{
    this->setColor(color);
    this->setOrigin(x1, y1);
    this->setDestination(x2, y2);
    this->setWidth(1);
}

Line::Line(SDL_Renderer* renderer, Color* color, const int& x1, const int& y1, const int& x2, const int& y2, const int& width) : Line(renderer, color, x1, y1, x2, y2)
{
    this->setWidth(width);
}

Line* Line::render()
{
    Renderable::render();

    SDL_RenderDrawLine(this->getRenderer(), this->getX1(), this->getY1(), this->getX2(), this->getY2());
    return this;
}

Color* Line::getColor() const
{
    return this->m_pColor;
}

Line* Line::setColor(Color* color)
{
    this->m_pColor = color;
    return this;
}

int Line::getX1() const
{
    return this->m_x1;
}

int Line::getY1() const
{
    return this->m_y1;
}

Line* Line::setOrigin(const int& x1, const int& y1)
{
    this->m_x1 = x1;
    this->m_y1 = y1;
    return this;
}

int Line::getX2() const
{
    return this->m_x2;
}

int Line::getY2() const
{
    return this->m_y2;
}

Line* Line::setDestination(const int& x2, const int& y2)
{
    this->m_x2 = x2;
    this->m_y2 = y2;
    return this;
}

int Line::getWidth() const
{
    return this->m_width;
}

Line* Line::setWidth(const int& width)
{
    this->m_width = width;
    return this;
}

/********************
 * Classe Rectangle *
 ********************/

 //This class inherits from Surface
 //Used to draw rectangles

 //Loads a rectangle with the specified color, size and position
Rectangle::Rectangle(SDL_Renderer* renderer, Color* color, const int& width, const int& height, const int& posX, const int& posY) : Surface(renderer, color, width, height, posX, posY)
{
    this->m_rect.w = width;
    this->m_rect.h = height;
    this->m_rect.x = posX;
    this->m_rect.y = posY;
}

//Displays the rectangle
Rectangle* Rectangle::render()
{
    Surface::render();

    if(this->isRenderable())
    {
        SDL_RenderFillRect(this->getRenderer(), &this->m_rect);
    }
    return this;
}

Rectangle* Rectangle::setSize(const int& width, const int& height)
{
    Surface::setSize(width, height);
    this->m_rect.w = width;
    this->m_rect.h = height;
    return this;
}

Rectangle* Rectangle::setPosition(const int& posX, const int& posY)
{
    Surface::setPosition(posX, posY);
    this->m_rect.x = posX;
    this->m_rect.y = posY;
    return this;
}

SDL_Rect Rectangle::getRect() const
{
    return this->m_rect;
}

/******************
 * Classe Ellipse *
 ******************/


//Inherits from Surface
//This class is used to draw ellipses (And circles of course)

//Loads an Ellipse with the specified Color, size and position
// DANGER : Do not use Color transparency as it is not well managed by this shape
Ellipse::Ellipse(SDL_Renderer* renderer, Color* color, const int& width, const int& height, const int& posX, const int& posY) : Surface(renderer, color, width, height, posX, posY)
{
    return;
}

//Displays the ellipse
Ellipse* Ellipse::render()
{
    Surface::render();
    if(this->isRenderable())
    {
        SDL_Renderer* renderer = this->getRenderer();
        int width = this->getWidth() / 2, height = this->getHeight() / 2, posX = this->getPosX(), posY = this->getPosY();
        int originX = posX + (this->getWidth() / 2), originY = posY + (this->getHeight() / 2);
        int hh = height * height, ww = width * width, hhww = hh * ww;
        int x0 = width;
        int dx = 0;

        for(int x = -width; x <= width; x++)
            SDL_RenderDrawPoint(renderer, originX + x, originY);

        for(int y = 1; y <= height; y++)
        {
            int x1 = x0 - (dx - 1);
            for( ; x1 > 0; x1--)
                if(x1*x1*hh + y*y*ww <= hhww)
                    break;

            dx = x0 - x1;
            x0 = x1;

            for(int x= -x0; x<= x0; x++)
            {
                SDL_RenderDrawPoint(renderer, originX + x, originY - y);
                SDL_RenderDrawPoint(renderer, originX + x, originY + y);
            }
        }
    }


    return this;
}

/**************************************************************
 * Classe QuarterCircle, hérite de Surface                    *
 * Quarter : de 1 à 4 dans le sens des aiguilles d'une montre *
 **************************************************************/

//Inherits from surface
//Used to draw quarter circles

//Loads a quarter circle with specified Color, size, position and which quarter to display
// Quarter : value between 1 and 4, goes clockwise from the top right quarter
// WARNING : width and height have to be the same !
QuarterCircle::QuarterCircle(SDL_Renderer* renderer, Color* color, const int& radius, const int& posX, const int& posY, const CircleQuarter& quarter)
: Surface(renderer, color, radius, radius, posX, posY)
{
    this->setQuarter(quarter);
    return;
}

//Displays the quarter circle
QuarterCircle* QuarterCircle::render()
{
    Surface::render();
    if(this->isRenderable())
    {
        SDL_Renderer* renderer = this->getRenderer();
        CircleQuarter quarter = this->getQuarter();
        int x0, y0 , radius;

        switch (quarter)
        {
            case CircleQuarter::TOP_RIGHT :
                x0 = this->getPosX(), y0 = this->getPosY() + this->getHeight(), radius = this->getWidth();
                break;
            case CircleQuarter::BOTTOM_RIGHT :
                x0 = this->getPosX(), y0 = this->getPosY(), radius = this->getWidth();
                break;
            case CircleQuarter::BOTTOM_LEFT :
                x0 = this->getPosX() + this->getWidth(), y0 = this->getPosY(), radius = this-> getWidth();
                break;
            case CircleQuarter::TOP_LEFT :
                x0 = this->getPosX() + this->getWidth(), y0 = this->getPosY() + this->getHeight(), radius = this->getWidth();
                break;
        }

        int x = 0, y = radius;
    	int dp = 1 - radius;
        set<int> donePixels;

    	do
    	{
    		if (dp < 0)
    			dp = dp + 2 * (++x) + 3;
    		else
    			dp = dp + 2 * (++x) - 2 * (--y) + 5;

            /* Midpoint algorithm used to calculate the outer points of a circle
            Here we are juste derivating it by drawing a line between the outer point of the circle and its equivalent on the radius
    		SDL_RenderDrawPoint(renderer, x0 + x, y0 + y);
    		SDL_RenderDrawPoint(renderer, x0 - x, y0 + y);
    		SDL_RenderDrawPoint(renderer, x0 + x, y0 - y);
    		SDL_RenderDrawPoint(renderer, x0 - x, y0 - y);
    		SDL_RenderDrawPoint(renderer, x0 + y, y0 + x);
    		SDL_RenderDrawPoint(renderer, x0 - y, y0 + x);
    		SDL_RenderDrawPoint(renderer, x0 + y, y0 - x);
    		SDL_RenderDrawPoint(renderer, x0 - y, y0 - x); */

            int xOrigin1, yOrigin1, xDest1, yDest1, xOrigin2, yOrigin2, xDest2, yDest2;

            switch (quarter)
            {
                case CircleQuarter::TOP_RIGHT :
                    xOrigin1 = x0 + x;
                    yOrigin1 = y0 - y;
                    xDest1 = x0;
                    yDest1 = y0 - y;
                    xOrigin2 = x0 + y;
                    yOrigin2 = y0 - x;
                    xDest2 = x0;
                    yDest2 = y0 - x;
                    //SDL_RenderDrawLine(renderer, x0 + x, y0 - y, x0 , y0 - y);
                    //SDL_RenderDrawLine(renderer, x0 + y, y0 - x, x0, y0 - x);
                    break;
                case CircleQuarter::BOTTOM_RIGHT :
                    xOrigin1 = x0 + x;
                    yOrigin1 = y0 + y;
                    xDest1 = x0;
                    yDest1 = y0 + y;
                    xOrigin2 = x0 + y;
                    yOrigin2 = y0 + x;
                    xDest2 = x0;
                    yDest2 = y0 + x;
                    //SDL_RenderDrawLine(renderer, x0 + x, y0 + y, x0 , y0 + y);
                    //SDL_RenderDrawLine(renderer, x0 + y, y0 + x, x0, y0 + x);
                    break;
                case CircleQuarter::BOTTOM_LEFT :
                    xOrigin1 = x0 - x;
                    yOrigin1 = y0 + y;
                    xDest1 = x0;
                    yDest1 = y0 + y;
                    xOrigin2 = x0 - y;
                    yOrigin2 = y0 + x;
                    xDest2 = x0;
                    yDest2 = y0 + x;
                    //SDL_RenderDrawLine(renderer, x0 - x, y0 + y, x0 , y0 + y);
                    //SDL_RenderDrawLine(renderer, x0 - y, y0 + x, x0, y0 + x);
                    break;
                case CircleQuarter::TOP_LEFT :
                    xOrigin1 = x0 - x;
                    yOrigin1 = y0 - y;
                    xDest1 = x0;
                    yDest1 = y0 - y;
                    xOrigin2 = x0 - y;
                    yOrigin2 = y0 - x;
                    xDest2 = x0;
                    yDest2 = y0 - x;
                    //SDL_RenderDrawLine(renderer, x0 - x, y0 - y, x0 , y0 - y);
                    //SDL_RenderDrawLine(renderer, x0 - y, y0 - x, x0, y0 - x);
                    break;
            }

            //We only draw lines one time per y
            if(donePixels.find(yOrigin1) == donePixels.end())
            {
                SDL_RenderDrawLine(renderer, xOrigin1, yOrigin1, xDest1 , yDest1);
                donePixels.insert(yOrigin1);
            }
            if(donePixels.find(yOrigin2) == donePixels.end())
            {
                SDL_RenderDrawLine(renderer, xOrigin2, yOrigin2, xDest2 , yDest2);
                donePixels.insert(yOrigin2);
            }


    	} while (x < y);

    }
    return this;
}

CircleQuarter QuarterCircle::getQuarter() const
{
    return this->m_quarter;
}

QuarterCircle* QuarterCircle::setQuarter(const CircleQuarter& quarter)
{
    this->m_quarter = quarter;
    return this;
}

/***************************
 * Classe RoundedRectangle *
 ***************************/

 //Used to draw arectangle with rounded corners
 //It is in fact 3 Rectangles and 4 QuarterCircles joined together

 //Init based on the desired Color, Size and Position
 // Danger : Do not use Color transparency as it is not well managed by the quarter circle shape and there are some overlaping parts
RoundedRectangle::RoundedRectangle(SDL_Renderer* renderer, Color* color, const int& width, const int& height, const int& posX, const int& posY, const RoundedAngles& roundedAngles, const int& slopePercentage)
: Surface(renderer, color, width, height, posX, posY),
rect1(renderer, color, 0, 0, 0, 0),
rect2(renderer, color, 0, 0, 0, 0),
rect3(renderer, color, 0, 0, 0, 0),
corn1(renderer, color, 0, 0, 0, CircleQuarter::TOP_RIGHT),
corn2(renderer, color, 0, 0, 0, CircleQuarter::BOTTOM_RIGHT),
corn3(renderer, color, 0, 0, 0, CircleQuarter::BOTTOM_LEFT),
corn4(renderer, color, 0, 0, 0, CircleQuarter::TOP_LEFT)
{
    this->setRoundedAngles(roundedAngles);
    this->setSlopePercentage(slopePercentage);
    this->setSize(width, height);
    return;
}

RoundedRectangle* RoundedRectangle::render()
{
    this->rect1.render();
    this->rect2.render();
    this->rect3.render();
    this->corn1.render();
    this->corn2.render();
    this->corn3.render();
    this->corn4.render();
    return this;
}

RoundedRectangle* RoundedRectangle::setSize(const int& width, const int& height)
{
    this->m_width = width;
    this->m_height = height;

    int calcBase;

    if(width > height)
    {
        calcBase = height;
    }
    else
    {
        calcBase = width;
    }

    const int angleSize = calcBase * this->getSlopePercentage() / 100 / 2;

    if(this->getRoundedAngles() == RoundedAngles::TOP_ANGLES || this->getRoundedAngles() == RoundedAngles::RIGHT_ANGLES || this->getRoundedAngles() == RoundedAngles::ALL_ANGLES)
    {
        this->corn1.setSize(angleSize, angleSize);
    }

    if(this->getRoundedAngles() == RoundedAngles::RIGHT_ANGLES || this->getRoundedAngles() == RoundedAngles::BOTTOM_ANGLES || this->getRoundedAngles() == RoundedAngles::ALL_ANGLES)
    {
        this->corn2.setSize(angleSize, angleSize);
    }

    if(this->getRoundedAngles() == RoundedAngles::BOTTOM_ANGLES || this->getRoundedAngles() == RoundedAngles::LEFT_ANGLES || this->getRoundedAngles() == RoundedAngles::ALL_ANGLES)
    {
        this->corn3.setSize(angleSize, angleSize);
    }

    if(this->getRoundedAngles() == RoundedAngles::LEFT_ANGLES || this->getRoundedAngles() == RoundedAngles::TOP_ANGLES || this->getRoundedAngles() == RoundedAngles::ALL_ANGLES)
    {
        this->corn4.setSize(angleSize, angleSize);
    }

    if(this->getRoundedAngles() == RoundedAngles::TOP_ANGLES)
    {
        this->rect1.setSize(angleSize + 1, height - angleSize);
        this->rect2.setSize(width - (2 * angleSize) - 2, height);
        this->rect3.setSize(angleSize + 1, height - angleSize);
    }
    else if(this->getRoundedAngles() == RoundedAngles::RIGHT_ANGLES)
    {
        this->rect1.setSize(0, 0);
        this->rect2.setSize(width - angleSize - 1, height);
        this->rect3.setSize(angleSize + 1, height - (2 * angleSize));
    }
    else if(this->getRoundedAngles() == RoundedAngles::BOTTOM_ANGLES)
    {
        this->rect1.setSize(angleSize + 1, height - angleSize);
        this->rect2.setSize(width - (2 * angleSize) - 2, height);
        this->rect3.setSize(angleSize + 1, height - angleSize);
    }
    else if(this->getRoundedAngles() == RoundedAngles::LEFT_ANGLES)
    {
        this->rect1.setSize(angleSize + 1, height - (2 * angleSize));
        this->rect2.setSize(width - angleSize - 1, height);
        this->rect3.setSize(0, 0);
    }
    else if(this->getRoundedAngles() == RoundedAngles::ALL_ANGLES)
    {
        this->rect1.setSize(angleSize + 1, height - (2 * angleSize));
        this->rect2.setSize(width - (2 * angleSize) - 2, height);
        this->rect3.setSize(angleSize + 1, height - (2 * angleSize));
    }

    this->setPosition(this->getPosX(), this->getPosY());
    return this;
}

RoundedRectangle* RoundedRectangle::setPosition(const int& posX, const int& posY)
{
    int width = this->getWidth(), height = this->getHeight();

    int calcBase;

    if(width > height)
    {
        calcBase = height;
    }
    else
    {
        calcBase = width;
    }

    const int angleSize = calcBase * this->getSlopePercentage() / 100 / 2;


    if(this->getRoundedAngles() == RoundedAngles::TOP_ANGLES || this->getRoundedAngles() == RoundedAngles::RIGHT_ANGLES || this->getRoundedAngles() == RoundedAngles::ALL_ANGLES)
    {
        this->corn1.setPosition(posX + width - angleSize - 1, posY);
    }

    if(this->getRoundedAngles() == RoundedAngles::RIGHT_ANGLES || this->getRoundedAngles() == RoundedAngles::BOTTOM_ANGLES || this->getRoundedAngles() == RoundedAngles::ALL_ANGLES)
    {
        this->corn2.setPosition(posX + width - angleSize - 1, posY + height - angleSize - 1);
    }

    if(this->getRoundedAngles() == RoundedAngles::BOTTOM_ANGLES || this->getRoundedAngles() == RoundedAngles::LEFT_ANGLES || this->getRoundedAngles() == RoundedAngles::ALL_ANGLES)
    {
        this->corn3.setPosition(posX, posY + height - angleSize - 1);
    }

    if(this->getRoundedAngles() == RoundedAngles::LEFT_ANGLES || this->getRoundedAngles() == RoundedAngles::TOP_ANGLES || this->getRoundedAngles() == RoundedAngles::ALL_ANGLES)
    {
        this->corn4.setPosition(posX, posY);
    }

    if(this->getRoundedAngles() == RoundedAngles::TOP_ANGLES)
    {
        this->rect1.setPosition(posX, posY + angleSize);
        this->rect2.setPosition(posX + angleSize + 1, posY);
        this->rect3.setPosition(posX + width - angleSize - 1, posY + angleSize);
    }
    else if(this->getRoundedAngles() == RoundedAngles::RIGHT_ANGLES)
    {
        this->rect2.setPosition(posX, posY);
        this->rect3.setPosition(posX + width - angleSize - 1, posY + angleSize);
    }
    else if(this->getRoundedAngles() == RoundedAngles::BOTTOM_ANGLES)
    {
        this->rect1.setPosition(posX, posY);
        this->rect2.setPosition(posX + angleSize + 1, posY);
        this->rect3.setPosition(posX + width - angleSize - 1, posY);
    }
    else if(this->getRoundedAngles() == RoundedAngles::LEFT_ANGLES)
    {
        this->rect1.setPosition(posX, posY + angleSize);
        this->rect2.setPosition(posX + angleSize + 1, posY);
    }
    else if(this->getRoundedAngles() == RoundedAngles::ALL_ANGLES)
    {
        this->rect1.setPosition(posX, posY + angleSize);
        this->rect2.setPosition(posX + angleSize + 1, posY);
        this->rect3.setPosition(posX + width - angleSize - 1, posY + angleSize);
    }

    return this;
}

RoundedRectangle* RoundedRectangle::setColor(Color* color)
{
    Surface::setColor(color);
    this->rect1.setColor(color);
    this->rect2.setColor(color);
    this->rect3.setColor(color);
    this->corn1.setColor(color);
    this->corn2.setColor(color);
    this->corn3.setColor(color);
    this->corn4.setColor(color);
    return this;
}

RoundedRectangle* RoundedRectangle::setRoundedAngles(const RoundedAngles& roundedAngles)
{
    this->m_roundedAngles = roundedAngles;
    return this;
}

RoundedAngles RoundedRectangle::getRoundedAngles() const
{
    return this->m_roundedAngles;
}

RoundedRectangle* RoundedRectangle::setSlopePercentage(const int& slopePercentage)
{
    this->m_slopePercentage = slopePercentage;
    return this;
}

int RoundedRectangle::getSlopePercentage() const
{
    return this->m_slopePercentage;
}
