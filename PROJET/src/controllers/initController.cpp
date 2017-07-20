#include "initController.hpp"

#include "controller.hpp"
#include "inputController.hpp"
#include "gameLaunchController.hpp"
#include "homeController.hpp"
#include "gameSelectController.hpp"
#include "inputConfiguratorController.hpp"
#include "diskController.hpp"
#include "firstLaunchController.hpp"
#include "osController.hpp"
#include "wifiController.hpp"
#include "errorController.hpp"
#include "visualKeyboardController.hpp"
#include "menuController.hpp"
#include "popupWindowController.hpp"
#include "inputPopupWindowController.hpp"

#include "../views/view.hpp"
#include "../views/abstractView.hpp"
#include "../views/homeView.hpp"
#include "../views/visualKeyboardView.hpp"
#include "../views/menuView.hpp"
#include "../views/errorview.hpp"
#include "../views/gameSelectView.hpp"
#include "../views/gamepadProfileSelectView.hpp"
#include "../views/firstLaunchView.hpp"
#include "../views/popupWindowView.hpp"
#include "../views/startupLoadingView.hpp"

#include <iostream>

using namespace std;

InitController::InitController(Controller* controller)
: AbstractController(controller, INIT_CONTROLLER)
{
    //Load elements
    View* view = new View(controller, controller->getWindow());

    new StartupLoadingView(view);

    this->incrementViewLoadProgress();

    if(this->getController()->getView()->getView(STARTUP_LOADING_VIEW) != nullptr)
    {
        this->getController()->getView()->getView(STARTUP_LOADING_VIEW)->init();
        this->incrementViewInitProgress();
    }
    else
    {
        //TODO error loading loading screen
    }

    //Errors
    new ErrorController(controller);
    this->incrementControllerLoadProgress();

    new MajorErrorView(view);
    this->incrementViewLoadProgress();

    new ExecutionErrorView(view);
    this->incrementViewLoadProgress();


    //Home
    new HomeController(controller);
    this->incrementControllerLoadProgress();

    new HomeView(view);
    this->incrementViewLoadProgress();


    //Game Select
    new GameSelectController(controller);
    this->incrementControllerLoadProgress();

    new GameSelectView(view);
    this->incrementViewLoadProgress();

    //First Launch
    new FirstLaunchController(controller);
    this->incrementControllerLoadProgress();

    new FirstLaunchView(view);
    this->incrementViewLoadProgress();

    //Menus
    new MenuController(controller);
    this->incrementControllerLoadProgress();

    new MenuView(view);
    this->incrementViewLoadProgress();


    //Inputs
    new InputController(controller);
    this->incrementControllerLoadProgress();

    new InputConfiguratorController(controller);
    this->incrementControllerLoadProgress();

    new GamepadProfileSelectView(view);
    this->incrementViewLoadProgress();


    //PopupWindows
    new PopupWindowController(controller);
    this->incrementControllerLoadProgress();

    new InputPopupWindowController(controller);
    this->incrementControllerLoadProgress();

    new PopupWindowView(view);
    this->incrementViewLoadProgress();

    new InputPopupWindowView(view);
    this->incrementViewLoadProgress();

    new DiskController(controller);
    this->incrementControllerLoadProgress();


    //Visual Keyboard
    new VisualKeyboardController(controller);
    this->incrementControllerLoadProgress();

    new VisualKeyboardView(view);
    this->incrementViewLoadProgress();


    //MISC Controllers
    new OSController(controller);
    this->incrementControllerLoadProgress();

    new WifiController(controller);
    this->incrementControllerLoadProgress();

    new GameLaunchController(controller);
    this->incrementControllerLoadProgress();


    //Check integrity for controllers
    for(int i = ControllersID::HOME_CONTROLLER; i != ControllersID::LAST_CONTROLLER; ++i)
    {
        ControllersID controllerID = static_cast<ControllersID>(i);
        if(controller->getController(controllerID) == nullptr)
        {
            //TODO error component not found
            cout << "error could not load a controller" << endl;
        }
    }


    //Check integrity for views and init them

    for(int i = ViewsID::HOME_VIEW; i != ViewsID::LAST_VIEW; ++i)
    {
        if(i != ViewsID::STARTUP_LOADING_VIEW)
        {
            ViewsID viewID = static_cast<ViewsID>(i);
            if(view->getView(viewID) != nullptr)
            {
                view->getView(viewID)->init();
            }
            else
            {
                cout << "an error occured loading the views" << i << endl;
                //TODO error lacking component
            }
            this->incrementViewInitProgress();
        }
    }
}

InitController::~InitController()
{

}

InitController* InitController::control()
{
    AbstractController::control();
    return this;
}

InitController* InitController::inputControl()
{
    AbstractController::control();
    return this;
}

InitController* InitController::incrementControllerLoadProgress()
{
    this->m_controllerLoadprogress += 100.0 / (ControllersID::LAST_CONTROLLER - 1.0);
    this->setGlobalLoading();
    this->renderLoadingScreen();
    return this;
}

InitController* InitController::incrementViewLoadProgress()
{
    this->m_viewLoadProgress += 100.0 / (ViewsID::LAST_VIEW);
    this->setGlobalLoading();
    this->renderLoadingScreen();
    return this;
}

InitController* InitController::incrementViewInitProgress()
{
    this->m_viewInitProgress += 100.0 / (ViewsID::LAST_VIEW);
    this->setGlobalLoading();
    this->renderLoadingScreen();
    return this;
}

InitController* InitController::setGlobalLoading()
{
    int globalLoading = (this->m_controllerLoadprogress / 4) + (this->m_viewLoadProgress / 4) + (this->m_viewInitProgress / 2);

    if(globalLoading > 100)
    {
        globalLoading = 100;
    }

    this->getController()->viewData.loadingScreenPercentage = globalLoading;
    return this;
}

InitController* InitController::renderLoadingScreen()
{
    if(this->getController()->getView() != nullptr && this->getController()->getView()->getView(STARTUP_LOADING_VIEW) != nullptr)
    {
        this->getController()->getView()->getView(STARTUP_LOADING_VIEW)->render(this->getController()->viewData);
        this->getController()->getView()->renderAll();
    }

    return this;
}
