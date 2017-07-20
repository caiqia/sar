#ifndef DEF_GAMEPAD
#define DEF_GAMEPAD

#include <SDL2/SDL.h>

#include <string>
#include <vector>
#include <map>

//Enum representing the possible type of inputs found on a gamepad
enum GamepadControl
{
    BUTTON,
    AXIS_POS,
    AXIS_NEG,
    HAT_UP,
    HAT_DOWN,
    HAT_LEFT,
    HAT_RIGHT
};

//enum for the different type of inputs that can be interpreted
enum GamepadActions
{
    UP,
    DOWN,
    RIGHT,
    LEFT,
    LUP,
    LDOWN,
    LLEFT,
    LRIGHT,
    RUP,
    RDOWN,
    RLEFT,
    RRIGHT,
    A,
    B,
    X,
    Y,
    R1,
    L1,
    R2,
    L2,
    R3,
    L3,
    START,
    SELECT,
    MAGIC
};

//Struct used to get the status from an input of the gamepad
struct GamepadStatus
{
    GamepadActions action;
    bool pushed;
};

//Struct used to store the configuration of a gamepad button/axis
struct GamepadConfig
{
    GamepadControl control;
    int number;

    bool operator<(const GamepadConfig &conf)  const
    {
        return control < conf.control  || ( control == conf.control && number < conf.number);
    }
};

class Gamepad
{
public:
    Gamepad(int device_index, std::string profile);
    ~Gamepad();

    int loadConfig();

    std::vector<GamepadStatus> querryGamePad();

    SDL_Joystick* getJoystick();
    Gamepad* setJoystick(SDL_Joystick* joystick);

    int getNumber();
    Gamepad* setNumber(int number);

    SDL_JoystickID getInstanceID();

    std::string getGuid();
    Gamepad* setGuid(std::string guid);

    std::string getProfile();
    Gamepad* setProfile(std::string profile);

    Gamepad* clearConfig();
    Gamepad* addConfig(GamepadConfig config, GamepadActions action);
    std::multimap<GamepadConfig, GamepadActions> getConfig();

    Gamepad* setInit(bool init);
    bool getInit();

    static int count;
    static std::map<GamepadActions, std::string> GamepadActionsRetroarchResolver;

protected:
    SDL_Joystick* m_joystick;
    int m_number;
    std::string m_guid;
    std::string m_profile;
    std::multimap<GamepadConfig, GamepadActions> m_config;
    bool m_init = 0;

    static std::map<std::string, GamepadControl> GamepadControlResolver;
    static std::map<std::string, GamepadActions> GamepadActionsResolver;
    static std::map<GamepadActions, std::string> GamepadActionsReverser;
};



#endif
