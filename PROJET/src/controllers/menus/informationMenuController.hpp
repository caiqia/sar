#ifndef DEF_INFORMATIONMENUCONTROLLER
#define DEF_INFORMATIONMENUCONTROLLER

#include "abstractMenuController.hpp"

class InformationMenuController : public AbstractMenuController
{
public:
    InformationMenuController(Controller* controller, MenuController* menuController);
    ~InformationMenuController();

    virtual InformationMenuController* control();

    virtual InformationMenuController* inputControl();

protected:

};

#endif
