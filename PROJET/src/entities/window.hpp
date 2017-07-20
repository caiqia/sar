#ifndef DEF_WINDOW
#define DEF_WINDOW

#include <SDL2/SDL.h>
#include <string>

class Window
{
public:
    Window();
    ~Window();
    void initWindow();

    int getHeight();
    void setHeight(int height);

    int getWidth();
    void setWidth(int width);

    int getDisplayRate();
    void setDisplayRate(int displayRate);

    SDL_Window* getWindow();
    void setWindow(SDL_Window* window);

    SDL_Renderer* getRenderer();
    void setRenderer(SDL_Renderer* renderer);

    bool getInitStatus();
    void setInitStatus(bool initStatus);

    void createWindow();

    void createRenderer();

    Uint32 getPixelFormat();
    void setPixelFormat(Uint32 pixelFormat);

    std::string getAspectRatio();
    void setAspectRatio(std::string aspectRatio);

    Window* deInitWindow();
    Window* reInitWindow();


protected:
    SDL_Window* m_window = NULL;
    SDL_Renderer* m_renderer = NULL;
    int m_height = 600;
    int m_width = 800;
    int m_displayRate = 50;
    bool m_initStatus = 0;
    Uint32 m_pixelFormat;
    std::string m_aspectRatio;
};

#endif
