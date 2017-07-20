#include <iostream>
#include <string>
#include <SDL2/SDL_image.h>

#include "image.hpp"
#include "../entities/log.hpp"
#include "../entities/error.hpp"
#include "../entities/errorexception.hpp"

using namespace std;

//Image Object to manipulate and show images from files
//Inherits from Renderable

//Init
Image::Image(SDL_Renderer* renderer, std::string file) : Renderable(renderer)
{
    //A SDL surface is created from the file
    SDL_Surface* pSprite = IMG_Load(file.c_str());
    if(pSprite)
    {
        this->setRenderer(renderer);
        //The surface is transformed into a SDL texture and then deleted
        this->setTexture(SDL_CreateTextureFromSurface(renderer, pSprite));
        SDL_FreeSurface(pSprite);

        //If the texture was not created correctly
        if(this->getTexture() == NULL)
        {
            string m_error = SDL_GetError();
            Log::addEntry(1, "/!\\ Could not transform image into texture from : " + file + "\n -> " + m_error);

        }
        else
        {
            //By default the image takes its original size from the file and is positionned on the top left corner
            int w, h;
            SDL_QueryTexture(this->getTexture(), NULL, NULL, &w, &h);
            this->setOrigialWidth(w);
            this->setOriginalHeight(h);
            this->setSize(w, h);
            this->setPosition(0, 0);
            Log::addEntry(3, "Image fully loaded : " + file);
        }
    }
    //If we could not load correctly the file
    else
    {
     throw Errorexception(ErrorType::MAJOR, "Could not load image from : " + file, "error code : 1000");
        //Log::addEntry(1, "/!\\ Could not load image from : " + file);

    }
    return;
}

//Init where size and position are set
Image::Image(SDL_Renderer* renderer, std::string file, int width, int height, int posX, int posY) : Image(renderer, file)
{
    this->setSize(width, height);
    this->setPosition(posX, posY);
    return;
}

//Init which adds a transparency level for the image
Image::Image(SDL_Renderer* renderer, std::string file, int width, int height, int posX, int posY, int transparency) : Image(renderer, file, width, height, posX, posY)
{
    this->setTransparency(transparency);
    return;
}

//Image is freed from RAM on object deletion
Image::~Image()
{
    SDL_DestroyTexture(this->getTexture());
}

//Render function for the image
//Displays the image
Image* Image::render()
{
    Renderable::render();
    SDL_RenderCopy(this->getRenderer(), this->getTexture(), &this->m_srcr, &this->m_rect);

    return this;
}

SDL_Texture* Image::getTexture()
{
    return this->m_pTexture;
}

Image* Image::setTexture(SDL_Texture* texture)
{
    this->m_pTexture = texture;
    return this;
}

SDL_Rect Image::getRect()
{
    return this->m_rect;
}

Image* Image::setSize(int width, int height, ResizeMode resizeMode)
{

    int finalWidth = this->m_rect.w;
    int finalHeight = this->m_rect.h;
    int originalHeight = this->getOriginalHeight();
    int originalWidth = this->getOriginalWidth();
    double widthHeightRatio = (double)width/(double)height;
    double originWidthHeightRatio = (double)originalWidth/(double)originalHeight;

    switch (resizeMode) {
        case FORCE :
        this->m_srcr.x = 0;
        this->m_srcr.y = 0;
        this->m_srcr.w = this->getOriginalWidth();
        this->m_srcr.h = this->getOriginalHeight();


            finalWidth = width;
            finalHeight = height;

            break;

        case FIT :
        this->m_srcr.x = 0;
        this->m_srcr.y = 0;
        this->m_srcr.w = this->getOriginalWidth();
        this->m_srcr.h = this->getOriginalHeight();
            if(originWidthHeightRatio < widthHeightRatio)
            {
                finalHeight = height;
                finalWidth = round(height * originWidthHeightRatio);
            }
            else
            {
                finalWidth = width;
                finalHeight = round(width / originWidthHeightRatio);
            }

            break;

        case CROP:


        if(originWidthHeightRatio < widthHeightRatio)
        {
            this->m_srcr.x = this->m_rect.x;
            this->m_srcr.y = this->m_rect.y+(this->getOriginalHeight()-height)/2;
            this->m_srcr.w = this->getOriginalWidth();
            this->m_srcr.h = this->getOriginalWidth()/widthHeightRatio;
        }
        else
        {
            this->m_srcr.x = this->m_rect.x+(this->getOriginalWidth()-width)/2;
            this->m_srcr.y = this->m_rect.y;
            this->m_srcr.w = this->getOriginalHeight()*widthHeightRatio;
            this->m_srcr.h = this->getOriginalHeight();
        }

            break;
    }
    this->m_rect.w = finalWidth;
    this->m_rect.h = finalHeight;

    return this;
}

Image* Image::setPosition(int posX, int posY)
{
    this->m_rect.x = posX;
    this->m_rect.y = posY;
    return this;
}

int Image::getWidth() const
{
    return this->m_rect.w;
}

int Image::getHeight() const
{
    return this->m_rect.h;
}

Image* Image::center(int x, int xMax, int y, int yMax)
{
    int posX, posY;
    posX = x + ((xMax - x) / 2) - (this->getWidth() / 2);
    posY = y + ((yMax - y) / 2) - (this->getHeight() / 2);
    this->setPosition(posX, posY);
    return this;
}

Image* Image::setTransparency(int transparency)
{
    SDL_SetTextureBlendMode(this->getTexture(), SDL_BLENDMODE_BLEND);
    SDL_SetTextureAlphaMod(this->getTexture(), transparency);
    return this;
}

int Image::getOriginalWidth() const
{
    return this->m_originalWidth;
}

Image* Image::setOrigialWidth(int originalWidth)
{
    this->m_originalWidth = originalWidth;
    return this;
}

int Image::getOriginalHeight() const
{
    return this->m_originalHeight;
}

Image* Image::setOriginalHeight(int originalHeight)
{
    this->m_originalHeight = originalHeight;
    return this;
}
