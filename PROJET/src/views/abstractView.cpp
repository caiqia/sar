#include "abstractView.hpp"
#include "view.hpp"
#include "../entities/window.hpp"

#include <iostream>

using namespace std;

const int MARGIN_RATIO = 8;

AbstractView::AbstractView(View* view, ViewsID viewID)
{
    this->setViewID(viewID);
    this->setView(view);
    this->getView()->addView(this->getViewID(), this);
}

AbstractView::~AbstractView()
{
}

AbstractView* AbstractView::init()
{
    this->calculateMeasures();
    this->loadImages();
    
    return this;
}

AbstractView* AbstractView::deInit()
{
    return this;
}

AbstractView* AbstractView::render(const ViewData& viewData)
{
    return this;
}

AbstractView* AbstractView::loadImages()
{
    return this;
}

AbstractView* AbstractView::destroyImages()
{
    return this;
}

AbstractView* AbstractView::setView(View* view)
{
    this->m_pView = view;
    return this;
}

View* AbstractView::getView() const
{
    return this->m_pView;
}

AbstractView* AbstractView::calculateMeasures()
{
    this->addMeasure("windowWidth", this->getView()->getWindow()->getWidth());
    this->addMeasure("windowHeight", this->getView()->getWindow()->getHeight());
    this->addMeasure("overscanTop", 0);
    this->addMeasure("overscanRight", 0);
    this->addMeasure("overscanBottom", 0);
    this->addMeasure("overscanLeft", 0);
    this->addMeasure("displayableWidth", this->getMeasure("windowWidth") - this->getMeasure("overscanLeft") - this->getMeasure("overscanRight"));
    this->addMeasure("displayableHeight", this->getMeasure("windowHeight") - this->getMeasure("overscanTop") - this->getMeasure("overscanBottom"));
    this->addMeasure("displayableX", 0 + this->getMeasure("overscanTop"));
    this->addMeasure("displayableY", 0 + this->getMeasure("overscanLeft"));
    if(this->getView()->getWindow()->getAspectRatio() == "16/9")
    {
        this->addMeasure("compensatedWidth", this->getMeasure("displayableWidth") - (this->getMeasure("displayableWidth") / 4));
        this->addMeasure("compensatedHeight", this->getMeasure("displayableHeight"));
        this->addMeasure("compensatedX", this->getMeasure("displayableX") + (this->getMeasure("displayableWidth") / 8));
        this->addMeasure("compensatedY", this->getMeasure("displayableY"));
    }
    else
    {
        this->addMeasure("compensatedWidth", this->getMeasure("displayableWidth"));
        this->addMeasure("compensatedHeight", this->getMeasure("displayableHeight"));
        this->addMeasure("compensatedX", this->getMeasure("displayableX"));
        this->addMeasure("compensatedY", this->getMeasure("displayableY"));
    }

    return this;
}

AbstractView* AbstractView::addMeasure(const string& label, const int& value)
{
    if(this->m_measures.count(label) > 0)
    {
        this->m_measures.erase(label);
    }
    this->m_measures.insert(pair<string, int> (label, value));
    return this;
}

int AbstractView::getMeasure(const string& label) const
{
    if(this->m_measures.count(label) > 0)
    {
        return this->m_measures.at(label);
    }
    else
    {
        return 0; // error not found
    }
}

map <string, int> AbstractView::getMeasures() const
{
    return this->m_measures;
}

AbstractView* AbstractView::setViewID(const ViewsID& viewID)
{
    this->m_viewID = viewID;
}

ViewsID AbstractView::getViewID() const
{
    return this->m_viewID;
}
