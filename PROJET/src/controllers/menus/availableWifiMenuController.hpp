#ifndef DEF_AVAILABLEWIFIMENUCONTROLLER
#define DEF_AVAILABLEWIFIMENUCONTROLLER

#include "abstractMenuController.hpp"

#include <vector>

class AvailableWifiMenuController : public AbstractMenuController
{
public:
    AvailableWifiMenuController(Controller* controller, MenuController* menuController);
    ~AvailableWifiMenuController();

    virtual AvailableWifiMenuController* control();

    virtual AvailableWifiMenuController* inputControl();

    AvailableWifiMenuController* createWifiEntities();

protected:
    std::string m_psswd_test;
    std::string m_output;
};

#endif
