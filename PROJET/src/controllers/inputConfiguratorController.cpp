#include <regex>
#include <iostream>

#include "inputConfiguratorController.hpp"
#include "controller.hpp"
#include "../entities/log.hpp"
#include "../entities/error.hpp"
#include "../entities/gamepad.hpp"

using namespace std;

//Controller that manages the user's choice of gamepad profile
// Also used for creation of a custom profile

//TODO manage in case it's not a supported gamepad

//Init
InputConfiguratorController::InputConfiguratorController(Controller* controller)
: AbstractController(controller, INPUT_CONFIGURATOR_CONTROLLER)
{
    Log::addEntry(1, "InputConfiguratorController initialized successfully");
}

InputConfiguratorController::~InputConfiguratorController()
{
    Log::addEntry(1, "Deleted InputConfiguratorController");
}

//General control loop sends the current data to the viewData
InputConfiguratorController* InputConfiguratorController::control()
{
    AbstractController::control();

    //If the gamepad configuration cue is not empty, we take the first one and set it as the one to configure
    if(this->getGamepadsToSelect().size() > 0)
    {
        ProfileSelection selection = this->getFirstGamepadToSelect();

        //We avoid errors where a null gamepad would have been inserted
        if(selection.gamepad != nullptr)
        {
            this->getController()->viewData.profilesToChoseFrom = selection.profiles;
            this->getController()->viewData.configuredGamepadNumber = selection.gamepad->getNumber();
            this->getController()->viewData.currentlySelectedProfile = selection.selected;
            this->getController()->viewData.displayGamepadProfileSelection = true;
            this->getController()->setOverlayLevel(OverlayLevel::GAMEPAD_PROFILE_SELECTION);
        }
        else
        {
            this->removeFirstGamepadtoSelect();
        }

    }
    else
    {
        this->getController()->viewData.displayGamepadProfileSelection = false;
    }

    return this;
}

//controller when the gamepad profile selection menu appears
InputConfiguratorController* InputConfiguratorController::inputControl()
{
    AbstractController::inputControl();

    for(auto& ent : this->getController()->getAdvancedControls())
    {
        //We only detect the controls from the controller to set-up
        if(this->getFirstGamepadToSelect().gamepad != nullptr && ent.first->getNumber() == this->getFirstGamepadToSelect().gamepad->getNumber())
        {
            if(this->getController()->getGamepadControl(ent.first, GamepadActions::B) || this->getController()->getGamepadControl(ent.first, GamepadActions::A) || this->getController()->getGamepadControl(ent.first, GamepadActions::START) || this->getController()->getGamepadControl(ent.first, GamepadActions::X) || this->getController()->getGamepadControl(ent.first, GamepadActions::Y))
            {
                this->selectProfile();
            }
            else if(this->getController()->getGamepadControl(ent.first, GamepadActions::RIGHT))
            {
                this->nextProfile();
            }
            else if(this->getController()->getGamepadControl(ent.first, GamepadActions::LEFT))
            {
                this->previousProfile();
            }
        }

    }

    return this;
}

//Function that fetches the profiles available for a given controller
//Returns : the profiles to use, or "multiple" if there is more then one profile available, or null if the controller is not supported
string InputConfiguratorController::getProfileFromIndex(int deviceIndex)
{
    SDL_JoystickGUID guid = SDL_JoystickGetDeviceGUID(deviceIndex);
    char guid_str[1024];
    SDL_JoystickGetGUIDString(guid, guid_str, sizeof(guid_str));

    int profilesForGUID = this->getNumberProfilesForGUID(guid_str);

    if(profilesForGUID == 1)
    {
        return this->getSingleProfileFromGUID(guid_str);
    }
    else if(profilesForGUID > 1)
    {
        return "multiple";
    }
    else
    {

        Log::addEntry(1, "/!\\ No known profile for controller !");
        return "null";
    }
}

//count the number of profiles available for a given GUID
int InputConfiguratorController::getNumberProfilesForGUID(string GUID)
{
    //Open for read gamepad config file
    ifstream configFile("/nostal/inputconfig.conf", std::ios::in);

    int counter = 0;

    //We cycle through the file and count the number of profiles for a GUID

    if(configFile)
    {
        string ligne;

        regex rgx("^([a-zA-Z0-9_]+):([a-zA-Z0-9_]+):([a-zA-Z0-9_]+)$");
        smatch match;

        while(getline(configFile, ligne))
        {
            if(regex_search(ligne, match, rgx))
            {
                if(match[2] == GUID)
                {
                    ++counter;
                }
            }
        }
    }
    //If we could not open the file
    else
    {

        Log::addEntry(1, "/!\\ Could not load inputconfig.conf in function getNumberProfilesForGUID");
    }


    return counter;
}

//Returns the first profile it finds for a GUID or NULL if none exists
string InputConfiguratorController::getSingleProfileFromGUID(string GUID)
{
    //Open for read gamepad config file
    ifstream configFile("/nostal/inputconfig.conf", std::ios::in);

    string profile = "";

    //We cycle through the file and return the first profile with a GUID
    if(configFile)
    {
        string ligne;

        regex rgx("^([a-zA-Z0-9_]+):([a-zA-Z0-9_]+):([a-zA-Z0-9_]+)$");
        smatch match;

        while(getline(configFile, ligne))
        {
            if(regex_search(ligne, match, rgx))
            {
                if(match[2] == GUID)
                {
                    profile = match[3];
                }
            }
        }
    }
    //If we could not open the file
    else
    {

        Log::addEntry(1, "/!\\ Could not load inputconfig.conf in function getSingleProfileFromGUID");
    }

    if(profile != "")
    {
        return profile;
    }
    else
    {
        return "";
    }
}

//Returns a vector containing all given profiles available for a given GUID
vector<string> InputConfiguratorController::getProfilesFromGUID(string GUID)
{
    vector<string> profiles;
    //Open for read gamepad config file
    ifstream configFile("/nostal/inputconfig.conf", std::ios::in);

    //We cycle through the file and return the first profile with a GUID
    if(configFile)
    {
        string ligne;

        regex rgx("^([a-zA-Z0-9_]+):([a-zA-Z0-9_]+):([a-zA-Z0-9_]+)$");
        smatch match;

        while(getline(configFile, ligne))
        {
            if(regex_search(ligne, match, rgx))
            {
                if(match[2] == GUID)
                {
                    profiles.push_back(match[3]);
                }
            }
        }
    }
    //If we could not open the file
    else
    {

        Log::addEntry(1, "/!\\ Could not load inputconfig.conf in function getSingleProfileFromGUID");
    }

    return profiles;
}

//Adds the given gamepad to the Profile selection Queue
InputConfiguratorController* InputConfiguratorController::selectProfile(Gamepad* gamepad)
{

    vector<string> profiles = this->getProfilesFromGUID(gamepad->getGuid());

    if(profiles.size() > 0)
    {
        ProfileSelection profileSelection;
        profileSelection.gamepad = gamepad;
        profileSelection.profiles = profiles;
        profileSelection.selected = 0;
        this->addGamepadToSelect(profileSelection);
    }
    else
    {

        Log::addEntry(1, "/!\\ Could not find any profiles for GUID in selectProfile function");
    }
    return this;
}

vector<ProfileSelection> InputConfiguratorController::getGamepadsToSelect()
{
    return this->m_gamepadsToSelect;
}

//Returns the first gamepad from the ProfileSelection queue
ProfileSelection InputConfiguratorController::getFirstGamepadToSelect()
{
    if(this->m_gamepadsToSelect.size() > 0)
    {
        return this->m_gamepadsToSelect.front();
    }
    else
    {
        ProfileSelection profileSelection;
        profileSelection.gamepad = nullptr;
        return profileSelection;
    }
}

//Adds a gamepad at the end of the profile selection queue
InputConfiguratorController* InputConfiguratorController::addGamepadToSelect(ProfileSelection profileSelection)
{
    this->m_gamepadsToSelect.push_back(profileSelection);
    return this;
}

//Removes the first gamepad from the profile selection queue
InputConfiguratorController* InputConfiguratorController::removeFirstGamepadtoSelect()
{
    if(this->m_gamepadsToSelect.size() > 0)
    {
        this->m_gamepadsToSelect.erase(this->m_gamepadsToSelect.begin());
    }
    return this;
}

//Checks if a given gamepad is in the profile selection queue
bool InputConfiguratorController::isGamepadInSelectQueue(int gamepadNumber)
{
    bool gamepadInVect = 0;

    for(auto& ent : this->getGamepadsToSelect())
    {
        if(ent.gamepad != nullptr && ent.gamepad->getNumber() == gamepadNumber)
        {
            gamepadInVect = 1;
            break;
        }
    }

    return gamepadInVect;
}

//Removes a given gamepad from the profile seleciton queue
InputConfiguratorController* InputConfiguratorController::removeGamepadFromSelectQueue(int gamepadNumber)
{
    for(auto it = this->m_gamepadsToSelect.begin(); it != this->m_gamepadsToSelect.end(); ++it)
    {
        if(it->gamepad != nullptr && it->gamepad->getNumber() == gamepadNumber)
        {
            this->m_gamepadsToSelect.erase(it);
            break;
        }
    }
    return this;
}

//Transforms a vector into a string with coma separated values
string InputConfiguratorController::profilesToString(vector<string> profiles)
{
    string profilesStr = "";
    int i = 0;

    for(auto & ent : profiles)
    {
        if(i != 0)
        {
            profilesStr += ",";
        }

        profilesStr += ent;

        ++i;
    }

    return profilesStr;
}

//moves the selector for the current gamepad profile selection
InputConfiguratorController* InputConfiguratorController::nextProfile()
{
    int selected = this->getFirstGamepadToSelect().selected;
    vector<string> profiles = this->getFirstGamepadToSelect().profiles;

    if(selected == (profiles.size() - 1) )
    {
        this->m_gamepadsToSelect[0].selected = 0;
    }
    else
    {
        this->m_gamepadsToSelect[0].selected = ++selected;
    }
}

//moves the selector for the current gamepad profile selection
InputConfiguratorController* InputConfiguratorController::previousProfile()
{
    int selected = this->getFirstGamepadToSelect().selected;
    vector<string> profiles = this->getFirstGamepadToSelect().profiles;

    if(selected == 0)
    {
        this->m_gamepadsToSelect[0].selected = profiles.size() - 1;
    }
    else
    {
        this->m_gamepadsToSelect[0].selected = --selected;
    }
}

//Selects the current selected profile as the one to apply to the gamepad being configured
InputConfiguratorController* InputConfiguratorController::selectProfile()
{
    ProfileSelection profileSelection = this->getFirstGamepadToSelect();

    if(profileSelection.gamepad == nullptr)
    {
        //TODO
    }
    else
    {
        profileSelection.gamepad->setProfile(profileSelection.profiles[profileSelection.selected])->loadConfig();
        this->removeFirstGamepadtoSelect();
        Log::addEntry(3, "Selected profile : " + profileSelection.profiles[profileSelection.selected] + " for gamepad #" + to_string(profileSelection.gamepad->getNumber()));
    }
}
