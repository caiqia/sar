#include <iostream>
#include <cmath>
#include "window.hpp"
#include "log.hpp"
#include "error.hpp"

using namespace std;

//This class is the window class used to manade the Window and Renderer from SDL

Window::Window()
{
}

Window::~Window()
{
    SDL_DestroyRenderer(this->getRenderer());
    SDL_DestroyWindow(this->getWindow());
    return;
}

//Function used to create the window and the corresponding renderer
void Window::initWindow()
{

    //Used to store information about the screen from SDL
    SDL_DisplayMode current;

    //We go through all of the screens connected
    //TODO Manage to allow the user to choose in config from HDMI or AV; not very important
    for(int i = 0; i < 1/*SDL_GetNumVideoDisplays()*/; ++i)
    {
        int shouldBeZero = SDL_GetCurrentDisplayMode(i, &current);

        //If we could not get the information on the current screen
        if(shouldBeZero != 0)
        {
          
          Log::addEntry(1, "/!\\ Could not get display mode for display #" + to_string(i) + "\n-> " + SDL_GetError());
        }
        else
        {
          Log::addEntry(1, "Display #" + to_string(i) + " current display mode is " + to_string(current.w) + "x" + to_string(current.h) + " @ " + to_string(current.refresh_rate) + "hz");

            //We store the information in our object, based on SDL's info
            this->setHeight(current.h);
            this->setWidth(current.w);
            this->setDisplayRate(current.refresh_rate);
            this->setPixelFormat(current.format);

            //We check if the screen if 4/3 of 16/9
            //It's not absolute so we get the closest one
            //There is probably a better way to do it taking into account the pixel aspect ratio
            double d43 = 4./3.;
            double d169 = 16./9.;
            double screenRatio = (double) this->getWidth() / (double) this->getHeight();
            double d43Dif = std::abs(screenRatio - d43);
            double d169Dif = std::abs(screenRatio - d169);

            if(d43Dif < d169Dif)
            {
                this->setAspectRatio("4/3");
            }
            else
            {
                this->setAspectRatio("16/9");
            }

            //We create the window
            this->createWindow();

            //If we could not initiate the window
            if(!this->getWindow())
            {
                std::string m_error = SDL_GetError();

                Log::addEntry(1, "/!\\ Window could not be created !\n-> " + m_error);
            }
            //If everything went fine
            else
            {
                //We create the renderer and if everything went fine set the whole as inited
                this->createRenderer();
                if(!this->getRenderer())
                {
                    std::string m_error = SDL_GetError();

                    Log::addEntry(1, "/!\\ Renderer could not be created !\n-> " + m_error);
                }
                else{
                    this->setInitStatus(1);
                }
            }
        }

    }
    return;
}

int Window::getHeight()
{
    return this->m_height;
}

void Window::setHeight(int height)
{
    this->m_height = height;
    return;
}

int Window::getWidth()
{
    return this->m_width;
}

void Window::setWidth(int width)
{
    this->m_width = width;
    return;
}

int Window::getDisplayRate()
{
    return this->m_displayRate;
}

void Window::setDisplayRate(int displayRate)
{
    this->m_displayRate = displayRate;
    return;
}

SDL_Window* Window::getWindow()
{
    return this->m_window;
}

void Window::setWindow(SDL_Window* window)
{
    this->m_window = window;
    return;
}

SDL_Renderer* Window::getRenderer()
{
    return this->m_renderer;
}

void Window::setRenderer(SDL_Renderer* renderer)
{
    this -> m_renderer = renderer;
    return;
}

bool Window::getInitStatus()
{
    return this->m_initStatus;
}

void Window::setInitStatus(bool initStatus)
{
    this->m_initStatus = initStatus;
    return;
}

void Window::createWindow()
{
    this->setWindow(SDL_CreateWindow("NostalFront", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, this->getWidth(), this->getHeight(), SDL_WINDOW_FULLSCREEN));
    return;
}

void Window::createRenderer()
{
    this->setRenderer(SDL_CreateRenderer(this->getWindow(),-1,SDL_RENDERER_ACCELERATED));
}

Uint32 Window::getPixelFormat()
{
    return this->m_pixelFormat;
}

void Window::setPixelFormat(Uint32 pixelFormat)
{
    this->m_pixelFormat = pixelFormat;
    return;
}

std::string Window::getAspectRatio()
{
    return this->m_aspectRatio;
}

void Window::setAspectRatio(std::string aspectRatio)
{
    this->m_aspectRatio = aspectRatio;
    return;
}

//Function used to destroy an allready initiated window (when launching a game for example)
Window* Window::deInitWindow()
{
    SDL_DestroyRenderer(this->getRenderer());
    this->setRenderer(nullptr);
    SDL_DestroyWindow(this->getWindow());
    this->setWindow(nullptr);
    this->setInitStatus(0);
    return this;
}

//function used to reCreate a previoslu existing window which was destroyed thanks to deInitWindow()
Window* Window::reInitWindow()
{
    this->createWindow();
    this->createRenderer();
    this->setInitStatus(1);
    return this;
}
