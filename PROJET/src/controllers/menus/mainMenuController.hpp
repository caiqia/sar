#ifndef DEF_MAINMENUCONTROLLER
#define DEF_MAINMENUCONTROLLER

#include "../controller.hpp"
#include "../../entities/menuEntities.hpp"
#include "abstractMenuController.hpp"

class MenuController;

class MainMenuController : public AbstractMenuController
{
public:
    MainMenuController(Controller* controller, MenuController* menuController);
    ~MainMenuController();

    virtual MainMenuController* control();

    virtual MainMenuController* inputControl();

    std::string getQuitButtonPopupString() const;
    MainMenuController* clearQuitButtonPopupString();
    std::string* getQuitButtonPopupStringPointer();

protected:
    std::map<int, MenuEntity*> m_menuEntities;

    std::string m_quitButtonPopup;

};

#endif
