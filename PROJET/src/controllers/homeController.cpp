#include "homeController.hpp"
#include "menuController.hpp"
#include "../entities/log.hpp"

using namespace std;

//Controlleur de la page d'accueil de présentation des systèmes

HomeController::HomeController(Controller* controller)
: AbstractController(controller, ControllersID::HOME_CONTROLLER)
{
    Log::addEntry(1, "HomeController initialized successfully");

    return;
}

HomeController::~HomeController()
{

    Log::addEntry(1, "Deleted HomeController");
}

// Boucle de contrôle en fonction des inputs
HomeController* HomeController::control()
{
    AbstractController::control();
    return this;
}

HomeController* HomeController::inputControl()
{
    AbstractController::inputControl();
    for(auto& ent : this->getController()->getAdvancedControls())
    {
        if(this->getController()->getGamepadControl(ent.first, GamepadActions::RIGHT))
        {
            this->systemNext();
        }
        else if(this->getController()->getGamepadControl(ent.first, GamepadActions::DOWN))
        {
            this->systemNextRow();
        }
        else if(this->getController()->getGamepadControl(ent.first, GamepadActions::LEFT))
        {
            this->systemPrevious();
        }
        else if(this->getController()->getGamepadControl(ent.first, GamepadActions::UP))
        {
            this->systemPreviousRow();
        }
        else if(this->getController()->getGamepadControl(ent.first, GamepadActions::START))
        {
            MenuController* menuController = static_cast<MenuController*>(this->getController()->getController(MENU_CONTROLLER));
            if(menuController != nullptr)
            {
                menuController->isActive(true);
            }
        }
        else if((this->getController()->getGamepadControl(ent.first, GamepadActions::B)))
        {
            if(this->getController()->viewData.systems.count(this->getController()->viewData.selectedSystem) > 0)
            {
                if(this->getController()->viewData.systems.at(this->getController()->viewData.selectedSystem) != nullptr)
                {
                    if(this->getController()->viewData.systems.at(this->getController()->viewData.selectedSystem)->isAvailable())
                    {
                        this->getController()->setCurrentView(CurrentViews::GAME_SELECT);
                    }
                }
            }
        }
    }
    return this;
}

HomeController* HomeController::systemNext()
{
    if(this->getController()->viewData.selectedSystem + 1 != this->getController()->viewData.numberSystems)
    {
        ++ this->getController()->viewData.selectedSystem;
    }
    else{
        this->getController()->viewData.selectedSystem = 0;
    }
    return this;
}

HomeController* HomeController::systemPrevious()
{
    if(this->getController()->viewData.selectedSystem != 0)
    {
        -- this->getController()->viewData.selectedSystem;
    }
    else
    {
        this->getController()->viewData.selectedSystem = this->getController()->viewData.numberSystems - 1;
    }
    return this;
}

HomeController* HomeController::systemNextRow()
{
    if(this->getController()->viewData.selectedSystem + 3 <= this->getController()->viewData.numberSystems - 1)
    {
        this->getController()->viewData.selectedSystem = this->getController()->viewData.selectedSystem + 3;
    }
    else
    {
        if(this->getController()->viewData.selectedSystem % 3 == 0)
        {
            this->getController()->viewData.selectedSystem = 0;
        }
        else if(this->getController()->viewData.selectedSystem % 3 == 1)
        {
            this->getController()->viewData.selectedSystem = 1;
        }
        else if(this->getController()->viewData.selectedSystem % 3 == 2)
        {
            this->getController()->viewData.selectedSystem = 2;
        }
    }
    return this;
}

HomeController* HomeController::systemPreviousRow()
{
    if(this->getController()->viewData.selectedSystem - 3 >= 0)
    {
        this->getController()->viewData.selectedSystem = this->getController()->viewData.selectedSystem - 3;
    }
    else
    {
        int delta = (this->getController()->viewData.numberSystems - 1) % 3;

        if(delta == 2) // The last system is on the third column
        {
            switch (this->getController()->viewData.selectedSystem) {
                case 0 :
                    this->getController()->viewData.selectedSystem = this->getController()->viewData.numberSystems - 4;
                    break;
                case 1 :
                    this->getController()->viewData.selectedSystem = this->getController()->viewData.numberSystems - 2;
                    break;
                case 2 :
                    this->getController()->viewData.selectedSystem = this->getController()->viewData.numberSystems - 1;
                    break;
            }
        }
        else if(delta == 1) // The last system is on the second column
        {
            switch (this->getController()->viewData.selectedSystem) {
                case 0:
                    this->getController()->viewData.selectedSystem = this->getController()->viewData.numberSystems - 2;
                    break;
                case 1:
                    this->getController()->viewData.selectedSystem = this->getController()->viewData.numberSystems - 1;
                    break;
                case 2:
                    this->getController()->viewData.selectedSystem = this->getController()->viewData.numberSystems - 3;
                    break;
            }
        }
        else if(delta == 0) // The last system is on the first column
        {
            switch (this->getController()->viewData.selectedSystem) {
                case 0:
                    this->getController()->viewData.selectedSystem = this->getController()->viewData.numberSystems - 1;
                    break;
                case 1:
                    this->getController()->viewData.selectedSystem = this->getController()->viewData.numberSystems - 3;
                    break;
                case 2:
                    this->getController()->viewData.selectedSystem = this->getController()->viewData.numberSystems - 2;
                    break;
            }
        }

    }
    return this;
}
