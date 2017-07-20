#ifndef DEF_GAMESELECTCONTROLLER
#define DEF_GAMESELECTCONTROLLER

#include "abstractController.hpp"

#include <map>

class Controller;

class GameSelectController : public AbstractController
{
public:
    GameSelectController(Controller* controller);
    ~GameSelectController();

    virtual GameSelectController* control();
    virtual GameSelectController* inputControl();

    GameSelectController* nextGame();
    GameSelectController* previousGame();

    GameSelectController* launchSelectedGame();

    int getSelectedGame() const;
    int getSelectedGame(const int& systemID) const;
    GameSelectController* setSelectedGame(const int& gameID);
    GameSelectController* setSelectedGame(const int& systemID, const int& gameID);

    int getNumberGames() const;


protected:
};

#endif
