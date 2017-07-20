#ifndef DEF_ABSTRACTMENUCONTROLLER
#define DEF_ABSTRACTMENUCONTROLLER

#include <map>
#include "../controller.hpp"
#include "../../entities/menuEntities.hpp"

class MenuController;

class AbstractMenuController
{
public:
    AbstractMenuController(Controller* controller, MenuController* menuController, const CurrentMenu& menuID, std::string menuTitle);
    ~AbstractMenuController();

    virtual AbstractMenuController* control() = 0;

    virtual AbstractMenuController* inputControl() = 0;

    AbstractMenuController* setController(Controller* controller);
    Controller* getController() const;

    AbstractMenuController* setMenuController(MenuController* menuController);
    MenuController* getMenuController() const;

    AbstractMenuController* addEntity(MenuEntity* menuEntity);
    AbstractMenuController* clearEntities();
    MenuEntity* getEntity(const int& number) const;
    std::map<int, MenuEntity*> getEntities() const;

    AbstractMenuController* setSelectedEntity(const int& number);
    int getSelectedEntityNumber() const;
    MenuEntity* getSelectedEntity() const;
    MenuEntityType getSelectedEntityType() const;
    EntityID getSelectedEntityID() const;

    AbstractMenuController* goToNextEntity();
    AbstractMenuController* goToPreviousEntity();

    AbstractMenuController* setMenuID(const CurrentMenu& menuID);
    CurrentMenu getMenuID() const;

    AbstractMenuController* quitMenu();
    AbstractMenuController* goToMenu(const CurrentMenu& MenuID);

    AbstractMenuController* setMenuTitle(const std::string& menuTitle);
    std::string getMenuTitle() const;

protected:
    Controller* m_pController = nullptr;
    MenuController* m_pMenuController = nullptr;

    int m_entityCounter = 0;

    int m_selectedEntity = 0;

    std::map<int, MenuEntity*> m_menuEntities;

    CurrentMenu m_menuID;

    std::string m_menuTitle;
};

#endif
