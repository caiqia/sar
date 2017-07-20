#include "startupLoadingView.hpp"

#include <iostream>

using namespace std;

const string BACKGROUND43 = "/nostal/IMG/RESSOURCES/LOADING/BACKGROUND_4-3.png";
const string BACKGROUND169 = "/nostal/IMG/RESSOURCES/LOADING/BACKGROUND_16-9.png";
const string LOGO = "/nostal/IMG/RESSOURCES/LOADING/LOGO.png";

const NostalColors LOADING_TEXT_COLOR = NostalColors::WHITE;
const NostalColors PROGRESS_BAR_BACKGROUND_COLOR = NostalColors::GREY;
const NostalColors PROGRESS_BAR_COLOR = NostalColors::WHITE;

const string LOADING_TEXT = "Chargement...";

StartupLoadingView::StartupLoadingView(View* view)
: AbstractView(view, STARTUP_LOADING_VIEW)
{
    this->calculateMeasures();
}

StartupLoadingView::~StartupLoadingView()
{

}

StartupLoadingView* StartupLoadingView::render(const ViewData& viewData)
{
    AbstractView::render(viewData);

    if(this->getBackground() != nullptr)
    {
        this->getView()->addToRender(this->getBackground());
    }

    if(this->getLogo() != nullptr)
    {
        this->getView()->addToRender(this->getLogo());
    }

    if(this->getLoadingText() != nullptr)
    {
        this->getView()->addToRender(this->getLoadingText());
    }

    if(this->getProgressBarBackground() != nullptr)
    {
        this->getView()->addToRender(this->getProgressBarBackground());
    }

    if(this->getProgressBar() != nullptr)
    {
        this->getProgressBar()->setSize(viewData.loadingScreenPercentage * this->getMeasure("progressBarWidth") / 100 ,this->getProgressBar()->getHeight());
        this->getView()->addToRender(this->getProgressBar());
    }

    return this;
}

StartupLoadingView* StartupLoadingView::loadImages()
{
    AbstractView::loadImages();

    string imageToLoad;
    if(this->getView()->getWindow()->getAspectRatio() == "4/3")
    {
        imageToLoad = BACKGROUND43;
    }
    else
    {
        imageToLoad = BACKGROUND169;
    }

    this->setBackground(new Image(this->getView()->getRenderer(), imageToLoad));

    if(this->getBackground() != nullptr)
    {
        this->getBackground()->setPosition(this->getMeasure("displayableX"), this->getMeasure("displayableY"));
        this->getBackground()->setSize(this->getMeasure("displayableWidth"), this->getMeasure("displayableHeight"), CROP);
    }

    this->setLogo(new Image(this->getView()->getRenderer(), LOGO));

    if(this->getLogo() != nullptr)
    {
        this->getLogo()->setSize(this->getMeasure("logoWidth"), this->getMeasure("logoHeight"), FIT);
        this->getLogo()->center(this->getMeasure("logoX"), this->getMeasure("logoXMax"), this->getMeasure("logoY"), this->getMeasure("logoYMax"));
    }

    int fontSize = Font::getBiggestFontToFit(FontFamily::ROBOTO, LOADING_TEXT, this->getMeasure("textWidth"), this->getMeasure("textHeight"));
    this->setLoadingText(new Text(this->getView()->getRenderer(), LOADING_TEXT, Font::getFontFromMap(ROBOTO, fontSize), Color::getColorFromMap(LOADING_TEXT_COLOR), this->getMeasure("textX"), this->getMeasure("textY")));

    if(this->getLoadingText() != nullptr)
    {
        this->getLoadingText()->center(this->getMeasure("textX"), this->getMeasure("textXMax"), this->getMeasure("textY"), this->getMeasure("textYMax"));
    }

    this->setProgressBarBackground(new RoundedRectangle(this->getView()->getRenderer(), Color::getColorFromMap(PROGRESS_BAR_BACKGROUND_COLOR), this->getMeasure("progressBarWidth"), this->getMeasure("progressBarHeight"), this->getMeasure("progressBarX"), this->getMeasure("progressBarY"), ALL_ANGLES, 100));

    this->setProgressBar(new RoundedRectangle(this->getView()->getRenderer(), Color::getColorFromMap(PROGRESS_BAR_COLOR), this->getMeasure("progressBarWidth"), this->getMeasure("progressBarHeight"), this->getMeasure("progressBarX"), this->getMeasure("progressBarY"), ALL_ANGLES, 100));

    return this;
}

StartupLoadingView* StartupLoadingView::destroyImages()
{
    AbstractView::destroyImages();

    this->setBackground(nullptr);
    this->setLogo(nullptr);
    this->setLoadingText(nullptr);
    this->setProgressBarBackground(nullptr);
    this->setProgressBar(nullptr);

    return this;
}

StartupLoadingView* StartupLoadingView::calculateMeasures()
{
    AbstractView::calculateMeasures();

    this->addMeasure("logoWidth", this->getMeasure("compensatedWidth") / 2);
    this->addMeasure("logoHeight", this->getMeasure("compensatedHeight") * 12 / 64);
    this->addMeasure("logoX", this->getMeasure("compensatedX") + (this->getMeasure("compensatedWidth") / 4));
    this->addMeasure("logoXMax", this->getMeasure("logoX") + this->getMeasure("logoWidth"));
    this->addMeasure("logoY", this->getMeasure("compensatedY") + (this->getMeasure("compensatedHeight") * 22 / 64));
    this->addMeasure("logoYMax", this->getMeasure("logoY") + this->getMeasure("logoHeight"));

    this->addMeasure("textWidth", this->getMeasure("compensatedWidth") * 6 / 32);
    this->addMeasure("textHeight", this->getMeasure("compensatedHeight") * 2 / 64);
    this->addMeasure("textX", this->getMeasure("compensatedX") + (this->getMeasure("compensatedWidth") * 13 / 32));
    this->addMeasure("textXMax", this->getMeasure("textX") + this->getMeasure("textWidth"));
    this->addMeasure("textY", this->getMeasure("compensatedY") + (this->getMeasure("compensatedHeight") * 42 / 64));
    this->addMeasure("textYMax", this->getMeasure("textY") + this->getMeasure("textHeight"));

    this->addMeasure("progressBarWidth", this->getMeasure("compensatedWidth") * 14 / 32);
    this->addMeasure("progressBarHeight", this->getMeasure("compensatedHeight") / 64);
    this->addMeasure("progressBarX", this->getMeasure("compensatedX") + (this->getMeasure("compensatedWidth") * 9 / 32));
    this->addMeasure("progressBarXMax", this->getMeasure("progressBarX") + this->getMeasure("progressBarWidth"));
    this->addMeasure("progressBarY", this->getMeasure("compensatedY") + (this->getMeasure("compensatedHeight") * 49 / 64));
    this->addMeasure("progressBarYMax", this->getMeasure("progressBarY") + this->getMeasure("progressBarHeight"));

    return this;
}

Image* StartupLoadingView::getBackground() const
{
    return this->m_pBackground;
}

StartupLoadingView* StartupLoadingView::setBackground(Image* background)
{
    if(this->m_pBackground != nullptr)
    {
        delete this->m_pBackground;
    }
    this->m_pBackground = background;
    return this;
}

Image* StartupLoadingView::getLogo() const
{
        return this->m_pLogo;
}

StartupLoadingView* StartupLoadingView::setLogo(Image* logo)
{
    if(this->m_pLogo != nullptr)
    {
        delete this->m_pLogo;
    }
    this->m_pLogo = logo;
    return this;
}

Text* StartupLoadingView::getLoadingText() const
{
    return this->m_pLoadingText;
}

StartupLoadingView* StartupLoadingView::setLoadingText(Text* loadingText)
{
    if(this->m_pLoadingText != nullptr)
    {
        delete this->m_pLoadingText;
    }
    this->m_pLoadingText = loadingText;
    return this;
}

RoundedRectangle* StartupLoadingView::getProgressBarBackground() const
{
    return this->m_pProgressBarBackground;
}

StartupLoadingView* StartupLoadingView::setProgressBarBackground(RoundedRectangle* progressBarBackground)
{
    if(this->m_pProgressBarBackground != nullptr)
    {
        delete this->m_pProgressBarBackground;
    }
    this->m_pProgressBarBackground = progressBarBackground;
    return this;
}

RoundedRectangle* StartupLoadingView::getProgressBar() const
{
    return this->m_pProgressBar;
}

StartupLoadingView* StartupLoadingView::setProgressBar(RoundedRectangle* progressBar)
{
    if(this->m_pProgressBar != nullptr)
    {
        delete this->m_pProgressBar;
    }
    this->m_pProgressBar = progressBar;
    return this;
}
