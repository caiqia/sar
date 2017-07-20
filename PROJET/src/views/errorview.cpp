#include <iostream>
#include <regex>

#include "homeView.hpp"
#include "errorview.hpp"
#include "view.hpp"
#include "surface.hpp"
#include "text.hpp"
#include "../controllers/controller.hpp"
#include "../controllers/homeController.hpp"
#include "../entities/log.hpp"

//View for the errors display window

using namespace std;
const NostalColors HEADER_COLOR = NostalColors::LIGHT_BLUE;
const NostalColors TITLE_COLOR = NostalColors::WHITE;
const NostalColors CONTENT_WINDOW_COLOR = NostalColors::WHITE;
const NostalColors CONTENT_COLOR = NostalColors::LIGHT_BLUE;
const NostalColors BUTTON_COLOR = NostalColors::LIGHT_BLUE;
const NostalColors BUTTON_HILIGHT_COLOR = NostalColors::MAIN_BLUE;
const NostalColors BUTTON_CONTENT_COLOR = NostalColors::WHITE;

const int HEADER_SLOPE = 100;
const int WINDOW_SLOPE = 20;
const int BUTTON_SLOPE = 20;

const string INFOQUIT = "appuyez sur A pour quitter";
const string INFOCONTINUE = "appuyez sur A pour continuer";
const string MAJOR = "oops... il y a une erreur majeur !";
const string EXECUTION = "oops... il y a une erreur execution !";

//Init

MajorErrorView::MajorErrorView(View* view): AbstractView(view, ViewsID::MAJOR_ERROR_VIEW)
{
    Log::addEntry(1, "MajorErrorView initialized successfully");
    return;
}

MajorErrorView::~MajorErrorView()
{
    Log::addEntry(1, "Deleted MajorErrorView");
    return;
}

//Main rendering function
MajorErrorView * MajorErrorView::render(const ViewData& viewData)
{
   int infoTextFontSize = Font::getBiggestFontToFit(FontFamily::ROBOTO, INFOQUIT, this->getMeasure("displayableWidth")/ 4, this->getMeasure("displayableHeight")/16);
    AbstractView::render(viewData);
    //Renders the window background
   if(this->getWindow() != nullptr)
    {
        this->getView()->addToRender(this->getWindow());
    }
    if(this->getInfoText() != nullptr)
     {
         this->getView()->addToRender(this->getInfoText());
     }
     if(this->getTitleText() != nullptr)
      {
          this->getView()->addToRender(this->getTitleText());
      }
      this->setErrorMessage(new Text(this->getView()->getRenderer(), "[ ERROR CODE : "+ this->getView()->getController()->viewData.errorCode + " ] : "+ this->getView()->getController()->viewData.errorMessage , Font::getFontFromMap(FontFamily::ROBOTO, infoTextFontSize), Color::getColorFromMap(NostalColors::WHITE), this->getMeasure("displayableWidth")*7/50, this->getMeasure("displayableHeight")*25/50));
      this->getView()->addToRender(this->getErrorMessage());

return this;
}


MajorErrorView* MajorErrorView::loadImages()
{
    this->setWindow(new Rectangle(this->getView()->getRenderer(), Color::getColorFromMap(NostalColors::LIGHT_BLUE), this->getMeasure("displayableWidth"), this->getMeasure("displayableHeight"), 0, 0));
    int infoTextFontSize = Font::getBiggestFontToFit(FontFamily::ROBOTO, INFOQUIT, this->getMeasure("displayableWidth")/ 4, this->getMeasure("displayableHeight")/16);
    this->setInfoText(new Text(this->getView()->getRenderer(), INFOQUIT, Font::getFontFromMap(FontFamily::ROBOTO, infoTextFontSize), Color::getColorFromMap(NostalColors::WHITE), this->getMeasure("displayableWidth")*3/10, this->getMeasure("displayableHeight")*7/10));
    this->setTitleText(new Text(this->getView()->getRenderer(), MAJOR, Font::getFontFromMap(FontFamily::ROBOTO, infoTextFontSize), Color::getColorFromMap(NostalColors::WHITE), this->getMeasure("displayableWidth")*3/10, this->getMeasure("displayableHeight")*3/10));

    return this;

}


MajorErrorView* MajorErrorView::destroyImages()
{
    if(this->getInfoText() != nullptr)
    {

        this->setInfoText(nullptr);
    }

    if(this->getTitleText() != nullptr)
    {

        this->setTitleText(nullptr);
    }

    if(this->getErrorMessage() != nullptr)
    {

        this->m_errormessage = nullptr;
    }

    if(this->getWindow() != nullptr)
    {

        this->setWindow(nullptr);
    }

    return this;
}

MajorErrorView* MajorErrorView::setInfoText(Text* infoText)
{
    if(this->m_infoText != nullptr)
    {
        delete this->m_infoText;
    }
    this->m_infoText = infoText;
    return this;

}

Text* MajorErrorView::getInfoText() const
{
    return this->m_infoText;
}

MajorErrorView* MajorErrorView::setTitleText(Text* titleText)
{
    if(this->m_titleText != nullptr)
    {
        delete this->m_titleText;
    }
    this->m_titleText = titleText;
    return this;
}

Text* MajorErrorView::getTitleText() const
{
    return this->m_titleText;
}

MajorErrorView* MajorErrorView::setErrorMessage(Text* ErrorMessage)
{
    if(this->m_errormessage != nullptr)
    {
        delete this->m_errormessage;
    }
    this->m_errormessage = ErrorMessage;
    return this;
}

Text* MajorErrorView::getErrorMessage() const
{
    return this->m_errormessage;
}

MajorErrorView* MajorErrorView::setWindow(Rectangle* window)
{
    if(this->m_window != nullptr)
    {
        delete this->m_window;
    }
    this->m_window = window;
    return this;
}

Rectangle* MajorErrorView::getWindow() const
{
    return this->m_window;
}

ExecutionErrorView::ExecutionErrorView(View* view): AbstractView(view, ViewsID::EXECUTION_ERROR_VIEW)
{
    Log::addEntry(1, "ExecutionErrorView initialized successfully");
    return;
}

ExecutionErrorView::~ExecutionErrorView()
{
    Log::addEntry(1, "Deleted ExecutionErrorView");
    return;
}

//Main rendering function
ExecutionErrorView * ExecutionErrorView::render(const ViewData& viewData)
{

     AbstractView::render(viewData);
     //Renders the window background
    if(this->getWindowHeader() != nullptr)
     {
         this->getView()->addToRender(this->getWindowHeader());
     }


      if(this->getContentWindow() != nullptr)
       {
           this->getView()->addToRender(this->getContentWindow());
       }
       if(this->getButton() != nullptr)
        {
            this->getView()->addToRender(this->getButton());
        }
        if(this->getHeaderText() != nullptr)
         {
             this->getView()->addToRender(this->getHeaderText());

         }
         if(this->getInfo() != nullptr)
          {

              this->getView()->addToRender(this->getInfo());

          }
        if(this->getButtonContent() != nullptr)
         {
             this->getView()->addToRender(this->getButtonContent());
         }

       this->setContent(new Text(this->getView()->getRenderer(), "[ ERROR CODE : "+ this->getView()->getController()->viewData.errorCode + " ] : "+ this->getView()->getController()->viewData.errorMessage , Font::getFontFromMap(FontFamily::ROBOTO, 20), Color::getColorFromMap(CONTENT_COLOR), this->getMeasure("ContentX"), this->getMeasure("ContentY")));

       this->getView()->addToRender(this->getContent());

 return this;
 }


ExecutionErrorView* ExecutionErrorView::loadImages()
{

    this->setWindowHeader(new RoundedRectangle(this->getView()->getRenderer(), Color::getColorFromMap(HEADER_COLOR), this->getMeasure("WindowHeaderWidth"), this->getMeasure("WindowHeaderHeight"), this->getMeasure("WindowHeaderX"), this->getMeasure("WindowHeaderY"), RoundedAngles::TOP_ANGLES, HEADER_SLOPE));

    this->setHeaderText(new Text(this->getView()->getRenderer(), EXECUTION,Font::getFontFromMap(FontFamily::ROBOTO, 24), Color::getColorFromMap(TITLE_COLOR), this->getMeasure("HeaderTextX"), this->getMeasure("HeaderTextY")));

    this->setContentWindow(new RoundedRectangle(this->getView()->getRenderer(), Color::getColorFromMap(CONTENT_WINDOW_COLOR), this->getMeasure("ContentWindowWidth"), this->getMeasure("ContentWindowHeight"), this->getMeasure("ContentWindowX"), this->getMeasure("ContentWindowY"), RoundedAngles::BOTTOM_ANGLES, WINDOW_SLOPE));

    this->setInfo(new Text(this->getView()->getRenderer(), INFOCONTINUE, Font::getFontFromMap(FontFamily::ROBOTO, 20), Color::getColorFromMap(CONTENT_COLOR), this->getMeasure("infoX"), this->getMeasure("infoY") ));
    this->setButton(new RoundedRectangle(this->getView()->getRenderer(), Color::getColorFromMap(BUTTON_COLOR), this->getMeasure("ButtonWidth"), this->getMeasure("ButtonHeight"), this->getMeasure("ButtonX"), this->getMeasure("ButtonY"), RoundedAngles::ALL_ANGLES, BUTTON_SLOPE));

    this->setButtonContent(new Text(this->getView()->getRenderer(),"continuer", Font::getFontFromMap(FontFamily::ROBOTO, 20), Color::getColorFromMap(BUTTON_CONTENT_COLOR), this->getMeasure("ButtonTextX"), this->getMeasure("ButtonTextY")));


    return this;
}

ExecutionErrorView* ExecutionErrorView::destroyImages()
{

        this->setWindowHeader(nullptr);
        this->setHeaderText(nullptr);

        this->setButtonContent(nullptr);
        this->setContentWindow(nullptr);

        this->setContent(nullptr);

        this->setButton(nullptr);

        this->setInfo(nullptr);

    return this;
}

ExecutionErrorView* ExecutionErrorView::calculateMeasures()
{
    AbstractView::calculateMeasures();
    this->addMeasure("WindowHeaderWidth", this->getMeasure("compensatedWidth") * 2/3 + this->getMeasure("compensatedWidth")/18);
    this->addMeasure("WindowHeaderX", this->getMeasure("compensatedWidth")/3 - this->getMeasure("compensatedWidth")/36 );
    this->addMeasure("WindowHeaderHeight", this->getMeasure("compensatedHeight") / 20);
    this->addMeasure("WindowHeaderY", this->getMeasure("compensatedHeight") / 3);

    this->addMeasure("HeaderTextX", this->getMeasure("compensatedWidth")/2 );
    this->addMeasure("HeaderTextY", this->getMeasure("compensatedHeight") / 3+ this->getMeasure("compensatedWidth")/180 );

    this->addMeasure("infoX", this->getMeasure("compensatedWidth")/2 + this->getMeasure("compensatedWidth")/18 );
    this->addMeasure("infoY", this->getMeasure("compensatedHeight") *5/12 + this->getMeasure("compensatedWidth")/9 );

    this->addMeasure("ContentWindowWidth", this->getMeasure("compensatedWidth") * 2/3 + this->getMeasure("compensatedWidth")/18);
    this->addMeasure("ContentWindowX", this->getMeasure("compensatedWidth")/3 - this->getMeasure("compensatedWidth")/36 );
    this->addMeasure("ContentWindowHeight", this->getMeasure("compensatedHeight") /3);
    this->addMeasure("ContentWindowY", this->getMeasure("compensatedHeight")*23/60);

    this->addMeasure("ContentX", this->getMeasure("compensatedWidth")/3  );
    this->addMeasure("ContentY", this->getMeasure("compensatedHeight") *5/12);

    this->addMeasure("ButtonWidth", this->getMeasure("compensatedWidth")*19/180);
    this->addMeasure("ButtonX", this->getMeasure("compensatedWidth")*2/3 - this->getMeasure("compensatedWidth")*2/45 );
    this->addMeasure("ButtonHeight", this->getMeasure("compensatedWidth")*2/45);
    this->addMeasure("ButtonY", this->getMeasure("compensatedHeight") *5/8);

    this->addMeasure("ButtonTextX", this->getMeasure("compensatedWidth")*2/3 - this->getMeasure("compensatedWidth")*6/181 );
    this->addMeasure("ButtonTextY", this->getMeasure("compensatedHeight") *5/8 + this->getMeasure("compensatedWidth")/140 );



    return this;

}

RoundedRectangle* ExecutionErrorView::getWindowHeader() const
{
    return this->m_pWindowHeader;
}

ExecutionErrorView* ExecutionErrorView::setWindowHeader(RoundedRectangle* windowheader)
{
    if(this->m_pWindowHeader != nullptr)
    {
        delete this->m_pWindowHeader;
    }
    this->m_pWindowHeader = windowheader;
    return this;

}

Text* ExecutionErrorView::getHeaderText() const
{
    return this->m_pHeaderText;
}

ExecutionErrorView* ExecutionErrorView::setHeaderText(Text* headerText)
{
    if(this->m_pHeaderText != nullptr)
    {
        delete this->m_pHeaderText;
    }
    this->m_pHeaderText = headerText;
    return this;
}

RoundedRectangle* ExecutionErrorView::getContentWindow() const
{
    return this->m_pContentWindow;
}

ExecutionErrorView* ExecutionErrorView::setContentWindow(RoundedRectangle* contentwindow)
{
    if(this->m_pContentWindow != nullptr)
    {
        delete this->m_pContentWindow;
    }
    this->m_pContentWindow = contentwindow;
    return this;

}

ExecutionErrorView* ExecutionErrorView::setInfo(Text* info)
{
    if(this->m_info != nullptr)
    {
        delete this->m_info;
    }
    this->m_info = info;
    return this;

}

Text* ExecutionErrorView::getInfo() const
{
    return this->m_info;
}

Text* ExecutionErrorView::getContent() const
{
    return this->m_pContent;
}

ExecutionErrorView* ExecutionErrorView::setContent(Text* content)
{
    if(this->m_pContent != nullptr)
    {
        delete this->m_pContent;
    }
    this->m_pContent = content;
    return this;
}

Text* ExecutionErrorView::getButtonContent() const
{
    return this->m_pButtonContent;
}

ExecutionErrorView* ExecutionErrorView::setButtonContent(Text* buttoncontent)
{
    if(this->m_pButtonContent != nullptr)
    {
        delete this->m_pButtonContent;
    }
    this->m_pButtonContent = buttoncontent;
    return this;
}


RoundedRectangle* ExecutionErrorView::getButton() const
{
    return this->m_pButton;
}

ExecutionErrorView* ExecutionErrorView::setButton(RoundedRectangle* button)
{
    if(this->m_pButton != nullptr)
    {
        delete this->m_pButton;
    }
    this->m_pButton = button;
    return this;

}
