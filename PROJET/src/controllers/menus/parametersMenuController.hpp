#ifndef DEF_PARAMETERSMENUCONTROLLER
#define DEF_PARAMETERSMENUCONTROLLER

#include "abstractMenuController.hpp"

class ParametersMenuController : public AbstractMenuController
{
public:
    ParametersMenuController(Controller* controller, MenuController* menuController);
    ~ParametersMenuController();

    virtual ParametersMenuController* control();

    virtual ParametersMenuController* inputControl();

protected:

};

#endif
