#ifndef DEF_GAMELAUNCHCONTROLLER
#define DEF_GAMELAUNCHCONTROLLER

#include "../entities/game.hpp"
#include "abstractController.hpp"

class Controller;

class GameLaunchController : public AbstractController
{
public:
    GameLaunchController(Controller* controller);
    ~GameLaunchController();

    virtual GameLaunchController* control();

    virtual GameLaunchController* inputControl();

    GameLaunchController* launch(Game* game);

    GameLaunchController* createConfig(Game* game);

protected:

};

#endif
