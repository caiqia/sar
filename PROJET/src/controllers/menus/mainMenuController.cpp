#include "mainMenuController.hpp"
#include "../menuController.hpp"
#include "../controller.hpp"
#include "../visualKeyboardController.hpp"
#include "../popupWindowController.hpp"

#include <iostream>

using namespace std;

const string MAIN_MENU_TITLE = "Menu";

MainMenuController::MainMenuController(Controller* controller, MenuController* menuController)
: AbstractMenuController(controller, menuController, CurrentMenu::MAIN_MENU, MAIN_MENU_TITLE)
{
    this->addEntity(new MenuButtonEntity(EntityID::PARAMETERS_BTN ,"Paramètres"));
    this->addEntity(new MenuButtonEntity(EntityID::INFORMATION_BTN ,"Informations"));
    this->addEntity(new MenuButtonEntity(EntityID::UPDATE_BTN ,"Mise à jour"));
    this->addEntity(new MenuButtonEntity(EntityID::QUIT_BTN ,"Quitter"));
}

MainMenuController::~MainMenuController()
{

}

MainMenuController* MainMenuController::control()
{
    AbstractMenuController::control();

    if(this->getQuitButtonPopupString() != "")
    {
        if(this->getQuitButtonPopupString() == "Annuler")
        {
            this->clearQuitButtonPopupString();
        }
        else if(this->getQuitButtonPopupString() == "Eteindre")
        {
            this->getController()->stop();
        }
        else if(this->getQuitButtonPopupString() == "Redémarrer")
        {
            this->getController()->reboot();
        }
    }

    return this;
}

MainMenuController* MainMenuController::inputControl()
{
    AbstractMenuController::inputControl();

    for(const auto& ent : this->getController()->getAdvancedControls())
    {
        switch (this->getSelectedEntityID()) {
            case EntityID::PARAMETERS_BTN :
            {
                if(this->getController()->getGamepadControl(ent.first, GamepadActions::B))
                {
                    this->goToMenu(PARAMETERS_MENU);
                }
                break;
            }
            case EntityID::INFORMATION_BTN :
            {
                if(this->getController()->getGamepadControl(ent.first, GamepadActions::B))
                {
                    this->goToMenu(INFORMATION_MENU);
                }
                break;
            }
            case EntityID::QUIT_BTN :
            {
                if(this->getController()->getGamepadControl(ent.first, GamepadActions::B))
                {
                    vector <string> buttons = {"Annuler", "Eteindre", "Redémarrer"};

                    PopupWindowController* popupWindowController = static_cast<PopupWindowController*> (this->getController()->getController(POPUP_WINDOW_CONTROLLER));

                    if(popupWindowController != nullptr)
                    {
                        popupWindowController->popup("Vous êtes sur le point de quitter", "Souhaitez vous réellement mettre hors tension votre Retropac ?", buttons, this->getQuitButtonPopupStringPointer());
                    }
                }
                break;
            }

        }

        if(this->getController()->getGamepadControl(ent.first, GamepadActions::START) || this->getController()->getGamepadControl(ent.first, GamepadActions::A))
        {
            this->quitMenu();
        }
    }
    return this;
}

string MainMenuController::getQuitButtonPopupString() const
{
    return this->m_quitButtonPopup;
}

MainMenuController* MainMenuController::clearQuitButtonPopupString()
{
    this->m_quitButtonPopup = "";
    return this;
}

string* MainMenuController::getQuitButtonPopupStringPointer()
{
    return &(this->m_quitButtonPopup);
}
