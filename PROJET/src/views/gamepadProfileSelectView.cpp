#include "gamepadProfileSelectView.hpp"
#include "../entities/error.hpp"

#include <iostream>

using namespace std;

const NostalColors HEADER_COLOR = NostalColors::LIGHT_BLUE;
const NostalColors TITLE_COLOR = NostalColors::WHITE;
const NostalColors CONTENT_WINDOW_COLOR = NostalColors::WHITE;
const NostalColors SELECTOR_COLOR = NostalColors::LIGHT_BLUE_SEMI_TRANSPARENT;

const string SNES_PROFILE_IMAGE = "/nostal/IMG/RESSOURCES/GAMEPAD/SNES.png";
const string NES_PROFILE_IMAGE = "/nostal/IMG/RESSOURCES/GAMEPAD/NES.png";

const int HEADER_SLOPE = 100;
const int WINDOW_SLOPE = 20;
const int SELECTOR_SLOPE = 20;

GamepadProfileSelectView::GamepadProfileSelectView(View* view)
: AbstractView(view, GAMEPAD_PROFILE_SELECT_VIEW)
{
    this->calculateMeasures();
}

GamepadProfileSelectView::~GamepadProfileSelectView()
{

}

GamepadProfileSelectView* GamepadProfileSelectView::render(const ViewData& viewData)
{
    AbstractView::render(viewData);

    if(this->getWindowHeader() != nullptr)
    {
        this->getView()->addToRender(this->getWindowHeader());
    }

    if(viewData.configuredGamepadNumber != this->currentlyDisplayedPlayer)
    {
        string text = "Joueur " + to_string(viewData.configuredGamepadNumber + 1) + " sélectionnez votre format de manette";
        this->setHeaderText(text);
        this->currentlyDisplayedPlayer = viewData.configuredGamepadNumber;
    }

    if(this->getHeaderText() != nullptr)
    {
        this->getView()->addToRender(this->getHeaderText());
    }

    if(this->getContentWindow() != nullptr)
    {
        this->getView()->addToRender(this->getContentWindow());
    }

    int i = 0;
    if(viewData.profilesToChoseFrom.size() == 2)
    {
        for(const auto& ent : viewData.profilesToChoseFrom)
        {
            if(i == viewData.currentlySelectedProfile)
            {
                int posX;

                posX = this->getMeasure("gamepadAreaFirstX") + i * (this->getMeasure("gamepadAreaWidth") + this->getMeasure("interGamepadSpacing"));

                if(this->getSelector() != nullptr)
                {
                    this->getSelector()->setPosition(posX, this->getMeasure("gamepadAreaY"));
                    this->getView()->addToRender(this->getSelector());
                }
            }

            int posX = this->getMeasure("gamepadAreaFirstX") + this->getMeasure("gamepadAreaHorizontalPadding") + i * (this->getMeasure("gamepadAreaWidth") + this->getMeasure("interGamepadSpacing"));
            int posXMax = posX + this->getMeasure("gamepadImageWidth");

            if(ent == "NES" && this->getNESProfile() != nullptr)
            {
                this->getNESProfile()->center(posX, posXMax, this->getMeasure("gamepadImageY"), this->getMeasure("gamepadImageYMax"));
                this->getView()->addToRender(this->getNESProfile());
            }

            else if(ent == "SNES" && this->getSNESProfile() != nullptr)
            {
                this->getSNESProfile()->center(posX, posXMax, this->getMeasure("gamepadImageY"), this->getMeasure("gamepadImageYMax"));
                this->getView()->addToRender(this->getSNESProfile());
            }

            ++i;
        }
    }

    return this;
}

GamepadProfileSelectView* GamepadProfileSelectView::loadImages()
{

    AbstractView::loadImages();

    this->setWindowHeader(new RoundedRectangle(this->getView()->getRenderer(), Color::getColorFromMap(HEADER_COLOR), this->getMeasure("windowWidth"), this->getMeasure("headerHeight"), this->getMeasure("windowX"), this->getMeasure("windowY"), RoundedAngles::TOP_ANGLES, HEADER_SLOPE));

    this->setHeaderText(new Text(this->getView()->getRenderer(), "",Font::getFontFromMap(FontFamily::ROBOTO, 12), Color::getColorFromMap(TITLE_COLOR), this->getMeasure("titleX"), this->getMeasure("titleY")));

    this->setContentWindow(new RoundedRectangle(this->getView()->getRenderer(), Color::getColorFromMap(CONTENT_WINDOW_COLOR), this->getMeasure("windowWidth"), this->getMeasure("contentWindowHeight"), this->getMeasure("windowX"), this->getMeasure("contentWindowY"), RoundedAngles::BOTTOM_ANGLES, WINDOW_SLOPE));

    this->setSelector(new RoundedRectangle(this->getView()->getRenderer(), Color::getColorFromMap(SELECTOR_COLOR), this->getMeasure("gamepadAreaWidth"), this->getMeasure("gamepadAreaHeight"), this->getMeasure("gamepadAreaFirstX"), this->getMeasure("gamepadAreaY"), RoundedAngles::ALL_ANGLES, SELECTOR_SLOPE));

    this->setSNESProfile(new Image(this->getView()->getRenderer(), SNES_PROFILE_IMAGE));
    if(this->getSNESProfile() != nullptr)
    {
        this->getSNESProfile()->setSize(this->getMeasure("gamepadImageWidth"), this->getMeasure("gamepadImageHeight"), FIT);
    }

    this->setNESProfile(new Image(this->getView()->getRenderer(), NES_PROFILE_IMAGE));
    if(this->getNESProfile() != nullptr)
    {
        this->getNESProfile()->setSize(this->getMeasure("gamepadImageWidth"), this->getMeasure("gamepadImageHeight"), FIT);
    }

    return this;
}

GamepadProfileSelectView* GamepadProfileSelectView::destroyImages()
{
    AbstractView::destroyImages();

    this->setWindowHeader(nullptr);

    this->setHeaderText(nullptr);

    this->setContentWindow(nullptr);

    this->setSelector(nullptr);

    this->setSNESProfile(nullptr);

    this->setNESProfile(nullptr);

    return this;
}

GamepadProfileSelectView* GamepadProfileSelectView::calculateMeasures()
{
    AbstractView::calculateMeasures();

    this->addMeasure("windowWidth", this->getMeasure("compensatedWidth") * 82 / 128);
    this->addMeasure("windowX", this->getMeasure("compensatedX") + (this->getMeasure("compensatedWidth") * 23 / 128));
    this->addMeasure("windowHeight", this->getMeasure("compensatedHeight") * 26 / 64);
    this->addMeasure("windowY", this->getMeasure("compensatedY") + (this->getMeasure("compensatedHeight") * 24 / 64));

    this->addMeasure("headerHeight", this->getMeasure("windowHeight") * 5 / 26);
    this->addMeasure("headerVerticalPadding", this->getMeasure("headerHeight") / 5);
    this->addMeasure("headerHorizontalPadding", this->getMeasure("headerHeight"));

    this->addMeasure("titleWidth", this->getMeasure("windowWidth") - (2 * this->getMeasure("headerHorizontalPadding")));
    this->addMeasure("titleHeight", this->getMeasure("headerHeight") - (2 * this->getMeasure("headerVerticalPadding")));
    this->addMeasure("titleX", this->getMeasure("windowX") + this->getMeasure("headerHorizontalPadding"));
    this->addMeasure("titleXMax", this->getMeasure("titleX") + this->getMeasure("titleWidth"));
    this->addMeasure("titleY", this->getMeasure("windowY") + this->getMeasure("headerVerticalPadding"));
    this->addMeasure("titleYMax", this->getMeasure("titleY") + this->getMeasure("titleHeight"));

    this->addMeasure("contentWindowHeight", this->getMeasure("windowHeight") - this->getMeasure("headerHeight"));
    this->addMeasure("contentWindowY", this->getMeasure("windowY") + this->getMeasure("headerHeight"));
    this->addMeasure("contentWindowTopPadding", this->getMeasure("contentWindowHeight") * 4 / 21);
    this->addMeasure("contentWindowBottomPadding", this->getMeasure("contentWindowHeight") * 3 / 21);
    this->addMeasure("contentWindowHorizontalMargin", this->getMeasure("windowWidth") * 4 / 82);
    this->addMeasure("interGamepadSpacing", this->getMeasure("windowWidth") * 2 / 82);

    this->addMeasure("gamepadAreaWidth", (this->getMeasure("windowWidth") - this->getMeasure("interGamepadSpacing") - (2 * this->getMeasure("contentWindowHorizontalMargin"))) / 2);
    this->addMeasure("gamepadAreaHeight", this->getMeasure("contentWindowHeight") - this->getMeasure("contentWindowTopPadding") - this->getMeasure("contentWindowBottomPadding"));
    this->addMeasure("gamepadAreaY", this->getMeasure("contentWindowY") + this->getMeasure("contentWindowTopPadding"));
    this->addMeasure("gamepadAreaYMax", this->getMeasure("gamepadAreaY") + this->getMeasure("gamepadAreaHeight"));
    this->addMeasure("gamepadAreaFirstX", this->getMeasure("windowX") + this->getMeasure("contentWindowHorizontalMargin"));

    this->addMeasure("gamepadAreaHorizontalPadding", this->getMeasure("gamepadAreaWidth") * 2 / 36);
    this->addMeasure("gamepadAreaVerticalPadding", this->getMeasure("gamepadAreaHeight") / 14);

    this->addMeasure("gamepadImageHeight", this->getMeasure("gamepadAreaHeight") - (2 * this->getMeasure("gamepadAreaVerticalPadding")));
    this->addMeasure("gamepadImageWidth", this->getMeasure("gamepadAreaWidth") - (2 * this->getMeasure("gamepadAreaHorizontalPadding")));
    this->addMeasure("gamepadImageY", this->getMeasure("gamepadAreaY") + this->getMeasure("gamepadAreaVerticalPadding"));
    this->addMeasure("gamepadImageYMax", this->getMeasure("gamepadImageY") + this->getMeasure("gamepadImageHeight"));

    return this;
}


GamepadProfileSelectView* GamepadProfileSelectView::setWindowHeader(RoundedRectangle* windowHeader)
{
    if(this->m_pWindowHeader != nullptr)
    {
        delete this->m_pWindowHeader;
    }
    this->m_pWindowHeader = windowHeader;
    return this;
}

RoundedRectangle* GamepadProfileSelectView::getWindowHeader() const
{
    return this->m_pWindowHeader;
}

GamepadProfileSelectView* GamepadProfileSelectView::setHeaderText(Text* headerText)
{
    if(this->m_pHeaderText != nullptr)
    {
        delete this->m_pHeaderText;
    }
    this->m_pHeaderText = headerText;
    return this;
}

GamepadProfileSelectView* GamepadProfileSelectView::setHeaderText(string text)
{
    if(this->getHeaderText() != nullptr)
    {
        int font = Font::getBiggestFontToFit(FontFamily::ROBOTO, text, this->getMeasure("titleWidth"), this->getMeasure("titleHeight"));
        this->getHeaderText()->setText(text);
        this->getHeaderText()->setFont(Font::getFontFromMap(FontFamily::ROBOTO, font));
        this->getHeaderText()->reload();
        this->getHeaderText()->center(this->getMeasure("titleX"), this->getMeasure("titleXMax"), this->getMeasure("titleY"), this->getMeasure("titleYMax"));
    }
    return this;
}

Text* GamepadProfileSelectView::getHeaderText() const
{
    return this->m_pHeaderText;
}

GamepadProfileSelectView* GamepadProfileSelectView::setContentWindow(RoundedRectangle* contentWindow)
{
    if(this->m_pContentWindow != nullptr)
    {
        delete this->m_pContentWindow;
    }
    this->m_pContentWindow = contentWindow;
    return this;
}

RoundedRectangle* GamepadProfileSelectView::getContentWindow() const
{
    return this->m_pContentWindow;
}

GamepadProfileSelectView* GamepadProfileSelectView::setSelector(RoundedRectangle* selector)
{
    if(this->m_pSelector != nullptr)
    {
        delete this->m_pSelector;
    }

    this->m_pSelector = selector;

    return this;
}

RoundedRectangle* GamepadProfileSelectView::getSelector() const
{
    return this->m_pSelector;
}

GamepadProfileSelectView* GamepadProfileSelectView::setSNESProfile(Image* SNESProfile)
{
    if(this->m_pSNESProfile != nullptr)
    {
        delete this->m_pSNESProfile;
    }

    this->m_pSNESProfile = SNESProfile;

    return this;
}

Image* GamepadProfileSelectView::getSNESProfile() const
{
    return this->m_pSNESProfile;
}

GamepadProfileSelectView* GamepadProfileSelectView::setNESProfile(Image* NESProfile)
{
    if(this->m_pNESProfile != nullptr)
    {
        delete this->m_pNESProfile;
    }

    this->m_pNESProfile = NESProfile;

    return this;
}

Image* GamepadProfileSelectView::getNESProfile() const
{
    return this->m_pNESProfile;
}
