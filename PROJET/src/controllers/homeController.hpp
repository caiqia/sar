#ifndef DEF_HOMECONTROLLER
#define DEF_HOMECONTROLLER


#include "abstractController.hpp"

#include <map>
#include <vector>

class Controller;

class HomeController : public AbstractController
{
public:
    HomeController(Controller* controller);
    ~HomeController();

    virtual HomeController* control();
    virtual HomeController* inputControl();

    HomeController* systemNext();
    HomeController* systemNextRow();
    HomeController* systemPrevious();
    HomeController* systemPreviousRow();

protected:
};

#endif
