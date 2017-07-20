#ifndef DEF_INPUTCONFIGURATORCONTROLLER
#define DEF_INPUTCONFIGURATORCONTROLLER

#include <string>
#include <vector>

#include "abstractController.hpp"

#include "../entities/gamepad.hpp"

struct ProfileSelection
{
    Gamepad* gamepad;
    std::vector<std::string> profiles;
    int selected;
};


class InputConfiguratorController : public AbstractController
{
public:
    InputConfiguratorController(Controller* controller);
    ~InputConfiguratorController();

    virtual InputConfiguratorController* control();

    virtual InputConfiguratorController* inputControl();

    std::string getProfileFromIndex(int deviceIndex);

    int getNumberProfilesForGUID(std::string GUID);

    std::string getSingleProfileFromGUID(std::string GUID);
    std::vector<std::string> getProfilesFromGUID(std::string GUID);

    InputConfiguratorController* selectProfile(Gamepad* gamepad);

    std::vector<ProfileSelection> getGamepadsToSelect();
    ProfileSelection getFirstGamepadToSelect();
    InputConfiguratorController* addGamepadToSelect(ProfileSelection profileSelection);
    InputConfiguratorController* removeFirstGamepadtoSelect();

    bool isGamepadInSelectQueue(int gamepadNumber);
    InputConfiguratorController* removeGamepadFromSelectQueue(int gamepadNumber);

    std::string profilesToString(std::vector<std::string> profiles);

    InputConfiguratorController* nextProfile();
    InputConfiguratorController* previousProfile();
    InputConfiguratorController* selectProfile();

protected:
    std::vector<ProfileSelection> m_gamepadsToSelect;

};
#endif
