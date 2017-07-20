#ifndef DEF_ABSTRACTCONTROLLER
#define DEF_ABSTRACTCONTROLLER

#include "controller.hpp"

class AbstractController
{
public:
    AbstractController(Controller* controller, ControllersID controllerID);
    ~AbstractController();

    virtual AbstractController* control() = 0;
    virtual AbstractController* inputControl() = 0;
    virtual AbstractController* eventControl(SDL_Event* events);

    AbstractController* setController(Controller* controller);
    Controller* getController() const;

    AbstractController* setControllerID(const ControllersID& controllerID);
    ControllersID getControllerID() const;

protected:
    Controller* m_pController = nullptr;
    ControllersID m_controllerID;
};

#endif
