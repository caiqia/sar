
#include "gameLaunchController.hpp"
#include "controller.hpp"
#include "inputController.hpp"
#include "../entities/system.hpp"
#include "../entities/log.hpp"
#include "../views/view.hpp"
#include "../entities/gamepad.hpp"
#include "../entities/error.hpp"


#include <string>
#include <regex>
#include <iostream>
#include <fstream>
#include <SDL2/SDL.h>
#include <boost/filesystem.hpp>

using namespace std;
namespace fs = boost::filesystem;

//Controller who's aim is to launch games


//Basic init
GameLaunchController::GameLaunchController(Controller* controller)
: AbstractController(controller, GAME_LAUNCH_CONTROLLER)
{

}

GameLaunchController::~GameLaunchController()
{

}

GameLaunchController* GameLaunchController::control()
{
    AbstractController::control();

    return this;
}

GameLaunchController* GameLaunchController::inputControl()
{
    AbstractController::inputControl();

    return this;
}

//Launch game with this function
GameLaunchController* GameLaunchController::launch(Game* game)
{
    //Get launch command from system config
    string launchCmd = game->getSystem()->getLaunchCmd();

    //Replace %OPT% with options, and %ROM% with path to the rom
    launchCmd = regex_replace(launchCmd, regex("%OPT%"), "--fullscreen --config \"/etc/retroarch.cfg\" --verbose");

    launchCmd = regex_replace(launchCmd, regex("%ROM%"), "\"" + game->getPath() + "\"");

    //Create retroarch.cfg file corresponding to game settings
    this->createConfig(game);

    //Delete window to let the emulator take the place
    this->getController()->getView()->deInitWindow();

    InputController* inputController = static_cast <InputController*>(this->getController()->getController(INPUT_CONTROLLER));
    if(inputController != nullptr)
    {
        inputController->getInputStatus().clear();
    }

    Log::addEntry(3, "Launching : " + game->getPath());

    //Launch !!! and pause...
    system(launchCmd.c_str());

    //this->getController()->getInputController()->resetGamepad();

    Log::addEntry(3, "Game stopped");

    //Recreate window fully now that emulation as stopped
    this->getController()->getView()->reInitWindow();


    Log::addEntry(3, "Environment fully loaded");

    return this;

}

//Function used to create a custom retroarch.cfg for a game before launching it
GameLaunchController* GameLaunchController::createConfig(Game* game)
{
    //Open for writing retroarch.cfg
    ofstream retroarchCfg("/etc/retroarch.cfg", ios::out | ios::trunc);

    //Write config
    if(retroarchCfg)
    {
        retroarchCfg << "##Temporary config file##" << endl;
        retroarchCfg << "###GENERIC" << endl;
        retroarchCfg << "savefile_directory = \"" << game->getSystem()->getSavePath() << "\"" << endl;
        retroarchCfg << "savestate_directory = \"" << game->getSystem()->getSavePath() << "\"" << endl;
        retroarchCfg << "savestate_auto_save = false" << endl;
        retroarchCfg << "savestate_auto_load = true" << endl;
        retroarchCfg << "libretro_log_level = 1" << endl;
        retroarchCfg << "log_verbosity = false" << endl;
        retroarchCfg << "content_history_dir = \"/nostal/\"" << endl; //TODO get more look into retroarch content history
        retroarchCfg << "history_list_enable = true" << endl;
        retroarchCfg << "content_history_size = 100" << endl;


        //If BIOS files are correct indicate directory
        if(game->getSystem() != nullptr)
        {
            if(game->getSystem()->getReqiresBios() > ACCEPTS_NO_BIOS)
            {
                retroarchCfg << "system_directory = \"" << game->getSystem()->getBiosPath() << "\"" << endl;
            }
        }

        retroarchCfg << "rgui_browser_directory = \"/nostal/Jeux\"";
        retroarchCfg << "rgui_show_start_screen = false" << endl;
        retroarchCfg << "config_save_on_exit = false" << endl;

        //TODO Tweak arround with audio and video parameters to enhance speed
        retroarchCfg << "###VIDEO" << endl;
        retroarchCfg << "video_driver = \"gl\"";
        retroarchCfg << "video_fullscreen_x = 0" << endl;
        retroarchCfg << "video_fullscreen_y = 0" << endl;
        retroarchCfg << "video_fullscreen = true" << endl;
        retroarchCfg << "video_windowed_fullscreen = false" << endl;
        retroarchCfg << "video_monitor_index = 0" << endl;
        retroarchCfg << "video_vsync = true" << endl;
        retroarchCfg << "video_threaded = true" << endl;
        retroarchCfg << "video_shared_context = false" << endl;
        retroarchCfg << "video_smooth = true" << endl;
        retroarchCfg << "video_force_aspect = false" << endl;
        retroarchCfg << "video_scale_integer = true" << endl;
        retroarchCfg << "video_aspect_ratio_auto = true" << endl;
        retroarchCfg << "video_font_path = \"/nostal/fonts/roboto.ttf\"" << endl;
        retroarchCfg << "video_font_size = 32" << endl;

        if(this->getController()->getDevMode())
        {
            retroarchCfg << "video_font_enable = true" << endl;
        }
        else
        {
            retroarchCfg << "video_font_enable = false" << endl;
        }

        retroarchCfg << "video_message_pos_x = 0.05" << endl;
        retroarchCfg << "video_message_pos_y = 0.05" << endl;
        retroarchCfg << "video_message_color = fff10d" << endl;
        retroarchCfg << "video_refresh_rate = " << this->getController()->getView()->getWindow()->getDisplayRate() << endl;
        retroarchCfg << "video_allow_rotate = false" << endl;
        retroarchCfg << "video_rotation = 0" << endl;

        retroarchCfg << "###Audio" << endl;
        retroarchCfg << "audio_enable = true" << endl;
        retroarchCfg << "audio_mute_enable = false" << endl;
        retroarchCfg << "audio_driver = alsathread" << endl;
        retroarchCfg << "audio_device = \"hw:0,0\"" << endl;
        retroarchCfg << "audio_out_rate = 48000" << endl;
        retroarchCfg << "audio_latency = 64" << endl;
        retroarchCfg << "audio_sync = true" << endl;
        retroarchCfg << "audio_rate_control = true" << endl;
        retroarchCfg << "audio_rate_control_delta = 0.005" << endl;
        retroarchCfg << "audio_max_timing_skew = 0.05" << endl;
        retroarchCfg << "audio_volume = 0.0" << endl;

        retroarchCfg << "###MISC" << endl;
        retroarchCfg << "rewind_enable = false" << endl;
        retroarchCfg << "screenshot_directory = \"/nostal/Screenshot/\"" << endl;

        retroarchCfg << "###INPUT" << endl;
        retroarchCfg << "input_driver = udev" << endl;
        retroarchCfg << "input_max_users = 4" << endl;
        retroarchCfg << "input_axis_threshold = 0.3" << endl;
        retroarchCfg << "input_autodetect_enable = true" << endl;
        retroarchCfg << "input_descriptor_label_show = true" << endl;
        retroarchCfg << "input_descriptor_hide_unbound = false" << endl;
        retroarchCfg << "input_poll_type_behavior = 1" << endl;


        //Get all plugged in gamepads
        vector<Gamepad*> gamepads;

        InputController* inputController = static_cast <InputController*>(this->getController()->getController(INPUT_CONTROLLER));
        if(inputController != nullptr)
        {
            gamepads = inputController->getGamepads();
        }

        int player = 1;

        //Go through all gamepads and translate config to retroarch gamepad config
        for(auto & ent : gamepads)
        {
            retroarchCfg << "input_libretro_device_p" << player << " = 1" << endl;
            retroarchCfg << "input_player" << player << "_joypad_index = " << ent->getNumber() << endl;

            multimap<GamepadConfig, GamepadActions> gamepadConf = ent->getConfig();

            for(auto & en : gamepadConf)
            {
                string line = "";

                //First part of the config line for regular keys and axis
                if(en.second != GamepadActions::MAGIC)
                {
                    line += "input_player" + to_string(player) + "_";
                    line += Gamepad::GamepadActionsRetroarchResolver[en.second];
                }
                else
                {
                    continue;
                }

                //Second part of the config line for regular keys and axis
                if(en.first.control == GamepadControl::BUTTON)
                {
                    line += "_btn = \"";
                    line += to_string(en.first.number);
                    line += "\"";
                }
                else if(en.first.control == GamepadControl::AXIS_POS || en.first.control == GamepadControl::AXIS_NEG)
                {
                    line += "_axis = \"";

                    if(en.first.control == GamepadControl::AXIS_POS)
                        line += "+";

                    else
                        line +="-";

                    line += to_string(en.first.number);
                    line += "\"";
                }
                else if(en.first.control == GamepadControl::HAT_UP || en.first.control == GamepadControl::HAT_DOWN || en.first.control == GamepadControl::HAT_LEFT || en.first.control == GamepadControl::HAT_RIGHT)
                {
                    line += "_btn = \"h";
                    line += to_string(en.first.number);

                    if (en.first.control == GamepadControl::HAT_UP)
                    {
                        line += "up";
                    }
                    else if (en.first.control == GamepadControl::HAT_DOWN)
                    {
                        line += "down";
                    }
                    else if (en.first.control == GamepadControl::HAT_LEFT)
                    {
                        line += "left";
                    }
                    else if (en.first.control == GamepadControl::HAT_RIGHT)
                    {
                        line += "right";
                    }

                    line += "\"";
                }

                //Write line to file
                retroarchCfg << line << endl;
            }

            if(player == 1)
            {
                //For player 1, go through gamepad config to set special keys
                for(auto & en : gamepadConf)
                {
                    string line = "";

                    //Identify if current key is special, set first part if
                    switch (en.second)
                    {
                        case GamepadActions::MAGIC :
                            line += "input_enable_hotkey";
                            break;
                        case GamepadActions::START :
                            line += "input_exit_emulator";
                            break;
                        case GamepadActions::L1 :
                            line += "input_save_state";
                            break;
                        case GamepadActions::R1 :
                            line += "input_load_state";
                            break;
                    }

                    //If current key or axis is special, set second part
                    if(line != "")
                    {
                        if(en.first.control == GamepadControl::BUTTON)
                        {
                            line += "_btn = \"";
                            line += to_string(en.first.number);
                            line += "\"";
                        }
                        else
                        {
                            line += "_axis = \"";

                            if(en.first.control == GamepadControl::AXIS_POS)
                                line += "+";

                            else
                                line +="-";

                            line += to_string(en.first.number);
                            line += "\"";
                        }
                        //Write to config file
                        retroarchCfg << line << endl;
                    }
                }
            }

            player ++;
        }

        //TODO Add system/emulator specific config

        retroarchCfg.close();
    }

    //If file cannot be oppened
    else
    {
        Log::addEntry(1, "/!\\ Could not load retroarch.cfg");
    }

    return this;
}
