#include "parametersMenuController.hpp"

using namespace std;

const string PARAMETERS_MENU_TITLE = "Paramètres";

ParametersMenuController::ParametersMenuController(Controller* controller, MenuController* menuController)
: AbstractMenuController (controller, menuController, CurrentMenu::PARAMETERS_MENU, PARAMETERS_MENU_TITLE)
{
    this->addEntity(new MenuButtonEntity(EntityID::AUDIO_PARAMETERS_BTN ,"Audio"));
    this->addEntity(new MenuButtonEntity(EntityID::VIDEO_PARAMETERS_BTN ,"Vidéo"));
    this->addEntity(new MenuButtonEntity(EntityID::NETWORK_PARAMETERS_BTN ,"Réseau"));
    this->addEntity(new MenuButtonEntity(EntityID::GAME_PARAMETERS_BTN ,"Jeux"));
    this->addEntity(new MenuButtonEntity(EntityID::INPUT_PARAMETERS_BTN ,"Manettes"));
}

ParametersMenuController::~ParametersMenuController()
{

}

ParametersMenuController* ParametersMenuController::control()
{
    AbstractMenuController::control();
    return this;
}

ParametersMenuController* ParametersMenuController::inputControl()
{
    AbstractMenuController::inputControl();

    for(const auto& ent : this->getController()->getAdvancedControls())
    {
        switch (this->getSelectedEntityID()) {
            case EntityID::NETWORK_PARAMETERS_BTN :
            {
                if(this->getController()->getGamepadControl(ent.first, GamepadActions::B))
                {
                    this->goToMenu(NETWORK_PARAMETERS_MENU);
                }
                break;
            }
        }

        if(this->getController()->getGamepadControl(ent.first, GamepadActions::A))
        {
            this->goToMenu(CurrentMenu::MAIN_MENU);
        }
    }

    return this;
}
