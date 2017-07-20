#ifndef DEF_GAMESELECTVIEW
#define DEF_GAMESELECTVIEW

#include "abstractView.hpp"

#include <map>

class GameSelectView : public AbstractView
{
public:
    GameSelectView(View* view);
    ~GameSelectView();

    virtual GameSelectView* render(const ViewData& viewData);

    virtual GameSelectView* loadImages();

    virtual GameSelectView* destroyImages();

    virtual GameSelectView* calculateMeasures();

    Image* getBackground() const;
    GameSelectView* setBackground(Image* background);

    RoundedRectangle* getContentBox() const;
    GameSelectView* setContentBox(RoundedRectangle* contentBox);

    Text* getTitle() const;
    GameSelectView* setTitle(Text* title);

    Image* getUpArrow() const;
    GameSelectView* setUpArrow(Image* upArrow);

    Image* getDownArrow() const;
    GameSelectView* setDownArrow(Image* downArrow);

    Text* getGameText(const int& systemID, const int& gameID) const;
    std::map <int, Text*> getGamesTexts(const int& systemID) const;
    std::map <int, std::map <int, Text*>> getGamesTexts() const;
    GameSelectView* addGameText(const int& systemID, const int& gameID, Text* gameText);

    int getGameRobotoSize() const;
    GameSelectView* setGameRobotoSize(const int& size);

    int getGameAquinoSize() const;
    GameSelectView* setGameAquinoSize(const int& size);


protected:
    Image* m_pBackground = nullptr;
    RoundedRectangle* m_pContentBox = nullptr;
    Text* m_pTitle = nullptr;

    Image* m_pUpArrow = nullptr;
    Image* m_pDownArrow = nullptr;

    std::map <int, std::map <int, Text*>> m_pGamesTexts;

    int m_gameRobotoSize;
    int m_gameAquinoSize;
};
#endif
