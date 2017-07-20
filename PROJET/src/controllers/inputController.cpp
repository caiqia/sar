#include <iostream>
#include "gameLaunchController.hpp"
#include "inputController.hpp"
#include "controller.hpp"
#include "inputConfiguratorController.hpp"
#include "../entities/log.hpp"
#include "../entities/error.hpp"

using namespace std;

//Controller who manages inputs and gamepad relative events

//Init
InputController::InputController(Controller* controller)
:AbstractController(controller, INPUT_CONTROLLER)
{
    //Get number of connected gamepads
    int connectedGamepads = SDL_NumJoysticks();

    Log::addEntry(1, to_string(connectedGamepads) + " gamepad(s) connected during system initialization");

    Log::addEntry(1, "InputController initialized successfully");

}

InputController::~InputController()
{
    //delete every objects for connected gamepads
    for(auto& ent : this->getGamepads())
    {
        delete(ent);
    }

    this->m_inputStatus.clear();

    Log::addEntry(1, "Deleted InputController");
}

//Control loop for input interpretation
InputController* InputController::control()
{
    AbstractController::control();
    //Go through each available gamepad
    for(auto& ent : this->getGamepads())
    {
        //Get button and axis status for the current gamepad
        vector<GamepadStatus> gamepadStatus = ent->querryGamePad();

        //Get the previous status to compare
        map<GamepadActions, InputStatus> previousInput = getControllerInputStatus(ent->getNumber());

        //Go through each button and axis for the current controller
        for(auto& en : gamepadStatus)
        {
            //InputStatus structure for key/axis, get current one and create a new one
            InputStatus previousStatus = previousInput[en.action];
            InputStatus newStatus;

            //If the button/axis is now inactive, set all its values to 0
            if(en.pushed == 0)
            {
                newStatus.pushed = 0;
                newStatus.originTimer = 0;
                newStatus.previousTimer = 0;
                this->getController()->setControl(en.action, 0);
                this->getController()->setGamepadControl(ent, en.action, 0);
            }
            //If the button/axis is no active
            else
            {
                //If it was peviously inactive, set pushed, and init timers.
                if(previousStatus.originTimer == 0)
                {
                    newStatus.pushed = 1;
                    newStatus.originTimer = SDL_GetTicks();
                    newStatus.previousTimer = SDL_GetTicks();
                    this->getController()->setControl(en.action, 1);
                    this->getController()->setGamepadControl(ent, en.action, 1);
                }
                //If it was already active
                else
                {
                    //There is no status change
                    newStatus = previousStatus;

                    //If it is Magic button, always send an positive signal to the main controller
                    if(en.action == GamepadActions::MAGIC)
                    {
                        this->getController()->setControl(en.action, 1);
                        this->getController()->setGamepadControl(ent, en.action, 1);
                    }
                    //If it is a directionnal control, send either a positive or negative signal to the main controller depending on since when it's pushed down
                    else if(en.action == GamepadActions::UP || en.action == GamepadActions::DOWN || en.action == GamepadActions::RIGHT || en.action == GamepadActions::LEFT)
                    {
                        //If last positive input was sent to the main controller more then X milliseconds ago send a new one and reset timer
                        //TODO add exponential speed steps
                        if((SDL_GetTicks() - previousStatus.previousTimer) > 250)
                        {
                            this->getController()->setControl(en.action, 1);
                            this->getController()->setGamepadControl(ent, en.action, 1);
                            newStatus.previousTimer = SDL_GetTicks();
                        }
                        else
                        {
                            this->getController()->setControl(en.action, 0);
                            this->getController()->setGamepadControl(ent, en.action, 0);
                        }
                    }
                    else
                    {
                        this->getController()->setControl(en.action, 0);
                        this->getController()->setGamepadControl(ent, en.action, 0);
                    }
                }
            }
            //save new status
            this->addInputstatus(ent->getNumber(), en.action, newStatus);
        }
    }

    return this;
}

InputController* InputController::inputControl()
{
    AbstractController::inputControl();
    return this;
}

//function used to control on SDL events
InputController* InputController::eventControl(SDL_Event* events)
{
    if(events->type == SDL_JOYDEVICEADDED)
    {
        this->connectGamepad(events->jdevice.which);

    }

    if(events->type == SDL_JOYDEVICEREMOVED)
    {
        this->disconnectGamepad(events->jdevice.which);

    }

}

InputController* InputController::addGamepad(Gamepad* gamepad)
{
    this->m_gamePads.push_back(gamepad);
    return this;
}



vector<Gamepad*> InputController::getGamepads()
{
    return this->m_gamePads;
}

InputController* InputController::reNumberGamepads()
{
    int i = 0;
    for(auto& ent : this->getGamepads())
    {
        ent->setNumber(i);
        ++i;
    }
}

InputController* InputController::addInputstatus(int gamepadNumber, GamepadActions action, InputStatus status)
{
    if(this->m_inputStatus[gamepadNumber].find(action) != this->m_inputStatus[gamepadNumber].end())
    {
        this->m_inputStatus[gamepadNumber].erase(action);
    }
    this->m_inputStatus[gamepadNumber].insert(pair<GamepadActions, InputStatus>(action, status));
    return this;
}

map<GamepadActions, InputStatus> InputController::getControllerInputStatus(int controller)
{
    return this->m_inputStatus[controller];
}

map<int, map<GamepadActions, InputStatus>> InputController::getInputStatus()
{
    return this->m_inputStatus;
}

InputController* InputController::connectGamepad(int deviceIndex)
{

    InputConfiguratorController* inputConfiguratorController = static_cast <InputConfiguratorController*> (this->getController()->getController(INPUT_CONFIGURATOR_CONTROLLER));

    if(inputConfiguratorController != nullptr)
    {
        //Create a gamepad object for the current gamepad
        string profile = inputConfiguratorController->getProfileFromIndex(deviceIndex);

        Gamepad* pGamepad = new Gamepad(deviceIndex, profile);

        //If the gamepad was correctly initialized, add it to the list of available gamepads
        if (pGamepad->getInit())
        {
            this->addGamepad(pGamepad);
            //Init gamepad status to 0
            for (int j = GamepadActions::UP; j != GamepadActions::MAGIC; j++)
            {
                InputStatus initStatus;
                initStatus.pushed = 0;
                initStatus.originTimer = 0;
                initStatus.previousTimer = 0;

                this->addInputstatus(deviceIndex, static_cast<GamepadActions>(j), initStatus);
            }

            //If the gamepad has multiple profiles possible, we ask the user which one he uses
            if(pGamepad->getProfile() == "multiple")
            {
                inputConfiguratorController->selectProfile(pGamepad);
            }

            //It in an unsupported gamepad
            else if(pGamepad->getProfile() == "null")
            {
                //TODO Not supported gamepad !!!

            }
        }
        else
        {

            Log::addEntry(1, "Controller could not be initialized correctly");
            delete pGamepad;
        }
    }
    else
    {
        //TODO error
    }

    return this;

}

InputController* InputController::disconnectGamepad(SDL_JoystickID instanceID)
{
    vector<Gamepad*>::size_type i = 0;

    InputConfiguratorController* inputConfiguratorController = static_cast <InputConfiguratorController*> (this->getController()->getController(INPUT_CONFIGURATOR_CONTROLLER));

    if(inputConfiguratorController != nullptr)
    {
        //We go through each gamepads to determine which one has the removed one's InstanceId
        while(i < this->m_gamePads.size())
        {
            if(this->m_gamePads[i]->getInstanceID() == instanceID)
            {
                //If the removed gamepad was being configured, we remove it from the ocnfiguration queue
                if(inputConfiguratorController->isGamepadInSelectQueue(i))
                {
                    inputConfiguratorController->removeGamepadFromSelectQueue(i);
                }

                //We delete the entity, remove the gamepad for the instance list and decrement the gamepad counter
                delete this->m_gamePads[i];
                this->m_gamePads.erase(this->m_gamePads.begin() + i);
                -- Gamepad::count;
            }
            else
            {
                ++i;
            }
        }

        this->reNumberGamepads();
    }
    else
    {
        //TODO error
    }

    return this;
}
