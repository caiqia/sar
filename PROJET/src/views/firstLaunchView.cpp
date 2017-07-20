#include "firstLaunchView.hpp"
#include "view.hpp"

#include "surface.hpp"
#include "../entities/window.hpp"
#include "../entities/log.hpp"
#include "text.hpp"
#include "../controllers/controller.hpp"
#include "../controllers/firstLaunchController.hpp"
#include <iostream>

using namespace std;

const string BACKGROUND43 = "/nostal/IMG/WIZARD_BG_4-3.png";
const string BACKGROUND169 = "/nostal/IMG/WIZARD_BG_16-9.png";
const string TITLE = "Bienvenue sur le Retropac";
const string EXPLANATION = "Veuillez patienter, votre console est en cours de configuration";




FirstLaunchView::FirstLaunchView(View* view)
: AbstractView(view, ViewsID::FIRST_LAUCH_VIEW)
{
    Log::addEntry(1, "FirstLaunchView initialized successfully");
    return;
}

FirstLaunchView::~FirstLaunchView()
{
    Log::addEntry(1, "Deleted FirstLaunchView");
    return;

}

FirstLaunchView* FirstLaunchView::render(const ViewData& viewData)
{
    AbstractView::render(viewData);

    if(this->getBackGround() != nullptr)
    {
        this->getView()->addToRender(this->getBackGround());
    }
    if(this->getTitleText() != nullptr)
    {
        this->getView()->addToRender(this->getTitleText());

    }

    if(this->getExplanationText() != nullptr)
    {
        this->getView()->addToRender(this->getExplanationText());
    }


    return this;

}

FirstLaunchView* FirstLaunchView::loadImages()
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

    int titleTextFontSize = Font::getBiggestFontToFit(FontFamily::ROBOTO, TITLE, this->getMeasure("titleTextWidth"), this->getMeasure("titleTextHeight"));
    int explanationFontSize = Font::getBiggestFontToFit(FontFamily::ROBOTO, EXPLANATION, this->getMeasure("explanationTextWidth"), this->getMeasure("explanationTextHeight"));

    this->setBackGround(new Image(this->getView()->getRenderer(), backgroundToLoad, this->getMeasure("displayableWidth"), this->getMeasure("displayableHeight"), this->getMeasure("displayableX"), this->getMeasure("displayableY")));

    this->setTitleText(new Text(this->getView()->getRenderer(), TITLE, Font::getFontFromMap(FontFamily::ROBOTO, titleTextFontSize), Color::getColorFromMap(NostalColors::WHITE), this->getMeasure("titleTextX"), this->getMeasure("titleTextY")));

    this->setExplanationText(new WrappedText(this->getView()->getRenderer(), EXPLANATION, FontFamily::ROBOTO, Color::getColorFromMap(NostalColors::WHITE), this->getMeasure("explanationTextWidth"), this->getMeasure("explanationTextHeight"), this->getMeasure("explanationTextX"), this->getMeasure("explanationTextY")));

return this;
}

FirstLaunchView* FirstLaunchView::destroyImages()
{
    if(this->getBackGround() != nullptr)
    {
        this->setBackGround(nullptr);
    }

    if(this->getTitleText() != nullptr)
    {
        this->setTitleText(nullptr);
    }

    if(this->getExplanationText() != nullptr)
    {
        this->setExplanationText(nullptr);
    }

    return this;

}

FirstLaunchView* FirstLaunchView::calculateMeasures()
{
    AbstractView::calculateMeasures();

    this->addMeasure("titleTextWidth", this->getMeasure("compensatedWidth")/3 );
    this->addMeasure("titleTextHeight", this->getMeasure("compensatedHeight") / 15);
    this->addMeasure("titleTextX", this->getMeasure("compensatedWidth")/2 );
    this->addMeasure("titleTextY", this->getMeasure("compensatedHeight")/3);
    this->addMeasure("titleTextMaxX", this->getMeasure("titleTextX") + this->getMeasure("titleTextWidth"));
    this->addMeasure("titleTextMaxY", this->getMeasure("titleTextY") + this->getMeasure("titleTextHeight"));
    this->addMeasure("explanationTextWidth", this->getMeasure("compensatedWidth")*40/100);
    this->addMeasure("explanationTextHeight", this->getMeasure("compensatedHeight") / 15);
    this->addMeasure("explanationTextX",  this->getMeasure("compensatedWidth")*48/100);
    this->addMeasure("explanationTextY", this->getMeasure("compensatedHeight")/2);
    this->addMeasure("explanationTextMaxX", this->getMeasure("explanationTextX") + this->getMeasure("explanationTextWidth"));
    this->addMeasure("explanationTextMaxY", this->getMeasure("explanationTextY") + this->getMeasure("explanationTextHeight"));


    return this;
}



FirstLaunchView* FirstLaunchView::setBackGround(Image* background)
{
    if(this->m_pBackground != nullptr)
    {
        this->m_pBackground == nullptr;
    }
    this->m_pBackground = background;

    return this;
}


Image* FirstLaunchView::getBackGround() const
{

    return this->m_pBackground;
}

FirstLaunchView* FirstLaunchView::setTitleText(Text* titletext)
{
    if(this->m_pTitleText != nullptr)
    {
       this->m_pTitleText = nullptr;
    }
    this->m_pTitleText =  titletext;

    return this;
}

Text* FirstLaunchView::getTitleText() const
{

    return this->m_pTitleText;
}

FirstLaunchView* FirstLaunchView::setExplanationText(WrappedText* explanationtext)
{
    if(this->m_pExplanationText != nullptr)
    {
        this->m_pExplanationText = nullptr;
    }
    this->m_pExplanationText =  explanationtext;


    return this;
}

WrappedText* FirstLaunchView::getExplanationText() const
{

    return this->m_pExplanationText;
}
