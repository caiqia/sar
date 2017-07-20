#include "osController.hpp"
#include "controller.hpp"

#include <memory>
#include <stdexcept>
#include <iostream>

using namespace std;

OSController::OSController(Controller* controller)
: AbstractController(controller, OS_CONTROLLER)
{

}

OSController::~OSController()
{

}

OSController* OSController::control()
{
    AbstractController::control();
    return this;
}

OSController* OSController::inputControl()
{
    AbstractController::inputControl();
    return this;
}

bool OSController::powerOff()
{
    if(!this->getController()->getDevMode())
    {
        system("poweroff");
    }
    else
    {
        cout << "[DEV] shutting down" << endl;
        this->getController()->stop();
    }


}

bool OSController::reboot()
{
    if(!this->getController()->getDevMode())
    {
        system("reboot");
    }
    else
    {
        cout << "[DEV] rebooting" << endl;
        this->getController()->stop();
    }

}

string OSController::execCmd(const char* cmd)
{
    char buffer[1024];
    string result = "";
    shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);

    if (!pipe) throw runtime_error("popen() failed!");

    while (!feof(pipe.get())) {

        if (fgets(buffer, 128, pipe.get()) != NULL)
            result += buffer;
    }
    return result;
}
