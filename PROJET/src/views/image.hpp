#ifndef DEF_IMAGE
#define DEF_IMAGE

#include <SDL2/SDL.h>
#include <string>
#include "surface.hpp"

enum ResizeMode
{
    FORCE,
    FIT,
    CROP,
};

class Image : public Renderable
{
public:
    Image(SDL_Renderer* renderer, std::string file);
    Image(SDL_Renderer* renderer, std::string file, int width, int height, int posX, int posY);
    Image(SDL_Renderer* renderer, std::string file, int width, int height, int posX, int posY, int transparency);
    ~Image();

    virtual Image* render();

    SDL_Texture* getTexture();
    Image* setTexture(SDL_Texture* texture);

    SDL_Rect getRect();

    virtual Image* setSize(int width, int height, ResizeMode resizeMode = ResizeMode::FORCE);
    virtual Image* setPosition(int posX, int posY);

    int getWidth() const;
    int getHeight() const;

    Image* center(int x0, int xMax0, int y0, int yMax0);

    Image* setTransparency(int transparency);

    int getOriginalWidth() const;
    Image* setOrigialWidth(int originalWidth);

    int getOriginalHeight() const;
    Image* setOriginalHeight(int originalHeight);

protected:
    SDL_Texture* m_pTexture;
    SDL_Rect m_rect;
    SDL_Rect m_srcr;
    int m_originalWidth;
    int m_originalHeight;

};

#endif
