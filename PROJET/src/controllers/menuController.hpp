#ifndef DEF_MENUCONTROLLER
#define DEF_MENUCONTROLLER

#include <map>

#include "abstractController.hpp"
#include "enums/currentMenuEnum.hpp"

class AbstractMenuController;

class MenuController : public AbstractController
{
public:
    MenuController(Controller* controller);
    ~MenuController();

    virtual MenuController* control();

    virtual MenuController* inputControl();

    MenuController* isActive(bool isActive);
    bool isActive();

    MenuController* setCurrentMenu(CurrentMenu currentMenu);
    CurrentMenu getCurrentMenu();

    MenuController* addSubMenu(const CurrentMenu& menuID, AbstractMenuController* submenu);
    AbstractMenuController* getSubMenu(const CurrentMenu& menuID) const;
    std::map <CurrentMenu, AbstractMenuController*> getSubMenus() const;

protected:
    bool m_isActive = false;

    CurrentMenu m_currentMenu = CurrentMenu::MAIN_MENU;

    std::map <CurrentMenu, AbstractMenuController*> m_subMenus;
};

#endif
