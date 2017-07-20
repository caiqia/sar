#include "availableWifiMenuController.hpp"

#include "../../entities/network.hpp"
#include "../wifiController.hpp"
#include "../inputPopupWindowController.hpp"

#include <iostream>

using namespace std;

const string WIFI_MENU_TITLE = "Réseaux Wi-Fi";
const string WIFI_LOOKOUT_BUTTON_TEXT = "Rechercher les réseaux";

AvailableWifiMenuController::AvailableWifiMenuController(Controller* controller, MenuController* menuController)
: AbstractMenuController (controller, menuController, CurrentMenu::WIFI_CONNECTION_MENU, WIFI_MENU_TITLE)
{
    this->addEntity(new MenuButtonEntity(EntityID::SCAN_WIFI_BTN, WIFI_LOOKOUT_BUTTON_TEXT));
}

AvailableWifiMenuController::~AvailableWifiMenuController()
{

}

AvailableWifiMenuController* AvailableWifiMenuController::control()
{
    AbstractMenuController::control();
    return this;
}

AvailableWifiMenuController* AvailableWifiMenuController::inputControl()
{
    AbstractMenuController::inputControl();

    for(const auto& ent : this->getController()->getAdvancedControls())
    {
        switch (this->getSelectedEntityID()) {
            case EntityID::SCAN_WIFI_BTN :
            {
                if(this->getController()->getGamepadControl(ent.first, GamepadActions::B))
                {
                    this->createWifiEntities();
                }
                break;
            }

            case EntityID::WIFI_BUTTON :
            {
                if(this->getController()->getGamepadControl(ent.first, GamepadActions::B))
                {
                    vector<string> but {"OK", "Annuler"};

                    //VisibleNetwork selectedNetwork =

                    InputPopupWindowController* inputPopup = static_cast<InputPopupWindowController*>(this->getController()->getController(INPUT_POPUP_WINDOW_CONTROLLER));
                    inputPopup->popup("Connexion au réseau", "Veillez saisir le mot de passe", but, &this->m_output, &this->m_psswd_test);
                }
                break;
            }
        }

        if(this->getController()->getGamepadControl(ent.first, GamepadActions::A))
        {
            this->goToMenu(CurrentMenu::NETWORK_PARAMETERS_MENU);
            this->clearEntities();
            this->addEntity(new MenuButtonEntity(EntityID::SCAN_WIFI_BTN, WIFI_LOOKOUT_BUTTON_TEXT));
        }
        if(this->getController()->getGamepadControl(ent.first, GamepadActions::START))
        {
            cout << this->m_psswd_test << endl;
        }
    }

    return this;
}

AvailableWifiMenuController* AvailableWifiMenuController::createWifiEntities()
{
    this->setSelectedEntity(0);
    this->clearEntities();

    WifiController* wifiController = static_cast<WifiController*> (this->getController()->getController(WIFI_CONTROLLER));
    vector<VisibleNetwork> network = wifiController->scanWifi();

    for(const auto& ent : network)
    {
        bool auth = false;
        if(ent.auth != AuthType::NONE)
        {
            auth = true;
        }
        this->addEntity(new WifiButtonEntity(EntityID::WIFI_BUTTON , ent.SSID , ent.signalLevel, auth));
    }

    this->addEntity(new MenuButtonEntity(EntityID::SCAN_WIFI_BTN, WIFI_LOOKOUT_BUTTON_TEXT));

    return this;
}
