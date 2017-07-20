#include "abstractController.hpp"

AbstractController::AbstractController(Controller* controller, ControllersID controllerID)
{
    this->setController(controller);
    this->setControllerID(controllerID);
    this->getController()->addController(this->getControllerID(), this);
}

AbstractController::~AbstractController()
{

}

AbstractController* AbstractController::control()
{
    return this;
}

AbstractController* AbstractController::inputControl()
{
    return this;
}

AbstractController* AbstractController::eventControl(SDL_Event* events)
{
    return this;
}

AbstractController* AbstractController::setController(Controller* controller)
{
    this->m_pController = controller;
}

Controller* AbstractController::getController() const
{
    return this->m_pController;
}

AbstractController* AbstractController::setControllerID(const ControllersID& controllerID)
{
    this->m_controllerID = controllerID;
}

ControllersID AbstractController::getControllerID() const
{
    return this->m_controllerID;
}
