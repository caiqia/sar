#include "menuController.hpp"
#include "menus/abstractMenuController.hpp"
#include "menus/mainMenuController.hpp"
#include "menus/parametersMenuController.hpp"
#include "menus/informationMenuController.hpp"
#include "menus/networkMenuController.hpp"
#include "menus/availableWifiMenuController.hpp"
#include <iostream>

using namespace std;

MenuController::MenuController(Controller* controller)
: AbstractController(controller, MENU_CONTROLLER)
{
    this->setCurrentMenu(CurrentMenu::MAIN_MENU);
    new MainMenuController(controller, this);
    new ParametersMenuController(controller, this);
    new InformationMenuController(controller, this);
    new NetworkMenuController(controller, this);
    new AvailableWifiMenuController(controller, this);
}

MenuController::~MenuController()
{
    for(auto& ent : this->getSubMenus())
    {
        if(ent.second != nullptr)
        {
            delete ent.second;
        }
    }

}

MenuController* MenuController::control()
{
    AbstractController::control();
    if(this->isActive())
    {
        this->getController()->setOverlayLevel(OverlayLevel::MENU);
        this->getController()->viewData.menuActive = true;
        this->getController()->viewData.currentMenu = this->getCurrentMenu();


        if(this->getSubMenu(this->getCurrentMenu()) != nullptr)
        {
            this->getSubMenu(this->getCurrentMenu())->control();
        }

    }
    else
    {
        this->getController()->viewData.menuActive = false;
    }

    return this;
}

MenuController* MenuController::inputControl()
{
    AbstractController::inputControl();

    if(this->getSubMenu(this->getCurrentMenu()) != nullptr)
    {
        this->getSubMenu(this->getCurrentMenu())->inputControl();
    }

    return this;
}

MenuController* MenuController::isActive(bool isActive)
{
    this->m_isActive = isActive;
    return this;
}

bool MenuController::isActive()
{
    return this->m_isActive;
}

MenuController* MenuController::setCurrentMenu(CurrentMenu currentMenu)
{
    this->m_currentMenu = currentMenu;
    return this;
}

CurrentMenu MenuController::getCurrentMenu()
{
    return this->m_currentMenu;
}

MenuController* MenuController::addSubMenu(const CurrentMenu& menuID, AbstractMenuController* submenu)
{
    if(this->m_subMenus.count(menuID) > 0)
    {
        if(this->m_subMenus.at(menuID) != nullptr)
        {
            delete this->m_subMenus.at(menuID);
        }
    }

    this->m_subMenus[menuID] = submenu;

    return this;
}

AbstractMenuController* MenuController::getSubMenu(const CurrentMenu& menuID) const
{
    if(this->m_subMenus.count(menuID) > 0)
    {
        return this->m_subMenus.at(menuID);
    }

    return nullptr;
}

map <CurrentMenu, AbstractMenuController*> MenuController::getSubMenus() const
{
    return this->m_subMenus;
}
