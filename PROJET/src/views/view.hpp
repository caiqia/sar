#ifndef DEF_VIEW
#define DEF_VIEW

#include <vector>

#include "surface.hpp"
#include "image.hpp"

class AbstractView;
class Controller;
class Window;
class GameSelectView;
class GamepadProfileSelectView;
class FirstLaunchView;


enum ViewsID
{
    HOME_VIEW,
    GAME_SELECT_VIEW,
    GAMEPAD_PROFILE_SELECT_VIEW,
    MAJOR_ERROR_VIEW,
    EXECUTION_ERROR_VIEW,
    VISUAL_KEYBOARD_VIEW,
    MENU_VIEW,
    POPUP_WINDOW_VIEW,
    INPUT_POPUP_WINDOW_VIEW,
    STARTUP_LOADING_VIEW,
    FIRST_LAUCH_VIEW,
    LAST_VIEW,
};

//Classe View, objet gérant le routage vers la bonen vue et le rendu

class View
{
public:
    View(Controller* controller, Window* window);
    ~View();

    void render();

    Controller* getController();
    void setController(Controller* controller);

    Window* getWindow();
    void setWindow(Window* window);

    View* addView(const ViewsID& viewID, AbstractView* view);
    AbstractView* getView(const ViewsID& viewID) const;
    std::map <ViewsID, AbstractView*> getViews() const;

    FirstLaunchView* getFirstLaunchView();
    View* setFirstLaunchView(FirstLaunchView* firstLaunchView);

    SDL_Renderer* getRenderer();
    void setRenderer(SDL_Renderer* renderer);

    void addToRender(Renderable* renderable);
    void renderAll();

    Image* getBackground(const int& system) const;
    std::map<int, Image*> getBackgrounds() const;
    View* addBackground(const int& system, const std::string& file);

    Image* getLogo(const int& system) const;
    std::map<int, Image*> getLogos() const;
    View* addLogo(const int& system, const std::string& file);

    Image* getGreyedLogo(const int& system) const;
    std::map<int, Image*> getGreyedLogos() const;
    View* addGreyedLogo(const int& system, const std::string& file);

    View* loadLogos();

    View* deInitWindow();
    View* reInitWindow();

    View* setOverlayGrey();
    Rectangle* getOverlayGrey();

    int getUsableWidth();
    int getUsableHeight();
    int getUsableXOrig();
    int getUsableYOrig();

    View* setUsableSpace();

private:
    Controller* m_pController;

    Window* m_pWindow;
    SDL_Renderer* m_pRenderer;



    int m_usableWidth;
    int m_usableHeight;
    int m_usableXOrigin;
    int m_usableYOrigin;

    std::map <ViewsID, AbstractView*> m_views;

    FirstLaunchView* m_pFirstLaunchView = nullptr;


    std::vector<Renderable*> m_toRender;

    std::map<int, Image*> m_backgrounds;
    std::map<int, Image*> m_logos;
    std::map<int, Image*> m_greyedLogos;

    Rectangle* m_overlayGrey;
};

#endif
