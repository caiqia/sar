#include "informationMenuController.hpp"

using namespace std;

const string INFORMATION_MENU_TITLE = "Informations";

InformationMenuController::InformationMenuController(Controller* controller, MenuController* menuController)
: AbstractMenuController (controller, menuController, CurrentMenu::INFORMATION_MENU, INFORMATION_MENU_TITLE)
{
    this->addEntity(new MenuButtonEntity(EntityID::CREDITS_INFORMATION_BTN ,"Crédits"));
    this->addEntity(new MenuButtonEntity(EntityID::SYSTEM_INFORMATION_BTN ,"Information Système"));
    this->addEntity(new MenuButtonEntity(EntityID::HELP_INFORMATION_BTN ,"Assistance"));
}

InformationMenuController::~InformationMenuController()
{

}

InformationMenuController* InformationMenuController::control()
{
    AbstractMenuController::control();
    return this;
}

InformationMenuController* InformationMenuController::inputControl()
{
    AbstractMenuController::inputControl();

    for(const auto& ent : this->getController()->getAdvancedControls())
    {
        switch (this->getSelectedEntityID()) {

        }

        if(this->getController()->getGamepadControl(ent.first, GamepadActions::A))
        {
            this->goToMenu(CurrentMenu::MAIN_MENU);
        }
    }

    return this;
}
