#include <regex>
#include <iostream>
#include "errorController.hpp"
#include "controller.hpp"
#include "../entities/error.hpp"
#include "../entities/log.hpp"
#include <string>


using namespace std;
//Controller that manages the errors


//Init
ErrorController::ErrorController(Controller* controller)
: AbstractController(controller, ERROR_CONTROLLER)
{
    Log::addEntry(1, "ErrorController initialized successfully");
}

ErrorController::~ErrorController()
{
    Log::addEntry(1, "Deleted ErrorController");
}

//Control loop to treat the errors
ErrorController* ErrorController::control()
{
    AbstractController::control();

    //If the errorqueue is not empty, we take the first one and set it as the one to treat
    if(Error::errorqueue.size() > 0)
    {

       this->setError(&Error::errorqueue.front());
        if(this->getError() != nullptr)
        {

          this->getController()->setOverlayLevel(OverlayLevel::ERROR);

          switch (this->getError()->getErrorType()) {
            case ErrorType::CRITIQUE:
                cerr<<this->getError()->getErrorMessage()<<endl;
                this->removeFirstError();
                break;
            case ErrorType::MAJOR:
                this->getController()->viewData.majorerrorAffich = true;
                break;
            case ErrorType::EXECUTION:
                this->getController()->viewData.executionerrorAffich = true;
                break;
            case ErrorType::MINOR:
                this->removeFirstError();
                break;
            default :
                break;
          }

          string logentry = "/!\\ ERROR [" + this->getError()->getErrorCode() + "] : " + this->getError()->getErrorMessage();
          Log::addEntry(1, logentry);

      }
      this->getController()->viewData.errorMessage = this->m_error->getErrorMessage();
      this->getController()->viewData.errorCode = this->m_error->getErrorCode();
    }

    else
    {
      this->getController()->viewData.majorerrorAffich = false;
      this->getController()->viewData.executionerrorAffich = false;
    }

    return this;
}

ErrorController* ErrorController::inputControl()
{
    AbstractController::inputControl();

    for(auto& ent : this->getController()->getAdvancedControls())
    {
        if(this->getController()->getGamepadControl(ent.first, GamepadActions::A) )
        {
            if(this->getController()->viewData.executionerrorAffich == true )
            {
                this->removeFirstError();
                this->getController()->viewData.executionerrorAffich = false;
            }
            if(this->getController()->viewData.majorerrorAffich == true )
            {
                this->getController()->stop();
            }

        }
    }
    return this;
}

ErrorController* ErrorController::removeFirstError()
{
    if(Error::errorqueue.size() > 0)
    {
        Error::errorqueue.erase(Error::errorqueue.begin());
    }
    return this;
}

Error* ErrorController::getError()
{
  return this->m_error;
}

ErrorController* ErrorController::setError(Error* error)
{
  this->m_error = error;
  return this;
}
