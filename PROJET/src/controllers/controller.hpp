#ifndef DEF_CONTROLLER
#define DEF_CONTROLLER

#include <string>
#include <map>

#include "../entities/window.hpp"
#include "../entities/gamepad.hpp"
#include "viewData.hpp"

enum ControllersID
{
    HOME_CONTROLLER,
    GAME_SELECT_CONTROLLER,
    WIFI_CONTROLLER,
    INPUT_POPUP_WINDOW_CONTROLLER,
    INIT_CONTROLLER,
    DISK_CONTROLLER,
    INPUT_CONTROLLER,
    OS_CONTROLLER,
    MENU_CONTROLLER,
    GAME_LAUNCH_CONTROLLER,
    FIRST_LAUNCH_CONTROLLER,
    INPUT_CONFIGURATOR_CONTROLLER,
    POPUP_WINDOW_CONTROLLER,
    ERROR_CONTROLLER,
    VISUAL_KEYBOARD_CONTROLLER,
    LAST_CONTROLLER,
};

class View;

class AbstractController;

class Controller
{
public:
    Controller(Window* window, SDL_Event* eventListener, bool devMode);
    ~Controller();

    bool control();

    Controller * systemsToViewData();

    CurrentViews getCurrentView() const;
    Controller* setCurrentView(CurrentViews view);

    OverlayLevel getOverlayLevel();
    void setOverlayLevel(OverlayLevel overlayLevel);

    void clearOverlayLevel();

    Window* getWindow();
    void setWindow(Window* window);

    View* getView();
    Controller* setView(View* view);

    std::map<ControllersID, AbstractController*> getControllers() const;
    AbstractController* getController(const ControllersID& controllerID) const;
    Controller* addController(const ControllersID& controllerID, AbstractController* controller);

    int getEventLoopCounter();
    Controller* incrementEventLoopCounter();
    Controller* resetEventLoopCounter();

    bool getContinu();
    void stop();
    void reboot();

    std::map<GamepadActions, bool> getControls();
    bool getControl(GamepadActions gamepadAction);
    Controller* setControl(GamepadActions gamepadAction, bool status);

    std::map<Gamepad*, std::map<GamepadActions, bool>> getAdvancedControls();
    std::map<GamepadActions, bool> getGamepadControls(Gamepad* gamepad);
    bool getGamepadControl(Gamepad* gamepad, GamepadActions action);
    Controller* setGamepadControl(Gamepad* gamepad, GamepadActions action, bool active);

    ViewData getViewDataX();

    std::map< std::string, std::map<std::string, std::string> > getViewData();
    Controller* addViewData(std::string param1, std::map<std::string, std::string> param2);

    std::string getParam(std::string param);
    Controller* setParam(std::string param, std::string value);

    System* getSelectedSystem();

    std::map< std::string, std::map<std::string, std::string> > m_viewData;

    ViewData viewData;

    SDL_Event* getEventListener();
    Controller* setEventListener(SDL_Event* eventListener);

    Controller* setDevMode(bool devMode);
    bool getDevMode();

protected:
    CurrentViews m_currentView;
    OverlayLevel m_overlayLevel;

    Window* m_pWindow = nullptr;
    View* m_pView = nullptr;

    std::map<ControllersID, AbstractController*> m_pControllers;

    int m_eventLoopcounter = 0;

    bool m_continu = 1;
    std::map<GamepadActions, bool> m_controls;

    bool m_devMode = false;

    std::map<Gamepad*, std::map<GamepadActions, bool>> m_advancedControls;
    static std::map<std::string, OverlayLevel>OverlayLevelResolver;

    SDL_Event* m_pEventListener;
};

#endif
