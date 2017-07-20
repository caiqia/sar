#ifndef DEF_NETWORKMENUCONTROLLER
#define DEF_NETWORKMENUCONTROLLER

#include "abstractMenuController.hpp"

class NetworkMenuController : public AbstractMenuController
{
public:
    NetworkMenuController(Controller* controller, MenuController* menuController);
    ~NetworkMenuController();

    virtual NetworkMenuController* control();

    virtual NetworkMenuController* inputControl();

protected:

};

#endif
