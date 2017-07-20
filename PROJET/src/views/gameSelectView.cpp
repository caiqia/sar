
#include "gameSelectView.hpp"

#include "../entities/game.hpp"

#include <iostream>

using namespace std;

const string BACKGROUND43 = "/nostal/IMG/WIZARD_BG_4-3.png";
const string BACKGROUND169 = "/nostal/IMG/WIZARD_BG_16-9.png";
const string UP_ARROW = "/nostal/IMG/RESSOURCES/GAME_SELECT/UP_ARROW.png";
const string DOWN_ARROW = "/nostal/IMG/RESSOURCES/GAME_SELECT/DOWN_ARROW.png";

const int CONTENT_BOX_SLOPE = 5;
const int GAMES_PER_PAGE = 10;

const string TITLE = "Choisissez votre jeu";

const NostalColors CONTENT_BOX_COLOR = WHITE_THIRD_TRANSPARENT;
const NostalColors TITLE_COLOR = WHITE;
const NostalColors TEXT_COLOR = WHITE;

const FontFamily GAME_FONT = ROBOTO;
const FontFamily GAME_HILIGHTED_FONT = AQUINO;

GameSelectView::GameSelectView(View* view)
: AbstractView(view, ViewsID::GAME_SELECT_VIEW)
{

}

GameSelectView::~GameSelectView()
{

}

GameSelectView* GameSelectView::render(const ViewData& viewData)
{
    AbstractView::render(viewData);

    if(this->getBackground() != nullptr)
    {
        this->getView()->addToRender(this->getBackground());
    }

    if(this->getContentBox() != nullptr)
    {
        this->getView()->addToRender(this->getContentBox());
    }

    if(this->getTitle() != nullptr)
    {
        this->getView()->addToRender(this->getTitle());
    }

    if(this->getView()->getLogo(viewData.selectedSystem) != nullptr)
    {
        Image* logo = this->getView()->getLogo(viewData.selectedSystem);
        logo->setSize(this->getMeasure("contentColumnWidth"), this->getMeasure("logoHeight"), ResizeMode::FIT);
        logo->center(this->getMeasure("contentColumnX"), this->getMeasure("contentColumnXMax"), this->getMeasure("logoY"), this->getMeasure("logoYMax"));
        this->getView()->addToRender(logo);
    }

    if(viewData.systems.count(viewData.selectedSystem) > 0)
    {
        int selectedGame = 0;

        if(viewData.selectedGames.count(viewData.selectedSystem) > 0)
        {
            selectedGame = viewData.selectedGames.at(viewData.selectedSystem);
        }

        int page = selectedGame / GAMES_PER_PAGE;

        if(this->getUpArrow() != nullptr && page != 0)
        {
            this->getView()->addToRender(this->getUpArrow());
        }

        if(this->getDownArrow() != nullptr && page != ((viewData.systems.at(viewData.selectedSystem)->getNumberGamesAvailable() - 1) / GAMES_PER_PAGE))
        {
            this->getView()->addToRender(this->getDownArrow());
        }


        if(viewData.systems.at(viewData.selectedSystem) != nullptr)
        {
            int i= 0;
            for(const auto& ent : viewData.systems.at(viewData.selectedSystem)->getGames())
            {
                if(ent.first >= (page * GAMES_PER_PAGE) && ent.first < ((page + 1) * GAMES_PER_PAGE))
                {
                    if(ent.second != nullptr)
                    {
                        Text* gameText = this->getGameText(viewData.selectedSystem, ent.first);
                        if(gameText != nullptr)
                        {

                            if(viewData.selectedGames.count(viewData.selectedSystem) > 0)
                            {
                                if(ent.first == viewData.selectedGames.at(viewData.selectedSystem) && gameText->getFont() != Font::getFontFromMap(GAME_HILIGHTED_FONT, this->getGameAquinoSize()))
                                {
                                    if(gameText->getText() != ent.second->getName())
                                    {
                                        gameText->setText(ent.second->getName());
                                    }
                                    gameText->setFont(Font::getFontFromMap(GAME_HILIGHTED_FONT, this->getGameAquinoSize()));
                                    gameText->cropToFit(this->getMeasure("gameAreaWidth"));
                                    gameText->reload();
                                }
                                else if(ent.first != viewData.selectedGames.at(viewData.selectedSystem) && gameText->getFont() != Font::getFontFromMap(GAME_FONT, this->getGameRobotoSize()))
                                {
                                    if(gameText->getText() != ent.second->getName())
                                    {
                                        gameText->setText(ent.second->getName());
                                    }
                                    gameText->setFont(Font::getFontFromMap(GAME_FONT, this->getGameRobotoSize()));
                                    gameText->cropToFit(this->getMeasure("gameAreaWidth"));
                                    gameText->reload();
                                }
                            }

                            int posY, posYMax;

                            posY = this->getMeasure("gameAreaY") + (i * (this->getMeasure("gameTextHeight") + this->getMeasure("gameTextVerticalMargin")));
                            posYMax = posY + this->getMeasure("gameTextHeight");

                            gameText->centerVert(posY, posYMax);
                            this->getView()->addToRender(gameText);
                        }
                    }
                    ++i;
                }
            }
        }
    }

    return this;
}

GameSelectView* GameSelectView::loadImages()
{
    AbstractView::loadImages();

    string backgroundToLoad;
    if(this->getView()->getWindow()->getAspectRatio() == "4/3")
    {
        backgroundToLoad = BACKGROUND43;
    }
    else
    {
        backgroundToLoad = BACKGROUND169;
    }

    this->setBackground(new Image(this->getView()->getRenderer(), backgroundToLoad, this->getMeasure("displayableWidth"), this->getMeasure("displayableHeight"), this->getMeasure("displayableX"), this->getMeasure("displayableY")));

    this->setContentBox(new RoundedRectangle(this->getView()->getRenderer(), Color::getColorFromMap(CONTENT_BOX_COLOR), this->getMeasure("contentBoxWidth"), this->getMeasure("contentBoxHeight"), this->getMeasure("contentBoxX"), this->getMeasure("contentBoxY"), RoundedAngles::ALL_ANGLES, CONTENT_BOX_SLOPE));

    int fontSize = Font::getBiggestFontToFit(FontFamily::ROBOTO, TITLE, this->getMeasure("contentColumnWidth"), this->getMeasure("titleHeight"));
    this->setTitle(new Text(this->getView()->getRenderer(), TITLE, Font::getFontFromMap(FontFamily::ROBOTO, fontSize), Color::getColorFromMap(TITLE_COLOR), 0, 0));
    this->getTitle()->center(this->getMeasure("contentColumnX"), this->getMeasure("contentColumnXMax"), this->getMeasure("titleY"), this->getMeasure("titleYMax"));

    this->setUpArrow(new Image(this->getView()->getRenderer(), UP_ARROW));

    if(this->getUpArrow() != nullptr)
    {
        this->getUpArrow()->setSize(this->getMeasure("arrowWidth"), this->getMeasure("arrowHeight"), FIT);
        this->getUpArrow()->center(this->getMeasure("arrowAreaX"), this->getMeasure("arrowAreaXMax"), this->getMeasure("upperArrowY"), this->getMeasure("upperArrowYMax"));
    }

    this->setDownArrow(new Image(this->getView()->getRenderer(), DOWN_ARROW));
    if(this->getDownArrow() != nullptr)
    {
        this->getDownArrow()->setSize(this->getMeasure("arrowWidth"), this->getMeasure("arrowHeight"), FIT);
        this->getDownArrow()->center(this->getMeasure("arrowAreaX"), this->getMeasure("arrowAreaXMax"), this->getMeasure("bottomArrowY"), this->getMeasure("bottomArrowYMax"));
    }

    int robotoFontSize = Fonts::ROBOTOLAST;
    int aquinoFontSize = Fonts::AQUINOLAST;

    for(const auto& ent1 : this->getView()->getController()->viewData.systems)
    {
        for(const auto& ent2 : ent1.second->getGames())
        {
            int currentRobotoFontSize = Font::getBiggestFontForHeight(GAME_FONT, ent2.second->getName(), this->getMeasure("gameTextHeight"));
            int currentAquinoFontSize = Font::getBiggestFontForHeight(GAME_HILIGHTED_FONT, ent2.second->getName(), this->getMeasure("gameTextHeight"));

            if(currentRobotoFontSize < robotoFontSize)
            {
                robotoFontSize = currentRobotoFontSize;
            }

            if(currentAquinoFontSize < aquinoFontSize)
            {
                aquinoFontSize = currentAquinoFontSize;
            }
        }
    }

    this->setGameRobotoSize(robotoFontSize);
    this->setGameAquinoSize(aquinoFontSize);

    for(const auto& ent1 : this->getView()->getController()->viewData.systems)
    {
        for(const auto& ent2 : ent1.second->getGames())
        {
            Text* gameText = new Text(this->getView()->getRenderer(), ent2.second->getName(), Font::getFontFromMap(GAME_FONT, robotoFontSize), Color::getColorFromMap(TEXT_COLOR), this->getMeasure("gameAreaX"), 0);

            if(gameText != nullptr)
            {
                gameText->cropToFit(this->getMeasure("gameAreaWidth"));
                this->addGameText(ent1.first, ent2.first, gameText);
            }
        }
    }

    return this;
}

GameSelectView* GameSelectView::destroyImages()
{
    AbstractView::destroyImages();

    this->setBackground(nullptr);

    this->setContentBox(nullptr);

    this->setTitle(nullptr);

    this->setUpArrow(nullptr);

    this->setDownArrow(nullptr);


    for(auto& ent1 : this->getGamesTexts())
    {
        for(auto& ent2 : ent1.second)
        {
            if(ent2.second != nullptr)
            {
                delete ent2.second;
            }
        }
    }
    this->m_pGamesTexts.clear();

    return this;
}

GameSelectView* GameSelectView::calculateMeasures()
{
    AbstractView::calculateMeasures();

    this->addMeasure("contentBoxWidth", this->getMeasure("compensatedWidth") - (this->getMeasure("compensatedWidth") / 4));
    this->addMeasure("contentBoxHeight", this->getMeasure("compensatedHeight") - (this->getMeasure("compensatedHeight") / 4));
    this->addMeasure("contentBoxX", this->getMeasure("compensatedX") + (this->getMeasure("compensatedWidth") / 8));
    this->addMeasure("contentBoxY", this->getMeasure("compensatedY") + (this->getMeasure("compensatedHeight") / 8));

    this->addMeasure("gameAreaHorizontalPadding", this->getMeasure("contentBoxWidth") / 4);

    this->addMeasure("contentColumnWidth", this->getMeasure("contentBoxWidth") / 3);
    this->addMeasure("contentColumnX", this->getMeasure("contentBoxX") + ((this->getMeasure("contentBoxWidth") - this->getMeasure("contentColumnWidth")) / 2));
    this->addMeasure("contentColumnXMax", this->getMeasure("contentColumnX") + this->getMeasure("contentColumnWidth"));

    this->addMeasure("titleHeight", this->getMeasure("contentBoxHeight") / 13);
    this->addMeasure("titleY", this->getMeasure("contentBoxY") + (this->getMeasure("contentBoxHeight") / 26));
    this->addMeasure("titleYMax", this->getMeasure("titleY") + this->getMeasure("titleHeight"));

    this->addMeasure("logoHeight", this->getMeasure("contentBoxHeight") * 3 / 26);
    this->addMeasure("logoY", this->getMeasure("titleYMax") + (this->getMeasure("contentBoxHeight") / 13));
    this->addMeasure("logoYMax", this->getMeasure("logoY") + this->getMeasure("logoHeight"));

    this->addMeasure("gameAreaHeight", this->getMeasure("contentBoxHeight") * 7 / 13);
    this->addMeasure("gameAreaWidth", this->getMeasure("contentBoxWidth") - (2 * this->getMeasure("gameAreaHorizontalPadding")));
    this->addMeasure("gameAreaY", this->getMeasure("logoYMax") + (this->getMeasure("contentBoxHeight") / 26));
    this->addMeasure("gameAreaYMax", this->getMeasure("gameAreaY") + this->getMeasure("gameAreaHeight"));
    this->addMeasure("gameAreaX", this->getMeasure("contentBoxX") + this->getMeasure("gameAreaHorizontalPadding"));
    this->addMeasure("gameAreaXMax", this->getMeasure("gameAreaX") + this->getMeasure("gameAreaWidth"));

    this->addMeasure("gameTextVerticalMargin", (this->getMeasure("gameAreaHeight") / GAMES_PER_PAGE) / 3);
    this->addMeasure("gameTextHeight", (this->getMeasure("gameAreaHeight") - ((GAMES_PER_PAGE - 1) * this->getMeasure("gameTextVerticalMargin"))) / GAMES_PER_PAGE);

    this->addMeasure("arrowHeight", this->getMeasure("contentBoxHeight") / 52);
    this->addMeasure("arrowVerticalPadding", this->getMeasure("arrowHeight") / 2);
    this->addMeasure("arrowWidth", this->getMeasure("contentBoxWidth") / 48);
    this->addMeasure("arrowHorizontalPadding", this->getMeasure("arrowWidth") / 2);
    this->addMeasure("arrowAreaX", this->getMeasure("contentColumnX") + (this->getMeasure("contentBoxWidth") * 3 / 24));
    this->addMeasure("arrowAreaXMax", this->getMeasure("arrowAreaX") + this->getMeasure("contentBoxWidth") / 12);
    this->addMeasure("upperArrowY", this->getMeasure("logoYMax"));
    this->addMeasure("upperArrowYMax", this->getMeasure("gameAreaY"));
    this->addMeasure("bottomArrowY", this->getMeasure("gameAreaYMax"));
    this->addMeasure("bottomArrowYMax", this->getMeasure("bottomArrowY") + this->getMeasure("arrowHeight") + (2 * this->getMeasure("arrowVerticalPadding")));
    return this;
}

Image* GameSelectView::getBackground() const
{
    return this->m_pBackground;
}

GameSelectView* GameSelectView::setBackground(Image* background)
{
    if(this->m_pBackground != nullptr)
    {
        delete this->m_pBackground;
    }
    this->m_pBackground = background;
    return this;
}

RoundedRectangle* GameSelectView::getContentBox() const
{
    return this->m_pContentBox;
}

GameSelectView* GameSelectView::setContentBox(RoundedRectangle* contentBox)
{
    if(this->m_pContentBox != nullptr)
    {
        delete this->m_pContentBox;
    }
    this->m_pContentBox = contentBox;
    return this;
}

Text* GameSelectView::getTitle() const
{
    return this->m_pTitle;
}

GameSelectView* GameSelectView::setTitle(Text* title)
{
    if(this->m_pTitle != nullptr)
    {
        delete this->m_pTitle;
    }
    this->m_pTitle = title;
    return this;
}

Image* GameSelectView::getUpArrow() const
{
    return this->m_pUpArrow;
}

GameSelectView* GameSelectView::setUpArrow(Image* upArrow)
{
    if(this->m_pUpArrow != nullptr)
    {
        delete this->m_pUpArrow;
    }
    this->m_pUpArrow = upArrow;

    return this;
}

Image* GameSelectView::getDownArrow() const
{
    return this->m_pDownArrow;
}

GameSelectView* GameSelectView::setDownArrow(Image* downArrow)
{
    if(this->m_pDownArrow != nullptr)
    {
        delete this->m_pDownArrow;
    }
    this->m_pDownArrow = downArrow;

    return this;
}

Text* GameSelectView::getGameText(const int& systemID, const int& gameID) const
{
    if(this->m_pGamesTexts.count(systemID) > 0)
    {
        if(this->m_pGamesTexts.at(systemID).count(gameID) > 0)
        {
            return this->m_pGamesTexts.at(systemID).at(gameID);
        }
    }
    return nullptr;
}

map <int, Text*> GameSelectView::getGamesTexts(const int& systemID) const
{
    if(this->m_pGamesTexts.count(systemID) > 0)
    {
        return this->m_pGamesTexts.at(systemID);
    }
    map<int, Text*> nullRtrn;
    return nullRtrn;
}

map <int, map <int, Text*>> GameSelectView::getGamesTexts() const
{
    return this->m_pGamesTexts;
}

GameSelectView* GameSelectView::addGameText(const int& systemID, const int& gameID, Text* gameText)
{
    if(this->m_pGamesTexts.count(systemID) > 0)
    {
        if(this->m_pGamesTexts.at(systemID).count(gameID) > 0)
        {
            delete this->m_pGamesTexts.at(systemID).at(gameID);
        }
    }
    this->m_pGamesTexts[systemID][gameID] = gameText;
    return this;
}

int GameSelectView::getGameRobotoSize() const
{
    return this->m_gameRobotoSize;
}

GameSelectView* GameSelectView::setGameRobotoSize(const int& size)
{
    this->m_gameRobotoSize = size;
    return this;
}

int GameSelectView::getGameAquinoSize() const
{
    return this->m_gameAquinoSize;
}

GameSelectView* GameSelectView::setGameAquinoSize(const int& size)
{
    this->m_gameAquinoSize = size;
    return this;
}
