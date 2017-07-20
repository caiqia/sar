#include "networkMenuController.hpp"

using namespace std;

const string NETWORK_PARAMETERS_MENU_TITLE = "Paramètres réseau";

NetworkMenuController::NetworkMenuController(Controller* controller, MenuController* menuController)
: AbstractMenuController (controller, menuController, CurrentMenu::NETWORK_PARAMETERS_MENU, NETWORK_PARAMETERS_MENU_TITLE)
{
    this->addEntity(new MenuButtonEntity(EntityID::NETWORK_STATUS_BTN ,"Etat de la connexion"));
    this->addEntity(new MenuButtonEntity(EntityID::WIRELESS_CONNEXION_BTN ,"Connexion sans fil"));
}

NetworkMenuController::~NetworkMenuController()
{

}

NetworkMenuController* NetworkMenuController::control()
{
    AbstractMenuController::control();

    return this;
}

NetworkMenuController* NetworkMenuController::inputControl()
{
    AbstractMenuController::inputControl();
    for(const auto& ent : this->getController()->getAdvancedControls())
    {
        switch (this->getSelectedEntityID()) {
            case EntityID::WIRELESS_CONNEXION_BTN :
            {
                if(this->getController()->getGamepadControl(ent.first, GamepadActions::B))
                {
                    this->goToMenu(WIFI_CONNECTION_MENU);
                }
                break;
            }
        }

        if(this->getController()->getGamepadControl(ent.first, GamepadActions::A))
        {
            this->goToMenu(CurrentMenu::PARAMETERS_MENU);
        }
    }

    return this;
}
