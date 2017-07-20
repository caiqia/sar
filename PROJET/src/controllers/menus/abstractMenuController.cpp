#include "abstractMenuController.hpp"
#include "../menuController.hpp"
#include "../enums/currentMenuEnum.hpp"
#include "../../entities/menuEntities.hpp"

#include <iostream>

using namespace std;

AbstractMenuController::AbstractMenuController(Controller* controller, MenuController* menuController, const CurrentMenu& menuID, std::string menuTitle)
{
    this->setController(controller);
    this->setMenuController(menuController);
    this->setMenuID(menuID);
    this->setSelectedEntity(0);
    this->setMenuTitle(menuTitle);
    this->getMenuController()->addSubMenu(menuID, this);
}

AbstractMenuController::~AbstractMenuController()
{
    for(const auto & ent : this->getEntities())
    {
        if(ent.second != nullptr)
        {
            delete ent.second;
        }
    }
}

AbstractMenuController* AbstractMenuController::control()
{
    if(this->getController()->viewData.menuTitle != this->getMenuTitle())
    {
        this->getController()->viewData.menuTitle = this->getMenuTitle();
    }

    return this;
}

AbstractMenuController* AbstractMenuController::inputControl()
{
    for(const auto& ent : this->getController()->getAdvancedControls())
    {
        if(this->getController()->getGamepadControl(ent.first, GamepadActions::DOWN))
        {
            this->goToNextEntity();
        }
        if(this->getController()->getGamepadControl(ent.first, GamepadActions::UP))
        {
            this->goToPreviousEntity();
        }
    }
    return this;
}

AbstractMenuController* AbstractMenuController::setController(Controller* controller)
{
    this->m_pController = controller;
    return this;
}

Controller* AbstractMenuController::getController() const
{
    return this->m_pController;
}

AbstractMenuController* AbstractMenuController::setMenuController(MenuController* menuController)
{
    this->m_pMenuController = menuController;
    return this;
}

MenuController* AbstractMenuController::getMenuController() const
{
    return this->m_pMenuController;
}

AbstractMenuController* AbstractMenuController::addEntity(MenuEntity* menuEntity)
{
    this->m_menuEntities.insert(pair<int, MenuEntity*> (this->m_entityCounter, menuEntity));

    this->getController()->viewData.menuEntities[this->getMenuID()][this->m_entityCounter] = menuEntity;

    ++ this->m_entityCounter;
    return this;
}

AbstractMenuController* AbstractMenuController::clearEntities()
{
    for(auto& ent : this->m_menuEntities)
    {
        if(ent.second != nullptr)
        {
            delete ent.second;
        }
    }

    this->m_menuEntities.clear();
    this->m_entityCounter = 0;

    if(this->getController()->viewData.menuEntities.count(this->getMenuID()) > 0)
    {
        this->getController()->viewData.menuEntities.at(this->getMenuID()).clear();
    }

    return this;
}

MenuEntity* AbstractMenuController::getEntity(const int& number) const
{
    if(this->m_menuEntities.count(number) > 0)
    {
        return this->m_menuEntities.at(number);
    }
    else
    {
        cout << "entity not found" << endl;
        //not found exception
    }
}

std::map<int, MenuEntity*> AbstractMenuController::getEntities() const
{
    return this->m_menuEntities;
}

AbstractMenuController* AbstractMenuController::setSelectedEntity(const int& number)
{
    this->m_selectedEntity = number;
    this->getController()->viewData.selectedItem = number;
    return this;
}

int AbstractMenuController::getSelectedEntityNumber() const
{
    return this->m_selectedEntity;
}

MenuEntity* AbstractMenuController::getSelectedEntity() const
{
    if(this->getEntities().count(this->getSelectedEntityNumber()) > 0)
    {
        return this->getEntity(this->getSelectedEntityNumber());
    }
    else
    {
        cout << "entity not found" << endl;
        //not found exception
    }
}

MenuEntityType AbstractMenuController::getSelectedEntityType() const
{
    return this->getSelectedEntity()->getEntityType();
}

EntityID AbstractMenuController::getSelectedEntityID() const
{
    return this->getSelectedEntity()->getID();
}

AbstractMenuController* AbstractMenuController::goToNextEntity()
{
    this->getController()->viewData.previouslySelectedItem = this->getSelectedEntityNumber();
    if(this->getSelectedEntityNumber() < (this->m_entityCounter - 1))
    {
        this->setSelectedEntity(this->getSelectedEntityNumber() + 1);
    }
    else
    {
        this->setSelectedEntity(0);
    }
    return this;
}

AbstractMenuController* AbstractMenuController::goToPreviousEntity()
{
    this->getController()->viewData.previouslySelectedItem = this->getSelectedEntityNumber();
    if(this->getSelectedEntityNumber() != 0)
    {
        this->setSelectedEntity(this->getSelectedEntityNumber() - 1);
    }
    else
    {
        this->setSelectedEntity(this->m_entityCounter - 1);
    }
    return this;
}

AbstractMenuController* AbstractMenuController::setMenuID(const CurrentMenu& menuID)
{
    this->m_menuID = menuID;
    return this;
}

CurrentMenu AbstractMenuController::getMenuID() const
{
    return this->m_menuID;
}

AbstractMenuController* AbstractMenuController::quitMenu()
{
    this->getMenuController()->setCurrentMenu(CurrentMenu::MAIN_MENU);
    this->getMenuController()->isActive(false);
    return this;
}

AbstractMenuController* AbstractMenuController::goToMenu(const CurrentMenu& MenuID)
{
    this->getMenuController()->setCurrentMenu(MenuID);
    this->setSelectedEntity(0);
    return this;
}

AbstractMenuController* AbstractMenuController::setMenuTitle(const string& menuTitle)
{
    this->m_menuTitle = menuTitle;
    return this;
}

string AbstractMenuController::getMenuTitle() const
{
    return this->m_menuTitle;
}
