#include <iostream>
#include <fstream>
#include<regex>

#include "gamepad.hpp"
#include "log.hpp"
#include "error.hpp"

using namespace std;

//Gamepad Object, stores informations for a gamepad

//Static attributes intitilization
int Gamepad::count = 0;
map<string, GamepadControl> Gamepad::GamepadControlResolver = {{"BUTTON", GamepadControl::BUTTON}, {"AXIS_POS", GamepadControl::AXIS_POS}, {"AXIS_NEG", GamepadControl::AXIS_NEG}, {"HAT_UP", GamepadControl::HAT_UP}, {"HAT_DOWN", GamepadControl::HAT_DOWN}, {"HAT_LEFT", GamepadControl::HAT_LEFT}, {"HAT_RIGHT", GamepadControl::HAT_RIGHT}};
map<string, GamepadActions> Gamepad::GamepadActionsResolver = {{"UP", GamepadActions::UP}, {"DOWN", GamepadActions::DOWN}, {"RIGHT", GamepadActions::RIGHT}, {"LEFT", GamepadActions::LEFT}, {"LUP", GamepadActions::LUP}, {"LDOWN", GamepadActions::LDOWN}, {"LRIGHT", GamepadActions::LRIGHT}, {"LLEFT", GamepadActions::LLEFT}, {"RUP", GamepadActions::RUP}, {"RDOWN", GamepadActions::RDOWN}, {"RLEFT", GamepadActions::RLEFT}, {"RRIGHT", GamepadActions::RRIGHT}, {"A", GamepadActions::A}, {"B", GamepadActions::B}, {"X", GamepadActions::X}, {"Y", GamepadActions::Y}, {"R1", GamepadActions::R1}, {"L1", GamepadActions::L1}, {"R2", GamepadActions::R2}, {"L2", GamepadActions::L2}, {"R3", GamepadActions::R3}, {"L3", GamepadActions::L3}, {"START", GamepadActions::START}, {"SELECT", GamepadActions::SELECT}, {"MAGIC", GamepadActions::MAGIC}};
map<GamepadActions, string> Gamepad::GamepadActionsReverser = {{GamepadActions::UP, "UP"}, {GamepadActions::DOWN, "DOWN"}, {GamepadActions::RIGHT, "RIGHT"}, {GamepadActions::LEFT, "LEFT"}, {GamepadActions::LUP, "LUP"}, {GamepadActions::LDOWN, "LDOWN"}, {GamepadActions::LRIGHT, "LRIGHT"}, {GamepadActions::LLEFT, "LLEFT"}, {GamepadActions::RUP, "RUP"}, {GamepadActions::RDOWN, "RDOWN"}, {GamepadActions::RLEFT, "RLEFT"}, {GamepadActions::RRIGHT, "RRIGHT"}, {GamepadActions::A, "A"}, {GamepadActions::B, "B"}, {GamepadActions::X, "X"}, {GamepadActions::Y, "Y"}, {GamepadActions::R1, "R1"}, {GamepadActions::L1, "L1"}, {GamepadActions::R2, "R2"}, {GamepadActions::L2, "L2"}, {GamepadActions::R3, "R3"}, {GamepadActions::L3, "L3"}, {GamepadActions::START, "START"}, {GamepadActions::SELECT, "SELECT"}, {GamepadActions::MAGIC, "MAGIC"}};
//To transfrom to a retroarch type config
map<GamepadActions, string> Gamepad::GamepadActionsRetroarchResolver = {{GamepadActions::UP, "up"}, {GamepadActions::DOWN, "down"}, {GamepadActions::RIGHT, "right"}, {GamepadActions::LEFT, "left"}, {GamepadActions::LUP, "l_y_plus"}, {GamepadActions::LDOWN, "l_y_minus"}, {GamepadActions::LRIGHT, "l_x_plus"}, {GamepadActions::LLEFT, "l_x_minus"}, {GamepadActions::RUP, "r_y_plus"}, {GamepadActions::RDOWN, "r_y_minus"}, {GamepadActions::RLEFT, "r_x_minus"}, {GamepadActions::RRIGHT, "r_x_plus"}, {GamepadActions::A, "a"}, {GamepadActions::B, "b"}, {GamepadActions::X, "x"}, {GamepadActions::Y, "y"}, {GamepadActions::R1, "r"}, {GamepadActions::L1, "l"}, {GamepadActions::R2, "r2"}, {GamepadActions::L2, "l2"}, {GamepadActions::R3, "r3"}, {GamepadActions::L3, "l3"}, {GamepadActions::START, "start"}, {GamepadActions::SELECT, "select"}};


//Gamepad Init
//TODO manage init gone wrong
Gamepad::Gamepad(int device_index, string profile)
{
    //Open gamepad through SDL
    this->setJoystick(SDL_JoystickOpen(device_index));


    //If correctly openned
    if(this->getJoystick())
    {
        this->setNumber(Gamepad::count);
        Gamepad::count ++;

        //Get GUID from SDL
        SDL_JoystickGUID guid = SDL_JoystickGetGUID(this->getJoystick());

        //transfrom GUID to string
        char guid_str[1024];
        SDL_JoystickGetGUIDString(guid, guid_str, sizeof(guid_str));
        this->setGuid(guid_str);

        //Set gamepad config profile
        this->setProfile(profile);

        //if config could be loaded, gamepad is initialized
        if(this->loadConfig())
        {
            this->setInit(1);
            Log::addEntry(1, "Gamepad #" + to_string(this->getNumber()) + " loaded | instance : " + to_string(this->getInstanceID()) + " | profile : " + this->getProfile() + " | GUID : " + this->getGuid());
        }
        //Gamepad config could not be loaded
        else
        {
          Log::addEntry(1, "/!\\ Could not load gamepad config | profile : " + this->getProfile() + " | GUID : " + this->getGuid());
        }

    }
    //If could not open gamepad
    else
    {

      Log::addEntry(1, "/!\\ Could not open gamepad #" + to_string(device_index));
    }
}

Gamepad::~Gamepad()
{
    Log::addEntry(1, "Removed gamepad #" + to_string(this->getNumber()) + " | instance : " + to_string(this->getInstanceID()) + " | profile : " + this->getProfile() + " | GUID : " + this->getGuid());
    if (this->getInit())
    {

    }
    else
    {

    }
    // CAUSES BUGS BUT REMOVING COMMENTS MIGHT CAUSE MEMORY LEAKS /!\ .

    /*cout << "Attempting to remove gamepad #" << this->getNumber() << endl;

    SDL_JoystickGetAttached was causing the program to crash /!\ must inspect further

    if(SDL_JoystickGetAttached(this->getJoystick()))
    {
        cout << "Removing gamepad #" << this->getNumber() << endl;
        SDL_JoystickClose(this->getJoystick());
    }
    else
    {
        cout << "Could not close Gamepad #" << this->getNumber() << endl;
    }*/

}

int Gamepad::loadConfig()
{
    if(this->getProfile() != "multiple" && this->getProfile() != "null")
    {
        //Open for read gamepad config file
        ifstream configFile("/nostal/inputconfig.conf", std::ios::in);

        if(configFile)
        {
            string ligne;

            //Regex to analyze gamepad config format
            //Begining of a new gamepad is : DEVICE: %GUID% : %PROFILE%
            //Config line format is : %GamepadAction% : %GamepadControl% : %Button/Axis Number%
            regex rgx("^([a-zA-Z0-9_]+):([a-zA-Z0-9_]+):([a-zA-Z0-9_]+)$");
            smatch match;
            string readDevice = "";
            string readProfile = "";
            bool configFound = 0;

            //Remove what was previously configured
            this->clearConfig();

            //Read all config lines and check if they match with the regex
            while(getline(configFile, ligne))
            {
                //only read if the line is not a comment or blank
                if(ligne != "" && ligne.at(0) != '#' )
                {
                    if(regex_search(ligne, match, rgx))
                    {

                        //If it's a device that announces a new device
                        if(match[1] == "DEVICE")
                        {
                            readDevice = match[2]; //GUID
                            readProfile = match[3]; //PROFILE
                        }
                        //It's an input config line for the previously declared device
                        else
                        {
                            //If we are reading a config line for the current device
                            if(readDevice == this->getGuid() && readProfile == this->getProfile())
                            {
                                configFound = 1;

                                //We create the GamepadActions and GamepadConfig structure to link an action to a config
                                GamepadActions mapping;
                                GamepadConfig conf;

                                //If the GamepadActions given in the config file doesn't match an available one
                                if(Gamepad::GamepadActionsResolver.find(match[1]) == Gamepad::GamepadActionsResolver.end())
                                {
                                  std::string match1 = match[1];

                                  Log::addEntry(1, "/!\\ Could not understand mapping for : " + match1 + "\nLine : " + ligne);
                                }
                                //If the GamepadActions given is a correct one we save our mapping
                                else
                                {
                                    mapping = Gamepad::GamepadActionsResolver[match[1]];
                                }
                                //If the equivalent GamepadControl given by the config file is not a valid one
                                if(Gamepad::GamepadControlResolver.find(match[2]) == Gamepad::GamepadControlResolver.end())
                                {
                                    std::string match2 = match[2];

                                    Log::addEntry(1, "/!\\ Could not understand control : " + match2 + "\nLine : " + ligne);
                                }
                                //If it is a valid one, we store it as the control attribute of the our GamepadConfig struct
                                else
                                {
                                    conf.control = GamepadControlResolver[match[2]];
                                }

                                //Third match is the button/axis number part of our GamepadConfig struct
                                conf.number = std::stoi(match[3]);

                                //TODO Check that the line was correctly read before saving the read info
                                this->addConfig(conf, mapping);
                            }
                        }
                    }

                    //If it doesn't match regex
                    else
                    {
                        //TODO allow for comments and blank lines

                        Log::addEntry(1, "/!\\ Could not read correctly config file on line : " + ligne);
                        return 0;
                    }
                }
            }
            //If a config was found for the current device return 1
            //TODO check that at least the minimum btns/axis were configured (UP/DOWN/RIGHT/LEFT/A/B/START/SELECT)
            if(configFound)
            {
                return 1;
            }
            //If no config was found for the current device return 0
            else
            {

              Log::addEntry(1, "/!\\ No config was found for the controller | profile = " + this->getProfile() + " | GUID = " + this->getGuid());
                return 0;
            }
        }
        //If input file could not be oppened
        else
        {

          Log::addEntry(1, "/!\\ Could not open input config file");
            return 0;
        }
    }
    else //Load default profile
    {
            GamepadConfig conf;
            GamepadActions action;

            conf.control = GamepadControl::AXIS_NEG;
            conf.number = 0;
            action = GamepadActions::LEFT;
            this->addConfig(conf, action);

            conf.control = GamepadControl::AXIS_POS;
            conf.number = 0;
            action = GamepadActions::RIGHT;
            this->addConfig(conf, action);

            conf.control = GamepadControl::AXIS_NEG;
            conf.number = 1;
            action = GamepadActions::UP;
            this->addConfig(conf, action);

            conf.control = GamepadControl::AXIS_POS;
            conf.number = 1;
            action = GamepadActions::DOWN;
            this->addConfig(conf, action);

            conf.control = GamepadControl::BUTTON;
            conf.number = 0;
            action = GamepadActions::B;
            this->addConfig(conf, action);

            conf.control = GamepadControl::BUTTON;
            conf.number = 1;
            action = GamepadActions::A;
            this->addConfig(conf, action);

            conf.control = GamepadControl::BUTTON;
            conf.number = 2;
            action = GamepadActions::X;
            this->addConfig(conf, action);


            conf.control = GamepadControl::BUTTON;
            conf.number = 3;
            action = GamepadActions::Y;
            this->addConfig(conf, action);

            return 1;
    }
}

//querryGamePad function
// Use : to get a full map of the value from the gamepad buttons/axis
// Returns : a vector containing each buttons/axis GamepadStatus(GamepadActions, bool pushed)
vector<GamepadStatus> Gamepad::querryGamePad()
{
    //We create the vector that will be returned
    vector<GamepadStatus> statusVect;

    //We go through all the keys and axis configured
    for(auto const it : this->getConfig())
    {
        //We create the GamepadStatus specific to the current key/axis, that will be inserted in ou returned vector
        GamepadStatus status;

        //If we are working on a button
        if(it.first.control == GamepadControl::BUTTON)
        {
            //We simply set our pushed value to the one returned by SDL for the current joystick and button
            status.pushed = SDL_JoystickGetButton(this->getJoystick(), it.first.number);
        }
        //If we are working on an axis
        else if(it.first.control == GamepadControl::AXIS_POS || it.first.control == GamepadControl::AXIS_NEG)
        {
            //We get the current value of our axis from SDL
            int axisValue = SDL_JoystickGetAxis(this->getJoystick(), it.first.number);

            //TODO add sensibility as some joysticks can send non 0 values on center position

            //If the current value of the axis is POSITIVE and we are looking for positive value of the axis
            if(it.first.control == GamepadControl::AXIS_POS && axisValue > 0)
            {
                status.pushed = 1;
            }
            //If the current value of the axis is NEGATIVE and we are looking for negative value of the axis
            else if(it.first.control == GamepadControl::AXIS_NEG && axisValue < 0)
            {
                status.pushed = 1;
            }
            //The axis is not in the position of the config were looking at
            else
            {
                status.pushed = 0;
            }
        }
        //If we're working on a hat
        else if(it.first.control == GamepadControl::HAT_UP || it.first.control == GamepadControl::HAT_DOWN || it.first.control == GamepadControl::HAT_LEFT || it.first.control == GamepadControl::HAT_RIGHT)
        {
            //We get the hat position
            Uint8 hatValue = SDL_JoystickGetHat(this->getJoystick(), it.first.number);

            //We then confront it to the config and determine it's status
            if(it.first.control == GamepadControl::HAT_UP)
            {
                if(hatValue == SDL_HAT_UP || hatValue == SDL_HAT_RIGHTUP || hatValue == SDL_HAT_LEFTUP)
                {
                    status.pushed = 1;
                }
                else
                {
                    status.pushed = 0;
                }
            }
            else if(it.first.control == GamepadControl::HAT_RIGHT)
            {
                if(hatValue == SDL_HAT_RIGHT || hatValue == SDL_HAT_RIGHTUP || hatValue == SDL_HAT_RIGHTDOWN)
                {
                    status.pushed = 1;
                }
                else
                {
                    status.pushed = 0;
                }
            }
            else if(it.first.control == GamepadControl::HAT_DOWN)
            {
                if(hatValue == SDL_HAT_DOWN || hatValue == SDL_HAT_LEFTDOWN || hatValue == SDL_HAT_RIGHTDOWN)
                {
                    status.pushed = 1;
                }
                else
                {
                    status.pushed = 0;
                }
            }
            else if(it.first.control == GamepadControl::HAT_LEFT)
            {
                if(hatValue == SDL_HAT_LEFT || hatValue == SDL_HAT_LEFTDOWN || hatValue == SDL_HAT_LEFTUP)
                {
                    status.pushed = 1;
                }
                else
                {
                    status.pushed = 0;
                }
            }
        }

        //The action is the one given in our config
        ///We push our status to our array, and return the array after we've been through all configs
        status.action = it.second;
        statusVect.push_back(status);
    }

    return statusVect;
}

SDL_Joystick* Gamepad::getJoystick()
{
    return this->m_joystick;
}

Gamepad* Gamepad::setJoystick(SDL_Joystick* joystick)
{
    this->m_joystick = joystick;
    return this;
}

int Gamepad::getNumber()
{
    return this->m_number;
}

Gamepad* Gamepad::setNumber(int number)
{
    this->m_number = number;
    return this;
}

SDL_JoystickID Gamepad::getInstanceID()
{
    SDL_JoystickID id = SDL_JoystickInstanceID(this->getJoystick());
    if(id < 0)
    {
        return 0;
    }
    else
    {
        return id;
    }
}

string Gamepad::getGuid()
{
    return this->m_guid;
}

Gamepad* Gamepad::setGuid(string guid)
{
    this->m_guid = guid;
    return this;
}

string Gamepad::getProfile()
{
    return this->m_profile;
}

Gamepad* Gamepad::setProfile(string profile)
{
    this->m_profile = profile;
    return this;
}

Gamepad* Gamepad::clearConfig()
{
    this->m_config.clear();
    return this;
}

Gamepad* Gamepad::addConfig(GamepadConfig config, GamepadActions action)
{
    this->m_config.insert(pair<GamepadConfig, GamepadActions>(config, action));
    return this;
}

multimap<GamepadConfig, GamepadActions> Gamepad::getConfig()
{
    return this->m_config;
}

Gamepad* Gamepad::setInit(bool init)
{
    this->m_init = init;
    return this;
}

bool Gamepad::getInit()
{
    return this->m_init;
}
