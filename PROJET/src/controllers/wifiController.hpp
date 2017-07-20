#ifndef DEF_WIFICONTROLLER
#define DEF_WIFICONTROLLER

#include <vector>
#include <string>
#include <map>

#include "../entities/network.hpp"
#include "abstractController.hpp"

class Controller;

class WifiController : public AbstractController
{
public:

    WifiController(Controller* controller);
    ~WifiController();

    virtual WifiController* control();
    virtual WifiController* inputControl();

    std::vector<VisibleNetwork> scanWifi();

    WifiController* startChip();
    WifiController* stopChip();
    WifiController* restartChip();
    WifiController* reloadChipParam();

    bool connect(Network network);

    ConnexionState testConnexion();

    bool pingTest();


protected:
    static std::map<AuthType, std::string> authTypeResolver;
};

#endif
