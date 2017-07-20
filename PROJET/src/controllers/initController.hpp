#ifndef DEF_INITCONTROLLER
#define DEF_INITCONTROLLER

#include "abstractController.hpp"

class InitController : public AbstractController
{
public:
    InitController(Controller* controller);
    ~InitController();

    virtual InitController* control();
    virtual InitController* inputControl();

    InitController* incrementControllerLoadProgress();
    InitController* incrementViewLoadProgress();
    InitController* incrementViewInitProgress();

protected:
    InitController* setGlobalLoading();
    InitController* renderLoadingScreen();

    double m_controllerLoadprogress = 0.0;
    double m_viewLoadProgress = 0.0;
    double m_viewInitProgress = 0.0;
};

#endif
