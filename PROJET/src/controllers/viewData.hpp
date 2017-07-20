#ifndef DEF_VIEWDATA
#define DEF_VIEWDATA

#include "../entities/system.hpp"
#include "../entities/menuEntities.hpp"
#include "enums/currentMenuEnum.hpp"
#include "enums/keysEnum.hpp"

#include <vector>
#include <map>

enum OverlayLevel
{
    NORMAL,
    MENU,
    VISUAL_KEYBOARD,
    POPUP_WINDOW,
    INPUT_POPUP_WINDOW,
    ERROR,
    GAMEPAD_PROFILE_SELECTION,
};

//enum for possible views
enum CurrentViews
{
    HOME,
    GAME_SELECT
};

struct ViewData
{
    std::map<int, const System*> systems;
    int selectedSystem = 0;
    int numberSystems = 0;

    std::map<int, int> selectedGames;

    CurrentViews currentView = CurrentViews::HOME;

    OverlayLevel overlayLevel = OverlayLevel::NORMAL;

    bool menuActive = false;
    std::string menuTitle;
    CurrentMenu currentMenu = CurrentMenu::MAIN_MENU;
    int selectedItem = 0;
    int previouslySelectedItem = -1;
    std::map<CurrentMenu, std::map<int, const MenuEntity*>> menuEntities;

    bool displayVisualKeyboard = false;
    std::string visualKeyboardInput = "";
    KeyboardPannel currentPannel = KeyboardPannel::MAIN;
    Keys currentKey = Keys::KEY1;
    bool shiftActive = false;
    std::map<KeyboardPannel, std::map<Keys, std::string>> keyMap;

    bool displayPopupWindow = false;
    std::string popupWindowTitle;
    std::string popupWindowContent;
    std::map<int, std::string> popupWindowButtons;
    int popupWindowSelectedButton;

    bool displayInputPopupWindow = false;
    std::string inputPopupWindowTitle;
    std::string inputPopupWindowHelpText;
    std::string inputPopupWindowInput;
    std::map<int, std::string> inputPopupWindowButtons;
    int inputPopupWindowSelectedButton;
    bool inputPopupWindowSelectedButtons = false;

    std::string errorMessage;
    std::string errorCode;
    bool executionerrorAffich = false;
    bool majorerrorAffich = false;


    bool displayGamepadProfileSelection = false;
    std::vector<std::string> profilesToChoseFrom;
    int configuredGamepadNumber = 0;
    int currentlySelectedProfile = 0;

    int loadingScreenPercentage = 0;
};

#endif
