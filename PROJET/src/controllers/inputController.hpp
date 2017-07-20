#ifndef DEF_INPUTCONTROLLER
#define DEF_INPUTCONTROLLER

#include <vector>
#include "../entities/gamepad.hpp"
#include "abstractController.hpp"

//Status for a particular buton or axis
struct InputStatus
{
    bool pushed = 0; //Is it active
    int originTimer = 0; //Intitial time it was pushed down
    int previousTimer = 0; //Time last input was sent to main controller
};

class InputController : public AbstractController
{
public:
    InputController(Controller* controller);
    ~InputController();

    virtual InputController* control();

    virtual InputController* inputControl();

    virtual InputController* eventControl(SDL_Event* events);

    InputController* addGamepad(Gamepad* gamepad);

    std::vector<Gamepad*> getGamepads();

    InputController* reNumberGamepads();

    InputController* addInputstatus(int gamepadNumber, GamepadActions action, InputStatus status);
    std::map<GamepadActions, InputStatus> getControllerInputStatus(int controller);
    std::map<int, std::map<GamepadActions, InputStatus>> getInputStatus();

    InputController* connectGamepad(int deviceIndex);
    InputController* disconnectGamepad(SDL_JoystickID instanceID);


protected:
    std::vector<Gamepad*> m_gamePads;
    std::map<int, std::map<GamepadActions, InputStatus>> m_inputStatus;

};

#endif
