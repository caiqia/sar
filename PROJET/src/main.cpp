#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <string.h>
#include <vector>
#include <exception>

#include "entities/error.hpp"
#include "entities/errorexception.hpp"
#include "entities/game.hpp"
#include "entities/log.hpp"
#include "entities/window.hpp"
#include "entities/system.hpp"
#include "entities/gamepad.hpp"
#include "entities/menuElements.hpp"

#include "controllers/controller.hpp"
#include "controllers/osController.hpp"


#include "views/view.hpp"
#include "views/homeView.hpp"
#include "views/gameSelectView.hpp"
#include "views/gamepadProfileSelectView.hpp"
#include "views/firstLaunchView.hpp"
#include "views/errorview.hpp"
#include "views/image.hpp"
#include "views/visualKeyboardView.hpp"
#include "views/menuView.hpp"

using namespace std;

/***************************************************
*                                                  *
* Nostalfront, emulator front-end for the Retropac *
* Written by Noé CHARMET                           *
*                                                  *
****************************************************/

int main(int argc, char *argv[])
{
    bool devMode = false;

    for (int i = 1; i < argc; ++i) {
        if (string(argv[i]) == "--dev" || string(argv[i]) == "-d") {
             cout << "dev mode" << endl;
             devMode = true;
        }
        else
        {
            cout << "unknown parameter : " <<  string(argv[i]) << endl;
        }
    }

    //Init of the SDL components

    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER | SDL_INIT_JOYSTICK) == -1)
    {
        SDL_JoystickEventState(SDL_ENABLE);

        SDL_SetHint(SDL_HINT_JOYSTICK_ALLOW_BACKGROUND_EVENTS, "1");
        cerr << "Erreur d'initialisation de la SDL" << endl;
        exit(EXIT_FAILURE);
    }

    //Create Log
    Log log(4);

    //disable the cursor
    SDL_ShowCursor(0);

    //Init Window and renderer
    Window window;
    SDL_SetRenderDrawBlendMode(window.getRenderer(), SDL_BLENDMODE_BLEND);
    window.initWindow();

    //If Window loaded correctly
    if (window.getInitStatus())
    {
        Log::addEntry(1, "Window and Renderer succesfully initialized");

    }
    else
    {
        cerr << "Erreur d'initialisation de la fenêtre" << endl;
        exit(EXIT_FAILURE);
    }

    if(TTF_Init() == -1)
    {
        cerr << "Erreur d'initialisation de TTF_init : " << TTF_GetError() << endl;
        exit(EXIT_FAILURE);
    }

    System::populate();

    SDL_Event eventListener;

    //Controller Inits
    Controller controller(&window, &eventListener, devMode);


    bool continu = 1;

    Log::addEntry(1, "All entities should be initialized, starting program loop !");

    //Var used to control the execution duration of a program loop
    Uint32 loopBeginTime = 0;


    while(continu)
    {

        loopBeginTime = SDL_GetTicks();

        //Call to main contoller
        continu = controller.control();

        //Call to main view

        controller.getView()->render();

        //Wait if it's been less then 40ms (1/25s)
        if(SDL_GetTicks() - loopBeginTime < 40)
        {
            SDL_Delay(40 - (SDL_GetTicks() - loopBeginTime));
        }

    }

    //destroy all dynamically allocated SDL objects
    Text::flushFonts();
    TTF_Quit();
    SDL_DestroyRenderer(window.getRenderer());
    SDL_DestroyWindow(window.getWindow());
    SDL_Quit();

    Log::addEntry(1, "Quitting program, bye bye !");

    OSController* osController = static_cast<OSController*>(controller.getController(OS_CONTROLLER));
    osController->powerOff();

    return 0;
}
