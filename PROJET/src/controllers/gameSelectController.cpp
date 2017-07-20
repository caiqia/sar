#include "gameSelectController.hpp"
#include "inputController.hpp"
#include "gameLaunchController.hpp"
#include "menuController.hpp"
#include "../entities/log.hpp"
#include "../entities/system.hpp"
#include "../entities/game.hpp"
#include "../entities/error.hpp"

using namespace std;

GameSelectController::GameSelectController(Controller* controller)
: AbstractController(controller, ControllersID::GAME_SELECT_CONTROLLER)
{

}

GameSelectController::~GameSelectController()
{

}

GameSelectController* GameSelectController::control()
{
    AbstractController::control();
    return this;
}

GameSelectController* GameSelectController::inputControl()
{
    AbstractController::inputControl();

    for(auto& ent : this->getController()->getAdvancedControls())
    {
        if(this->getController()->getGamepadControl(ent.first, GamepadActions::DOWN))
        {
            this->nextGame();
        }
        else if(this->getController()->getGamepadControl(ent.first, GamepadActions::UP))
        {
            this->previousGame();
        }
        else if(this->getController()->getGamepadControl(ent.first, GamepadActions::START))
        {
            MenuController* menuController = static_cast<MenuController*>(this->getController()->getController(MENU_CONTROLLER));
            if(menuController != nullptr)
            {
                menuController->isActive(true);
            }
        }
        else if(this->getController()->getGamepadControl(ent.first, GamepadActions::A))
        {
            this->getController()->setCurrentView(CurrentViews::HOME);
        }
        else if(this->getController()->getGamepadControl(ent.first, GamepadActions::B))
        {
            this->launchSelectedGame();
        }
    }

    return this;
}

GameSelectController* GameSelectController::nextGame()
{
    if((this->getSelectedGame() + 1) < this->getNumberGames())
    {
        this->setSelectedGame(this->getSelectedGame() + 1);
    }
    else
    {
        this->setSelectedGame(0);
    }
    return this;
}

GameSelectController* GameSelectController::previousGame()
{
    if(this->getSelectedGame() != 0)
    {
        this->setSelectedGame(this->getSelectedGame() - 1);
    }
    else
    {
        this->setSelectedGame(this->getNumberGames() - 1);
    }
    return this;
}

GameSelectController* GameSelectController::launchSelectedGame()
{
    if(this->getController()->viewData.systems.count(this->getController()->viewData.selectedSystem) > 0)
    {
        if(this->getController()->viewData.systems.at(this->getController()->viewData.selectedSystem) != nullptr)
        {
            if(this->getController()->viewData.systems.at(this->getController()->viewData.selectedSystem)->getGame(this->getSelectedGame()) != nullptr)
            {
                GameLaunchController* gameLaunchController = static_cast<GameLaunchController*>(this->getController()->getController(GAME_LAUNCH_CONTROLLER));

                if(gameLaunchController != nullptr)
                {
                    gameLaunchController->launch(this->getController()->viewData.systems.at(this->getController()->viewData.selectedSystem)->getGame(this->getSelectedGame()));
                }
            }
        }
    }
    return this;
}

int GameSelectController::getSelectedGame() const
{
    if(this->getController()->viewData.selectedGames.count(this->getController()->viewData.selectedSystem) > 0)
    {
        return this->getController()->viewData.selectedGames.at(this->getController()->viewData.selectedSystem);
    }
    return 0;
}

int GameSelectController::getSelectedGame(const int& systemID) const
{
    if(this->getController()->viewData.selectedGames.count(systemID) > 0)
    {
        return this->getController()->viewData.selectedGames.at(systemID);
    }
    return 0;
}

GameSelectController* GameSelectController::setSelectedGame(const int& gameID)
{
    this->getController()->viewData.selectedGames[this->getController()->viewData.selectedSystem] = gameID;
    return this;
}

GameSelectController* GameSelectController::setSelectedGame(const int& systemID, const int& gameID)
{
    this->getController()->viewData.selectedGames[systemID] = gameID;
    return this;
}

int GameSelectController::getNumberGames() const
{
    if(this->getController()->viewData.systems.count(this->getController()->viewData.selectedSystem) > 0)
    {
        if(this->getController()->viewData.systems.at(this->getController()->viewData.selectedSystem) != nullptr)
        {
            return this->getController()->viewData.systems.at(this->getController()->viewData.selectedSystem)->getNumberGamesAvailable();
        }
    }
    return 0;
}
/*
//Controller for the Game Select View

//Basic Init
GameSelectController::GameSelectController(Controller* controller)
{
    this->setController(controller);
    this->getController()->setGameSelectController(this);
    Log::addEntry(1, "GameSelectController initialized successfully");
}

GameSelectController::~GameSelectController()
{
    Log::addEntry(1, "GameSelectController deleted");
}

//Control loop for this view
GameSelectController* GameSelectController::control()
{

    this->getController()->setPreviouslySelectedGame(-1);

    for(auto& ent : this->getController()->getAdvancedControls())
    {
        if(this->getController()->getGamepadControl(ent.first, GamepadActions::A))
        {
            this->getController()->setCurrentView(CurrentViews::HOME);
        }
        else if(this->getController()->getGamepadControl(ent.first, GamepadActions::DOWN))
        {
            this->nextGame();
        }
        else if(this->getController()->getGamepadControl(ent.first, GamepadActions::UP))
        {
            this->previousGame();
        }
        else if(this->getController()->getGamepadControl(ent.first, GamepadActions::B))
        {
            this->launchSelectedGame();
        }
    }

    return this;
}

Controller* GameSelectController::getController()
{
    return this->m_pController;
}

GameSelectController* GameSelectController::setController(Controller* controller)
{
    this->m_pController = controller;
    return this;
}

//Function to select the next game in the list
GameSelectController* GameSelectController::nextGame()
{
    int gameSelect;

    int maxGames = this->getController()->viewData.systems.at(this->getController()->viewData.selectedSystem)->getNumberGamesAvailable() - 1;

    //If it is not the last game in the list
    if(this->getSelectedGame() < maxGames)
        gameSelect = this->getSelectedGame() + 1;

    else
        gameSelect = 0;

    this->getController()->setPreviouslySelectedGame(this->getSelectedGame());

    this->getController()->setSelectedGame(gameSelect);

    return this;
}

GameSelectController* GameSelectController::previousGame()
{
    int gameSelect;

    int maxGames = this->getController()->viewData.systems.at(this->getController()->viewData.selectedSystem)->getNumberGamesAvailable() - 1;

    //If it is the first game in the list
    if(this->getSelectedGame() == 0)
        gameSelect= maxGames;

    else
        gameSelect = this->getSelectedGame() - 1;

    this->getController()->setPreviouslySelectedGame(this->getSelectedGame());
    this->getController()->setSelectedGame(gameSelect);

    return this;
}

//Function to call launch controller on the selected game
GameSelectController* GameSelectController::launchSelectedGame()
{


    //Get pointer on selected game
    Game* selectedGame = this->getController()->viewData.systems.at(this->getController()->viewData.selectedSystem)->getGame(this->getSelectedGame());

    //Call launch controller

  //if(SDL_NumJoysticks()<1)
  //{

//  }

  this->getController()->getGameLaunchController()->launch(selectedGame);

    return this;
}

int GameSelectController::getSelectedGame()
{
    int selectedGame = this->getController()->viewData.systems.at(this->getController()->viewData.selectedSystem)->getSelectedGame();
    return selectedGame;
}*/
