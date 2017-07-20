#ifndef DEF_OSCONTROLLER
#define DEF_OSCONTROLLER

#include <string>

#include "abstractController.hpp"

class OSController : public AbstractController
{
public:
    OSController(Controller* controller);
    ~OSController();

    virtual OSController* control();
    virtual OSController* inputControl();

    bool powerOff();
    bool reboot();

    static std::string execCmd(const char* cmd);

protected:

};

#endif
