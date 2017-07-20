#include <iostream>
#include <regex>

#include "homeView.hpp"
#include "view.hpp"
#include "surface.hpp"
#include "text.hpp"
#include "../controllers/controller.hpp"
#include "../controllers/homeController.hpp"
#include "../entities/log.hpp"

using namespace std;

const string BACKGROUND43 = "/nostal/IMG/WIZARD_BG_4-3.png";
const string BACKGROUND169 = "/nostal/IMG/WIZARD_BG_16-9.png";
const string DOWN_ARROW = "/nostal/IMG/RESSOURCES/HOME/DOWN_ARROW.png";
const string DOWN_ARROW_GREYED = "/nostal/IMG/RESSOURCES/HOME/DOWN_ARROW_G.png";
const string UP_ARROW = "/nostal/IMG/RESSOURCES/HOME/UP_ARROW.png";
const string UP_ARROW_GREYED = "/nostal/IMG/RESSOURCES/HOME/UP_ARROW_G.png";
const string TITLE_TEXT_CONTENT = "Choisissez votre console";

const int SYSTEMS_PER_PAGE = 12;
//View for the home screen where systems are selected

//Init
HomeView::HomeView(View* view)
: AbstractView(view, ViewsID::HOME_VIEW)
{
    Log::addEntry(1, "HomeView initialized successfully");
    return;
}

HomeView::~HomeView()
{
    Log::addEntry(1, "Deleted HomeView");
    return;
}

//Sets and adds the needed elements to the render queue
HomeView* HomeView::render(const ViewData& viewData)
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

    if(this->getTitleText() != nullptr)
    {
        this->getView()->addToRender(this->getTitleText());
    }

    Image* downArrow = this->getDownArrow();
    Image* upArrow = this->getUpArrow();

    if(viewData.selectedSystem / SYSTEMS_PER_PAGE == 0)
    {
        upArrow = this->getUpArrowGreyed();
    }

    if(viewData.selectedSystem / SYSTEMS_PER_PAGE == viewData.numberSystems / SYSTEMS_PER_PAGE)
    {
        downArrow = this->getDownArrowGreyed();
    }
    if(upArrow != nullptr)
    {
        this->getView()->addToRender(upArrow);
    }
    if(downArrow != nullptr)
    {
        this->getView()->addToRender(downArrow);
    }

    this->setGrid(viewData);

    return this;
}

//Loads the images, texts and surfaces needed
HomeView* HomeView::loadImages()
{
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

    this->setContentBox(new RoundedRectangle(this->getView()->getRenderer(), Color::getColorFromMap(NostalColors::WHITE_THIRD_TRANSPARENT), this->getMeasure("contentBoxWidth"), this->getMeasure("contentBoxHeight"), this->getMeasure("contentBoxX"), this->getMeasure("contentBoxY"), RoundedAngles::ALL_ANGLES, 5));

    int titleTextFontSize = Font::getBiggestFontToFit(FontFamily::ROBOTO, TITLE_TEXT_CONTENT, this->getMeasure("titleTextWidth"), this->getMeasure("titleTextHeight"));
    this->setTitleText(new Text(this->getView()->getRenderer(), TITLE_TEXT_CONTENT, Font::getFontFromMap(FontFamily::ROBOTO, titleTextFontSize), Color::getColorFromMap(NostalColors::WHITE), this->getMeasure("titleTextX"), this->getMeasure("titleTextY")));
    this->getTitleText()->center(this->getMeasure("titleTextX"), this->getMeasure("titleTextMaxX"), this->getMeasure("titleTextY"), this->getMeasure("titleTextMaxY"));

    this->setSelector(new RoundedRectangle(this->getView()->getRenderer(), Color::getColorFromMap(NostalColors::DARK_BLUE_THIRD_TRANSPARENT), this->getMeasure("systemSelectorWidth"), this->getMeasure("systemSelectorHeight"), 0, 0, RoundedAngles::ALL_ANGLES, 10));

    for(const auto& ent : this->getView()->getController()->viewData.systems)
    {
        string text = "";
        if(ent.second->isAvailable())
        {
            text += to_string(ent.second->getNumberGamesAvailable());
            text += " jeu";
            if(ent.second->getNumberGamesAvailable() > 1)
            {
                text += "x";
            }
        }
        else
        {
            text += "Non disponible";
        }
        int subTextFontSize = Font::getBiggestFontToFit(FontFamily::ROBOTO, text, this->getMeasure("systemTextAreaWidth"), this->getMeasure("systemTextAreaHeight"));
        Text* subText = new Text(this->getView()->getRenderer(), text, Font::getFontFromMap(FontFamily::ROBOTO, subTextFontSize), Color::getColorFromMap(NostalColors::WHITE), 0, 0);
        this->addSubText(ent.first, subText);
    }

    this->setUpArrow(new Image(this->getView()->getRenderer(), UP_ARROW));
    this->getUpArrow()->setSize(this->getMeasure("arrowWidth"), this->getMeasure("arrowHeight"), ResizeMode::FIT);
    this->getUpArrow()->center(this->getMeasure("upArrowX"), this->getMeasure("upArrowXMax"), this->getMeasure("arrowY"), this->getMeasure("arrowYMax"));

    this->setUpArrowGreyed(new Image(this->getView()->getRenderer(), UP_ARROW_GREYED));
    this->getUpArrowGreyed()->setSize(this->getMeasure("arrowWidth"), this->getMeasure("arrowHeight"), ResizeMode::FIT);
    this->getUpArrowGreyed()->center(this->getMeasure("upArrowX"), this->getMeasure("upArrowXMax"), this->getMeasure("arrowY"), this->getMeasure("arrowYMax"));

    this->setDownArrow(new Image(this->getView()->getRenderer(), DOWN_ARROW));
    this->getDownArrow()->setSize(this->getMeasure("arrowWidth"), this->getMeasure("arrowHeight"), ResizeMode::FIT);
    this->getDownArrow()->center(this->getMeasure("downArrowX"), this->getMeasure("downArrowXMax"), this->getMeasure("arrowY"), this->getMeasure("arrowYMax"));

    this->setDownArrowGreyed(new Image(this->getView()->getRenderer(), DOWN_ARROW_GREYED));
    this->getDownArrowGreyed()->setSize(this->getMeasure("arrowWidth"), this->getMeasure("arrowHeight"), ResizeMode::FIT);
    this->getDownArrowGreyed()->center(this->getMeasure("downArrowX"), this->getMeasure("downArrowXMax"), this->getMeasure("arrowY"), this->getMeasure("arrowYMax"));

    return this;
}

HomeView* HomeView::destroyImages()
{
    if(this->getBackground() != nullptr)
    {
        this->setBackground(nullptr);
    }

    if(this->getContentBox() != nullptr)
    {
        this->setContentBox(nullptr);
    }

    if(this->getTitleText() != nullptr)
    {
        this->setTitleText(nullptr);
    }

    if(this->getSelector() != nullptr)
    {
        this->setSelector(nullptr);
    }

    for(auto& ent : this->getSubTexts())
    {
        if(ent.second != nullptr)
        {
            delete ent.second;
        }
    }
    this->m_pSubTexts.clear();

    if(this->getUpArrow() != nullptr)
    {
        this->setUpArrow(nullptr);
    }

    if(this->getDownArrow() != nullptr)
    {
        this->setDownArrow(nullptr);
    }

    if(this->getUpArrowGreyed() != nullptr)
    {
        this->setUpArrowGreyed(nullptr);
    }

    if(this->getDownArrowGreyed() != nullptr)
    {
        this->setDownArrowGreyed(nullptr);
    }

    return this;
}

//Creates the grid of 9 consoles for the current page
HomeView* HomeView::setGrid(const ViewData& viewData)
{
    int page = viewData.selectedSystem / SYSTEMS_PER_PAGE;

    int row = 0;
    int col = 0;

    for(int i = page * SYSTEMS_PER_PAGE; (i != (page + 1) * SYSTEMS_PER_PAGE) && (i != viewData.numberSystems); ++i)
    {
        int posX, posY, posXMax, posYMax, width, height;

        if(i == viewData.selectedSystem)
        {
            int selectorPosX, selectorPosY;
            selectorPosX = this->getMeasure("systemGridAreaX") + col * this->getMeasure("systemAreaWidth");
            selectorPosY = this->getMeasure("systemGridAreaY") + row * this->getMeasure("systemAreaHeight");
            this->getSelector()->setPosition(selectorPosX, selectorPosY);
            this->getView()->addToRender(this->getSelector());


            int subTextPosX, subTextPosY, subTextPosXMax, subTextPosYMax;
            subTextPosX = this->getMeasure("systemGridAreaX") + col * this->getMeasure("systemAreaWidth") + this->getMeasure("systemTextHorizontalMargin");
            subTextPosY = this->getMeasure("systemGridAreaY") + row * this->getMeasure("systemAreaHeight") + this->getMeasure("systemTextVerticalMargin");
            subTextPosXMax = subTextPosX + this->getMeasure("systemTextAreaWidth");
            subTextPosYMax = subTextPosY + this->getMeasure("systemTextAreaHeight");
            if(this->getSubText(i) != nullptr)
            {
                this->getSubText(i)->center(subTextPosX, subTextPosXMax, subTextPosY, subTextPosYMax);
                this->getView()->addToRender(this->getSubText(i));
            }
        }


        posX = this->getMeasure("systemGridAreaX") + col * this->getMeasure("systemAreaWidth") + this->getMeasure("systemLogoHorizontalMargin");
        posY = this->getMeasure("systemGridAreaY") + row * this->getMeasure("systemAreaHeight") + this->getMeasure("systemLogoVerticalMargin");
        posXMax = posX + this->getMeasure("systemLogoWidth");
        posYMax = posY + this->getMeasure("systemLogoHeight");

        if(viewData.systems.count(i) > 0)
        {
            Image* logo;
            if(viewData.systems.at(i)->isAvailable())
            {
                logo = this->getView()->getLogo(i);
            }
            else
            {
                logo = this->getView()->getGreyedLogo(i);
            }

            if(logo != nullptr)
            {
                logo->setSize(this->getMeasure("systemLogoWidth"), this->getMeasure("systemLogoHeight"), ResizeMode::FIT);
                logo->center(posX, posXMax, posY, posYMax);

                this->getView()->addToRender(logo);
            }
        }

        ++col;
        if(col == 3)
        {
            ++row;
            col = 0;
        }
    }

    return this;
}

HomeView* HomeView::calculateMeasures()
{
    AbstractView::calculateMeasures();
    this->addMeasure("contentBoxWidth", this->getMeasure("compensatedWidth") - (this->getMeasure("compensatedWidth") / 4));
    this->addMeasure("contentBoxHeight", this->getMeasure("compensatedHeight") - (this->getMeasure("compensatedHeight") / 4));
    this->addMeasure("contentBoxX", this->getMeasure("compensatedX") + (this->getMeasure("compensatedWidth") / 8));
    this->addMeasure("contentBoxY", this->getMeasure("compensatedY") + (this->getMeasure("compensatedHeight") / 8));

    this->addMeasure("titleTextWidth", this->getMeasure("contentBoxWidth") - (this->getMeasure("contentBoxWidth") / 2));
    this->addMeasure("titleTextHeight", this->getMeasure("contentBoxHeight") / 13);
    this->addMeasure("titleTextX", this->getMeasure("contentBoxX") + (this->getMeasure("contentBoxWidth") / 4));
    this->addMeasure("titleTextY", this->getMeasure("contentBoxY") + (this->getMeasure("contentBoxHeight") / 26));
    this->addMeasure("titleTextMaxX", this->getMeasure("titleTextX") + this->getMeasure("titleTextWidth"));
    this->addMeasure("titleTextMaxY", this->getMeasure("titleTextY") + this->getMeasure("titleTextHeight"));

    this->addMeasure("systemGridAreaWidth", this->getMeasure("contentBoxWidth") - (this->getMeasure("contentBoxWidth") / 4));
    this->addMeasure("systemGridAreaHeight", this->getMeasure("contentBoxHeight") * 10 / 13);
    this->addMeasure("systemGridAreaX", this->getMeasure("contentBoxX") + (this->getMeasure("contentBoxWidth") / 8));
    this->addMeasure("systemGridAreaY", this->getMeasure("contentBoxY") + (this->getMeasure("contentBoxHeight") * 2 / 13));

    this->addMeasure("systemAreaWidth", this->getMeasure("systemGridAreaWidth") / 3);
    this->addMeasure("systemAreaHeight", this->getMeasure("systemGridAreaHeight") / 4);

    this->addMeasure("systemLogoHorizontalMargin", this->getMeasure("systemAreaWidth") / 6);
    this->addMeasure("systemLogoVerticalMargin", this->getMeasure("systemAreaHeight") / 16);
    this->addMeasure("systemLogoWidth", this->getMeasure("systemAreaWidth") - (this->getMeasure("systemAreaWidth") / 3));
    this->addMeasure("systemLogoHeight", this->getMeasure("systemAreaHeight") * 5 / 8);

    this->addMeasure("systemSelectorWidth", this->getMeasure("systemAreaWidth"));
    this->addMeasure("systemSelectorHeight", this->getMeasure("systemAreaHeight") - (this->getMeasure("systemAreaHeight") / 8));

    this->addMeasure("systemTextHorizontalMargin", this->getMeasure("systemAreaWidth") / 6);
    this->addMeasure("systemTextVerticalMargin", this->getMeasure("systemLogoHeight"));
    this->addMeasure("systemTextAreaWidth", this->getMeasure("systemAreaWidth") - (this->getMeasure("systemAreaWidth") / 3));
    this->addMeasure("systemTextAreaHeight", this->getMeasure("systemAreaHeight") / 4);

    this->addMeasure("arrowWidth", this->getMeasure("contentBoxWidth") / 12);
    this->addMeasure("arrowHeight", this->getMeasure("contentBoxHeight") / 52);
    this->addMeasure("arrowY", this->getMeasure("contentBoxY") + (this->getMeasure("contentBoxHeight") * 49 / 52));
    this->addMeasure("arrowYMax", this->getMeasure("arrowY") + this->getMeasure("arrowHeight"));
    this->addMeasure("downArrowX", this->getMeasure("contentBoxX") + (this->getMeasure("contentBoxWidth") * 5 / 12 ));
    this->addMeasure("downArrowXMax", this->getMeasure("downArrowX") + this->getMeasure("arrowWidth"));
    this->addMeasure("upArrowX", this->getMeasure("contentBoxX") + (this->getMeasure("contentBoxWidth") / 2));
    this->addMeasure("upArrowXMax", this->getMeasure("upArrowX") + this->getMeasure("arrowWidth"));

    return this;
}

Image* HomeView::getBackground() const
{
    return this->m_pBackground;
}

HomeView* HomeView::setBackground(Image* background)
{
    if(this->m_pBackground != nullptr)
    {
        delete this->m_pBackground;
    }
    this->m_pBackground = background;
    return this;
}

RoundedRectangle* HomeView::getContentBox() const
{
    return this->m_pContentBox;
}

HomeView* HomeView::setContentBox(RoundedRectangle* contentBox)
{
    if(this->m_pContentBox != nullptr)
    {
        delete this->m_pContentBox;
    }
    this->m_pContentBox = contentBox;
    return this;
}

Text* HomeView::getTitleText() const
{
    return this->m_pTitleText;
}

HomeView* HomeView::setTitleText(Text* titleText)
{
    if(this->m_pTitleText != nullptr)
    {
        delete this->m_pTitleText;
    }
    this->m_pTitleText = titleText;
    return this;
}

RoundedRectangle* HomeView::getSelector() const
{
    return this->m_pSelector;
}

HomeView* HomeView::setSelector(RoundedRectangle* selector)
{
    if(this->m_pSelector != nullptr)
    {
        delete this->m_pSelector;
    }
    this->m_pSelector = selector;
    return this;
}

Text* HomeView::getSubText(int system) const
{
    if(this->getSubTexts().count(system) > 0)
    {
        return this->getSubTexts().at(system);
    }
    return nullptr;
}

std::map<int, Text*> HomeView::getSubTexts() const
{
    return this->m_pSubTexts;
}

HomeView* HomeView::addSubText(int system, Text* subText)
{
    if(this->m_pSubTexts.count(system) > 0)
    {
        delete this->m_pSubTexts.at(system);
        this->m_pSubTexts.erase(system);
    }
    this->m_pSubTexts.insert(pair<int, Text*>(system, subText));
    return this;
}

Image* HomeView::getDownArrow() const
{
    return this->m_pDownArrow;
}

HomeView* HomeView::setDownArrow(Image* downArrow)
{
    if(this->m_pDownArrow != nullptr)
    {
        delete this->m_pDownArrow;
    }
    this->m_pDownArrow = downArrow;
    return this;
}

Image* HomeView::getUpArrow() const
{
    return this->m_pUpArrow;
}

HomeView* HomeView::setUpArrow(Image* upArrow)
{
    if(this->m_pUpArrow != nullptr)
    {
        delete this->m_pUpArrow;
    }
    this->m_pUpArrow = upArrow;
    return this;
}

Image* HomeView::getUpArrowGreyed() const
{
    return this->m_pUpArrowGreyed;
}

HomeView* HomeView::setUpArrowGreyed(Image* upArrowGreyed)
{
    if(this->m_pUpArrowGreyed != nullptr)
    {
        delete this->m_pUpArrowGreyed;
    }
    this->m_pUpArrowGreyed = upArrowGreyed;
    return this;
}

Image* HomeView::getDownArrowGreyed() const
{
    return this->m_pDownArrowGreyed;
}

HomeView* HomeView::setDownArrowGreyed(Image* downArrowGreyed)
{
    if(this->m_pDownArrowGreyed != nullptr)
    {
        delete this->m_pDownArrowGreyed;
    }
    this->m_pDownArrowGreyed = downArrowGreyed;
    return this;
}
