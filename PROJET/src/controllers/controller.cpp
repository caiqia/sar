
#include "controller.hpp"
#include "inputController.hpp"
#include "gameLaunchController.hpp"
#include "inputConfiguratorController.hpp"
#include "errorController.hpp"
#include "diskController.hpp"
#include "firstLaunchController.hpp"
#include "wifiController.hpp"
#include "osController.hpp"
#include "visualKeyboardController.hpp"
#include "menuController.hpp"
#include "popupWindowController.hpp"
#include "inputPopupWindowController.hpp"
#include "initController.hpp"
#include "../entities/gamepad.hpp"
#include "../entities/log.hpp"
#include "../entities/error.hpp"
#include "../entities/system.hpp"
#include "../views/view.hpp"

using namespace std;

// This is the main controller who's aim is to route the querries and send the data to the view

//Basic init
Controller::Controller(Window* window, SDL_Event* eventListener, bool devMode)
{
    this->setDevMode(devMode);
    this->setWindow(window);
    this->setEventListener(eventListener);
    this->setCurrentView(CurrentViews::HOME);

    this->systemsToViewData();

    new InitController(this);

    Log::addEntry(1, "Controller initialized successfully");
}

Controller::~Controller()
{
    Log::addEntry(1, "Deleted Controller");
}

// Main control loop
bool Controller::control()
{
    //Event loop
    this->resetEventLoopCounter();
    while(SDL_PollEvent(this->getEventListener()))
    {
        if(this->getController(INPUT_CONTROLLER) != nullptr)
        {
            this->getController(INPUT_CONTROLLER)->eventControl(this->getEventListener());
        }

        this->incrementEventLoopCounter();
    }

    this->clearOverlayLevel();

    for(auto& ent : this->getControllers())
    {
        if(ent.second != nullptr)
        {
            ent.second->control();
        }
    }

    //We go through all the gamepads and check their inputs
    for(auto& ent : this->getAdvancedControls())
    {
        if(this->getDevMode() && this->getGamepadControl(ent.first, GamepadActions::START) && this->getGamepadControl(ent.first, GamepadActions::MAGIC))
        {
            this->stop(); //Stops the program if both start and magic button are pressed (SELECT in most cases)
        }
    }

    switch (this->getOverlayLevel())
    {
        case OverlayLevel::MENU :
        {
            if(this->getController(MENU_CONTROLLER) != nullptr)
            {
                this->getController(MENU_CONTROLLER)->inputControl();
            }
            break;
        }

        case OverlayLevel::GAMEPAD_PROFILE_SELECTION :
        {
            if(this->getController(INPUT_CONFIGURATOR_CONTROLLER) != nullptr)
            {
                this->getController(INPUT_CONFIGURATOR_CONTROLLER)->inputControl();
            }
            break;
        }

        case OverlayLevel::ERROR :
        {
            if(this->getController(VISUAL_KEYBOARD_CONTROLLER) != nullptr)
            {
                this->getController(ERROR_CONTROLLER)->inputControl();
            }
            break;
        }

        case OverlayLevel::VISUAL_KEYBOARD :
        {
            if(this->getController(VISUAL_KEYBOARD_CONTROLLER) != nullptr)
            {
                this->getController(VISUAL_KEYBOARD_CONTROLLER)->inputControl();
            }
            break;
        }

        case OverlayLevel::POPUP_WINDOW :
        {
            if(this->getController(POPUP_WINDOW_CONTROLLER) != nullptr)
            {
                this->getController(POPUP_WINDOW_CONTROLLER)->inputControl();
            }
            break;
        }

        case OverlayLevel::INPUT_POPUP_WINDOW :
        {
            if(this->getController(INPUT_POPUP_WINDOW_CONTROLLER) != nullptr)
            {
                this->getController(INPUT_POPUP_WINDOW_CONTROLLER)->inputControl();
            }
            break;
        }

        case OverlayLevel::NORMAL :
        {
            FirstLaunchController* firstLaunchController = static_cast<FirstLaunchController*>(this->getController(FIRST_LAUNCH_CONTROLLER));
            if(firstLaunchController != nullptr && !firstLaunchController->isFirstLaunch())
            {
                switch (this->getCurrentView()) //Calls the controller who's view is the current
                {
                    case CurrentViews::HOME :
                    {
                        if(this->getController(ControllersID::HOME_CONTROLLER) != nullptr)
                        {
                            this->getController(ControllersID::HOME_CONTROLLER)->inputControl();
                        }
                        break;
                    }

                    case CurrentViews::GAME_SELECT :
                    {
                        if(this->getController(ControllersID::GAME_SELECT_CONTROLLER) != nullptr)
                        {
                            this->getController(ControllersID::GAME_SELECT_CONTROLLER)->inputControl();
                        }
                        break;
                    }
                }
            }
            else
            {
                if(this->getController(FIRST_LAUNCH_CONTROLLER) != nullptr)
                {
                    this->getController(FIRST_LAUNCH_CONTROLLER)->inputControl();
                }
            }
            break;
        }
    }

    return this->getContinu();
}

// Inits the parameters for the view based on the config for systems
Controller* Controller::systemsToViewData()
{
    this->viewData.systems.clear();
    this->viewData.selectedGames.clear();
    this->viewData.numberSystems = 0;

    int numberSystems;

    //Reads all instances of System that have been created
    for(const auto & ent : System::instances)
     {
         this->viewData.systems.insert(pair<int, const System*> (ent.first, ent.second));
         this->viewData.selectedGames.insert(pair<int, int> (ent.first, 0));
         ++ this->viewData.numberSystems;
     }

    return this;
}

CurrentViews Controller::getCurrentView() const
{
    return this->viewData.currentView;
}

Controller* Controller::setCurrentView(CurrentViews view)
{
    this->viewData.currentView = view;
    return this;
}

OverlayLevel Controller::getOverlayLevel()
{
   return this->m_overlayLevel;
}

void Controller::setOverlayLevel(OverlayLevel overlayLevel)
{
  if(this->m_overlayLevel <= overlayLevel)
  {
    this->m_overlayLevel = overlayLevel;
    this->viewData.overlayLevel = overlayLevel;
  }
}

void Controller::clearOverlayLevel()
{
  this->m_overlayLevel = OverlayLevel::NORMAL;
  this->viewData.overlayLevel = OverlayLevel::NORMAL;
  this->setParam("hasOverlay", "false");
}


Window* Controller::getWindow()
{
    return this->m_pWindow;
}

void Controller::setWindow(Window* window)
{
    this->m_pWindow = window;
}

View* Controller::getView()
{
    return this->m_pView;
}

Controller* Controller::setView(View* view)
{
    this->m_pView = view;
    return this;
}

map<ControllersID, AbstractController*> Controller::getControllers() const
{
    return this->m_pControllers;
}

AbstractController* Controller::getController(const ControllersID& controllerID) const
{
    if(this->m_pControllers.count(controllerID) > 0)
    {
        return this->m_pControllers.at(controllerID);
    }
    return nullptr;
}

Controller* Controller::addController(const ControllersID& controllerID, AbstractController* controller)
{
    if(this->m_pControllers.count(controllerID) > 0)
    {
        if(this->m_pControllers.at(controllerID) != nullptr)
        {
            delete this->m_pControllers.at(controllerID);
        }
        this->m_pControllers.erase(controllerID);
    }
    this->m_pControllers.insert(pair<ControllersID, AbstractController*> (controllerID, controller));
    return this;
}

int Controller::getEventLoopCounter()
{
    return this->m_eventLoopcounter;
}

Controller* Controller::incrementEventLoopCounter()
{
    this->m_eventLoopcounter ++;
    return this;
}
Controller* Controller::resetEventLoopCounter()
{
    this->m_eventLoopcounter = 0;
    return this;
}

bool Controller::getContinu()
{
    return this->m_continu;
}

void Controller::stop()
{
    this->m_continu = 0;
    return;
}

void Controller::reboot()
{
    OSController* osController = static_cast<OSController*>(this->getController(OS_CONTROLLER));
    if(osController != nullptr)
    {
        osController->reboot();
    }
    return;
}

map<GamepadActions, bool> Controller::getControls()
{
    return this->m_controls;
}

bool Controller::getControl(GamepadActions gamepadAction)
{
    return this->m_controls[gamepadAction];
}

Controller* Controller::setControl(GamepadActions gamepadAction, bool status)
{
    if(this->m_controls.find(gamepadAction) != this->m_controls.end())
        this->m_controls.erase(gamepadAction);

    this->m_controls.insert(pair<GamepadActions, bool>(gamepadAction, status));
    return this;
}

map<Gamepad*, map<GamepadActions, bool>> Controller::getAdvancedControls()
{
    return this->m_advancedControls;
}

map<GamepadActions, bool> Controller::getGamepadControls(Gamepad* gamepad)
{
    return this->m_advancedControls[gamepad];
}

bool Controller::getGamepadControl(Gamepad* gamepad, GamepadActions action)
{
    return this->m_advancedControls[gamepad][action];
}

Controller* Controller::setGamepadControl(Gamepad* gamepad, GamepadActions action, bool active)
{
    if(this->m_advancedControls[gamepad].find(action) != this->m_advancedControls[gamepad].end())
    {
        this->m_advancedControls[gamepad].erase(action);
    }
    this->m_advancedControls[gamepad].insert(pair<GamepadActions, bool>(action, active));
    return this;
}

map<string, map<string, string> > Controller::getViewData()
{
    return this->m_viewData;
}

Controller* Controller::addViewData(string param1, map<string, string> param2)
{
    this->m_viewData.insert(pair<string, map<string, string>> (param1, param2));
    return this;
}

string Controller::getParam(string param)
{
    return this->m_viewData["param"][param];
}

Controller* Controller::setParam(string param, string value)
{
    if(this->m_viewData["param"].count(param) != 0)
    {
        this->m_viewData["param"].erase(param);
    }

    this->m_viewData["param"].insert(pair<string, string> (param, value));
    return this;
}

System* Controller::getSelectedSystem()
{
    if(System::instances.count(this->viewData.selectedSystem) > 0)
        return System::instances[this->viewData.selectedSystem];

    else
        return nullptr;
}

SDL_Event* Controller::getEventListener()
{
    return this->m_pEventListener;
}

Controller* Controller::setEventListener(SDL_Event* eventListener)
{
    this->m_pEventListener = eventListener;
    return this;
}

Controller* Controller::setDevMode(bool devMode)
{
    this->m_devMode = devMode;
    return this;
}

bool Controller::getDevMode()
{
    return this->m_devMode;
}
