#include "popupWindowView.hpp"

#include <iostream>

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

/***************************
 * AbstractPopupWindowView *
 ***************************/
AbstractPopupWindowView::AbstractPopupWindowView(View* view, const ViewsID& viewID)
: AbstractView(view, viewID)
{

}

AbstractPopupWindowView::~AbstractPopupWindowView()
{

}

AbstractPopupWindowView* AbstractPopupWindowView::render(const ViewData& viewData)
{
    AbstractView::render(viewData);

    if(this->getWindowHeader() != nullptr)
    {
        this->getView()->addToRender(this->getWindowHeader());
    }

    if(this->getContentWindow() != nullptr)
    {
        this->getView()->addToRender(this->getContentWindow());
    }

    if(this->getHeaderText() != nullptr)
    {
        if(this->getHeaderText()->getText() != viewData.popupWindowTitle)
        {
            this->setHeaderText(viewData.popupWindowTitle);
        }
        this->getView()->addToRender(this->getHeaderText());
    }

    bool createButtons = false;

    if(this->getButtons().size() != viewData.popupWindowButtons.size())
    {
        createButtons = true;
    }

    if(!createButtons)
    {
        for(const auto& ent : viewData.popupWindowButtons)
        {
            if(ent.second != this->getButtonContent(ent.first)->getText())
            {
                createButtons = true;
            }
        }
    }

    if(createButtons)
    {
        this->createButtons(viewData.popupWindowButtons);
    }

    for(const auto& ent : this->getButtons())
    {
        if(ent.second != nullptr)
        {
            if(ent.first == viewData.popupWindowSelectedButton && ent.second->getColor() != Color::getColorFromMap(BUTTON_HILIGHT_COLOR))
            {
                ent.second->setColor(Color::getColorFromMap(BUTTON_HILIGHT_COLOR));
            }
            else if(ent.first != viewData.popupWindowSelectedButton && ent.second->getColor() != Color::getColorFromMap(BUTTON_COLOR))
            {
                ent.second->setColor(Color::getColorFromMap(BUTTON_COLOR));
            }
            this->getView()->addToRender(ent.second);
        }
    }

    for(const auto& ent : this->getButtonsContent())
    {
        if(ent.second != nullptr)
        {
            this->getView()->addToRender(ent.second);
        }
    }

    return this;
}

AbstractPopupWindowView* AbstractPopupWindowView::loadImages()
{
    AbstractView::loadImages();

    this->setWindowHeader(new RoundedRectangle(this->getView()->getRenderer(), Color::getColorFromMap(HEADER_COLOR), this->getMeasure("windowWidth"), this->getMeasure("headerHeight"), this->getMeasure("windowX"), this->getMeasure("windowY"), RoundedAngles::TOP_ANGLES, HEADER_SLOPE));

    this->setHeaderText(new Text(this->getView()->getRenderer(), "",Font::getFontFromMap(FontFamily::ROBOTO, 12), Color::getColorFromMap(TITLE_COLOR), this->getMeasure("titleX"), this->getMeasure("titleY")));

    this->setContentWindow(new RoundedRectangle(this->getView()->getRenderer(), Color::getColorFromMap(CONTENT_WINDOW_COLOR), this->getMeasure("windowWidth"), this->getMeasure("contentWindowHeight"), this->getMeasure("windowX"), this->getMeasure("contentWindowY"), RoundedAngles::BOTTOM_ANGLES, WINDOW_SLOPE));

    return this;
}

AbstractPopupWindowView* AbstractPopupWindowView::destroyImages()
{
    AbstractView::destroyImages();

    this->setWindowHeader(nullptr);

    this->setHeaderText(nullptr);

    this->setContentWindow(nullptr);

    for(auto& ent : this->getButtons())
    {
        if(ent.second != nullptr)
        {
            delete ent.second;
        }
    }
    this->m_pButtons.clear();

    for(auto& ent : this->getButtonsContent())
    {
        if(ent.second != nullptr)
        {
            delete ent.second;
        }
    }
    this->m_pButtonsContent.clear();

    return this;
}

AbstractPopupWindowView* AbstractPopupWindowView::calculateMeasures()
{
    AbstractView::calculateMeasures();

    this->addMeasure("windowWidth", this->getMeasure("compensatedWidth") * 3 / 8);
    this->addMeasure("windowX", this->getMeasure("compensatedX") + (this->getMeasure("compensatedWidth") * 5 / 16));
    this->addMeasure("windowHeight", this->getMeasure("compensatedHeight") / 4);
    this->addMeasure("windowY", this->getMeasure("compensatedY") + (this->getMeasure("compensatedHeight") / 2) - (this->getMeasure("windowHeight") / 2));
    this->addMeasure("headerHeight", this->getMeasure("windowHeight") / 5);
    this->addMeasure("headerVerticalPadding", this->getMeasure("headerHeight") / 5);
    this->addMeasure("headerHorizontalPadding", this->getMeasure("headerHeight"));
    this->addMeasure("titleWidth", this->getMeasure("windowWidth") - (2 * this->getMeasure("headerHorizontalPadding")));
    this->addMeasure("titleHeight", this->getMeasure("headerHeight") - (2 * this->getMeasure("headerVerticalPadding")));
    this->addMeasure("titleX", this->getMeasure("windowX") + this->getMeasure("headerHorizontalPadding"));
    this->addMeasure("titleXMax", this->getMeasure("titleX") + this->getMeasure("titleWidth"));
    this->addMeasure("titleY", this->getMeasure("windowY") + this->getMeasure("headerVerticalPadding"));
    this->addMeasure("titleYMax", this->getMeasure("titleY") + this->getMeasure("titleHeight"));
    this->addMeasure("contentWindowHeight", this->getMeasure("windowHeight") - this->getMeasure("headerHeight"));
    this->addMeasure("contentWindowHorizontalPadding", this->getMeasure("windowWidth") * WINDOW_SLOPE / 100 / 2);
    this->addMeasure("contentWindowVerticalPadding", this->getMeasure("windowWidth") * WINDOW_SLOPE / 100 / 2);
    this->addMeasure("contentWindowY", this->getMeasure("windowY") + this->getMeasure("headerHeight"));
    this->addMeasure("bottomAreaHeight", this->getMeasure("contentWindowHeight") / 3);
    this->addMeasure("contentVerticalPadding", (this->getMeasure("contentWindowHeight") - this->getMeasure("bottomAreaHeight")) / 10);
    this->addMeasure("contentHeight", (this->getMeasure("contentWindowHeight") - this->getMeasure("bottomAreaHeight")) - (2 * this->getMeasure("contentVerticalPadding")));
    this->addMeasure("contentWidth", this->getMeasure("windowWidth") - (2 * this->getMeasure("contentWindowHorizontalPadding")));
    this->addMeasure("contentX", this->getMeasure("windowX") + this->getMeasure("contentWindowHorizontalPadding"));
    this->addMeasure("contentY", this->getMeasure("contentWindowY") + this->getMeasure("contentVerticalPadding"));
    this->addMeasure("contentXMax", this->getMeasure("contentX") + this->getMeasure("contentWidth"));
    this->addMeasure("contentYMax", this->getMeasure("contentY") + this->getMeasure("contentHeight"));
    this->addMeasure("bottomAreaVerticalPadding", this->getMeasure("bottomAreaHeight") / 10);
    this->addMeasure("bottomAreaY", this->getMeasure("contentWindowY") + this->getMeasure("contentHeight") + (2 * this->getMeasure("contentVerticalPadding")));
    this->addMeasure("buttonAreaWidth", this->getMeasure("windowWidth") - (2 * this->getMeasure("contentWindowHorizontalPadding")));
    this->addMeasure("buttonAreaHeight", this->getMeasure("bottomAreaHeight") - (2 * this->getMeasure("bottomAreaVerticalPadding")));
    this->addMeasure("buttonAreaX", this->getMeasure("windowX") + this->getMeasure("contentWindowHorizontalPadding"));
    this->addMeasure("buttonAreaY", this->getMeasure("bottomAreaY") + this->getMeasure("bottomAreaVerticalPadding"));
    this->addMeasure("buttonAreaXMax", this->getMeasure("buttonAreaX") + this->getMeasure("buttonAreaWidth"));
    this->addMeasure("buttonAreaYMax", this->getMeasure("buttonAreaY") + this->getMeasure("buttonAreaHeight"));
    this->addMeasure("interButtonSpacing", this->getMeasure("contentWindowHorizontalPadding") / 2);
    this->addMeasure("buttonVerticalPadding", this->getMeasure("buttonAreaHeight") * BUTTON_SLOPE / 100 / 2);
    this->addMeasure("buttonContentHeight", this->getMeasure("buttonAreaHeight") - (2 * this->getMeasure("buttonVerticalPadding")));
    this->addMeasure("buttonContentY", this->getMeasure("buttonAreaY") + this->getMeasure("buttonVerticalPadding"));
    this->addMeasure("buttonContentYMax", this->getMeasure("buttonContentY") + this->getMeasure("buttonContentHeight"));

    return this;
}

AbstractPopupWindowView* AbstractPopupWindowView::setWindowHeader(RoundedRectangle* windowHeader)
{
    if(this->m_pWindowHeader != nullptr)
    {
        delete this->m_pWindowHeader;
    }
    this->m_pWindowHeader = windowHeader;
    return this;
}

RoundedRectangle* AbstractPopupWindowView::getWindowHeader() const
{
    return this->m_pWindowHeader;
}

AbstractPopupWindowView* AbstractPopupWindowView::setHeaderText(Text* headerText)
{
    if(this->m_pHeaderText != nullptr)
    {
        delete this->m_pHeaderText;
    }
    this->m_pHeaderText = headerText;
    return this;
}

AbstractPopupWindowView* AbstractPopupWindowView::setHeaderText(string text)
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

Text* AbstractPopupWindowView::getHeaderText() const
{
    return this->m_pHeaderText;
}

AbstractPopupWindowView* AbstractPopupWindowView::setContentWindow(RoundedRectangle* contentWindow)
{
    if(this->m_pContentWindow != nullptr)
    {
        delete this->m_pContentWindow;
    }
    this->m_pContentWindow = contentWindow;
    return this;
}

RoundedRectangle* AbstractPopupWindowView::getContentWindow() const
{
    return this->m_pContentWindow;
}

AbstractPopupWindowView* AbstractPopupWindowView::createButtons(const map<int, string>& buttons)
{
    for(auto& ent : this->getButtons())
    {
        if(ent.second != nullptr)
        {
            delete ent.second;
        }
    }
    this->m_pButtons.clear();

    for(auto& ent : this->getButtonsContent())
    {
        if(ent.second != nullptr)
        {
            delete ent.second;
        }
    }
    this->m_pButtonsContent.clear();

    int numberButtons = buttons.size();

    if(numberButtons > 0)
    {
        int buttonWidth = (this->getMeasure("buttonAreaWidth") - ((numberButtons - 1) * this->getMeasure("interButtonSpacing"))) / numberButtons;
        int buttonHorizontalPadding = this->getMeasure("buttonVerticalPadding");
        int buttonContentWidth = buttonWidth - (2 * buttonHorizontalPadding);

        int fontSize = Fonts::ROBOTOLAST;

        for(const auto& ent : buttons)
        {
            int currentFont = Font::getBiggestFontToFit(FontFamily::ROBOTO, ent.second, buttonContentWidth, this->getMeasure("buttonContentHeight"));
            if(currentFont < fontSize)
            {
                fontSize = currentFont;
            }
        }

        int i = 0;
        for(const auto& ent : buttons)
        {
            int posX = this->getMeasure("buttonAreaX") + i * (buttonWidth + this->getMeasure("interButtonSpacing"));
            RoundedRectangle* buttonBackground = new RoundedRectangle(this->getView()->getRenderer(), Color::getColorFromMap(BUTTON_COLOR), buttonWidth, this->getMeasure("buttonAreaHeight"), posX, this->getMeasure("buttonAreaY"), RoundedAngles::ALL_ANGLES, BUTTON_SLOPE);
            this->addButton(ent.first, buttonBackground);

            Text* buttonContent = new Text(this->getView()->getRenderer(), ent.second, Font::getFontFromMap(FontFamily::ROBOTO, fontSize), Color::getColorFromMap(BUTTON_CONTENT_COLOR), 0, 0);
            if(buttonContent != nullptr)
            {
                buttonContent->center(posX + buttonHorizontalPadding, posX + buttonHorizontalPadding + buttonContentWidth, this->getMeasure("buttonContentY"), this->getMeasure("buttonContentYMax"));
                this->addButtonContent(ent.first, buttonContent);
            }
            ++i;
        }
    }

    return this;
}

AbstractPopupWindowView* AbstractPopupWindowView::addButton(const int& buttonNumber, RoundedRectangle* button)
{
    if(this->m_pButtons.count(buttonNumber) > 0)
    {
        if(this->m_pButtons.at(buttonNumber) != nullptr)
        {
            delete this->m_pButtons.at(buttonNumber);
        }
        this->m_pButtons.erase(buttonNumber);
    }
    this->m_pButtons.insert(pair<int, RoundedRectangle*>(buttonNumber, button));
    return this;
}

RoundedRectangle* AbstractPopupWindowView::getButton(const int& buttonNumber) const
{
    if(this->m_pButtons.count(buttonNumber) > 0)
    {
        return this->m_pButtons.at(buttonNumber);
    }
    return nullptr;
}

map<int, RoundedRectangle*> AbstractPopupWindowView::getButtons() const
{
    return this->m_pButtons;
}

AbstractPopupWindowView* AbstractPopupWindowView::addButtonContent(const int& buttonNumber, Text* buttonContent)
{
    if(this->m_pButtonsContent.count(buttonNumber) > 0)
    {
        if(this->m_pButtonsContent.at(buttonNumber) != nullptr)
        {
            delete this->m_pButtonsContent.at(buttonNumber);
        }
        this->m_pButtonsContent.erase(buttonNumber);
    }
    this->m_pButtonsContent.insert(pair<int, Text*>(buttonNumber, buttonContent));
    return this;
}

Text* AbstractPopupWindowView::getButtonContent(const int& buttonNumber) const
{
    if(this->m_pButtonsContent.count(buttonNumber) > 0)
    {
        return this->m_pButtonsContent.at(buttonNumber);
    }
    return nullptr;
}

map<int, Text*> AbstractPopupWindowView::getButtonsContent() const
{
    return this->m_pButtonsContent;
}

/*******************
 * PopupWindowView *
 *******************/

PopupWindowView::PopupWindowView(View* view)
: AbstractPopupWindowView(view, ViewsID::POPUP_WINDOW_VIEW)
{
    this->calculateMeasures();
}

PopupWindowView::~PopupWindowView()
{

}

PopupWindowView* PopupWindowView::render(const ViewData& viewData)
{
    AbstractPopupWindowView::render(viewData);

    if(this->getContent() != nullptr)
    {
        if(this->getContent()->getText() != viewData.popupWindowContent)
        {
            this->setContent(viewData.popupWindowContent);
        }
        this->getView()->addToRender(this->getContent());
    }

    return this;
}

PopupWindowView* PopupWindowView::loadImages()
{
    AbstractPopupWindowView::loadImages();
    this->setContent(new WrappedText(this->getView()->getRenderer(), "", FontFamily::ROBOTO, Color::getColorFromMap(CONTENT_COLOR), this->getMeasure("contentWidth"), this->getMeasure("contentHeight"), this->getMeasure("contentX"), this->getMeasure("contentY")));
    return this;
}

PopupWindowView* PopupWindowView::destroyImages()
{
    AbstractPopupWindowView::destroyImages();

    this->setContent(nullptr);

    return this;
}

PopupWindowView* PopupWindowView::calculateMeasures()
{
    AbstractPopupWindowView::calculateMeasures();

    return this;
}

PopupWindowView* PopupWindowView::setContent(WrappedText* content)
{
    if(this->m_pContent != nullptr)
    {
        delete this->m_pContent;
    }
    this->m_pContent = content;
    return this;
}

PopupWindowView* PopupWindowView::setContent(const std::string& content)
{
    if(this->getContent() != nullptr)
    {
        this->getContent()->setText(content);
        this->getContent()->center(this->getMeasure("contentX"), this->getMeasure("contentXMax"), this->getMeasure("contentY"), this->getMeasure("contentYMax"));
    }
    return this;
}

WrappedText* PopupWindowView::getContent() const
{
    return this->m_pContent;
}

/************************
 * InputPopupWindowView *
 ************************/

InputPopupWindowView::InputPopupWindowView(View* view)
: AbstractPopupWindowView(view, ViewsID::INPUT_POPUP_WINDOW_VIEW)
{
    this->calculateMeasures();
}

InputPopupWindowView::~InputPopupWindowView()
{

}

InputPopupWindowView* InputPopupWindowView::render(const ViewData& viewData)
{
    AbstractPopupWindowView::render(viewData);

    if(this->getHelpText()->getText() != viewData.inputPopupWindowTitle)
    {
        this->setHelpText(viewData.inputPopupWindowHelpText);
    }

    if(this->getHelpText() != nullptr)
    {
        this->getView()->addToRender(this->getHelpText());
    }

    return this;
}

InputPopupWindowView* InputPopupWindowView::loadImages()
{
    AbstractPopupWindowView::loadImages();
    this->setHelpText(new Text(this->getView()->getRenderer(), "", Font::getFontFromMap(FontFamily::ROBOTO, 12), Color::getColorFromMap(CONTENT_COLOR), this->getMeasure("helpTextX"), this->getMeasure("helpTextY")));
    return this;
}

InputPopupWindowView* InputPopupWindowView::destroyImages()
{
    AbstractPopupWindowView::destroyImages();
    this->setHelpText(nullptr);

    AbstractPopupWindowView::destroyImages();
    return this;
}

InputPopupWindowView* InputPopupWindowView::calculateMeasures()
{
    AbstractPopupWindowView::calculateMeasures();

    this->addMeasure("helpTextHeight", (this->getMeasure("contentHeight") - this->getMeasure("contentVerticalPadding")) / 2);
    this->addMeasure("helpTextWidth", this->getMeasure("contentWidth"));
    this->addMeasure("helpTextX", this->getMeasure("contentX"));
    this->addMeasure("helpTextXMax", this->getMeasure("helpTextX") + this->getMeasure("helpTextWidth"));
    this->addMeasure("helpTextY", this->getMeasure("contentY"));
    this->addMeasure("helpTextYMax", this->getMeasure("helpTextY") + this->getMeasure("helpTextHeight"));



    return this;
}

InputPopupWindowView* InputPopupWindowView::setHelpText(Text* helpText)
{
    if(this->m_pHelpText != nullptr)
    {
        delete this->m_pHelpText;
    }
    this->m_pHelpText = helpText;
    return this;
}

InputPopupWindowView* InputPopupWindowView::setHelpText(const std::string& helpText)
{
    if(this->m_pHelpText != nullptr)
    {
        int font = Font::getBiggestFontToFit(FontFamily::ROBOTO, helpText, this->getMeasure("helpTextWidth"), this->getMeasure("helpTextHeight"));
        this->m_pHelpText->setText(helpText);
        this->m_pHelpText->setFont(Font::getFontFromMap(FontFamily::ROBOTO, font));
        this->m_pHelpText->reload();
        this->m_pHelpText->center(this->getMeasure("helpTextX"), this->getMeasure("helpTextXMax"), this->getMeasure("helpTextY"), this->getMeasure("helpTextYMax"));
    }
    return this;
}

Text* InputPopupWindowView::getHelpText() const
{
    return this->m_pHelpText;
}

InputPopupWindowView* InputPopupWindowView::setInputBox(RoundedRectangle* inputBox)
{
    if(this->m_pInputBox != nullptr)
    {
        delete this->m_pInputBox;
    }
    this->m_pInputBox = inputBox;
    return this;
}

RoundedRectangle* InputPopupWindowView::getInputBox() const
{
    return this->m_pInputBox;
}

InputPopupWindowView* InputPopupWindowView::setInputContent(Text* inputContent)
{
    if(this->m_pInputText != nullptr)
    {
        delete this->m_pInputText;
    }
    this->m_pInputText = inputContent;
    return this;
}

InputPopupWindowView* InputPopupWindowView::setInputContent(const std::string& inputContent)
{
    if(this->m_pInputText != nullptr)
    {
        int font = Font::getBiggestFontToFit(FontFamily::ROBOTO, inputContent, this->getMeasure("inputContentWidth"), this->getMeasure("inputContentHeight"));
        this->m_pInputText->setText(inputContent);
        this->m_pInputText->setFont(Font::getFontFromMap(FontFamily::ROBOTO, font));
        this->m_pInputText->reload();
        this->m_pInputText->center(this->getMeasure("inputContentX"), this->getMeasure("inputContentXMax"), this->getMeasure("inputContentY"), this->getMeasure("inputContentYMax"));
    }
    return this;
}

Text* InputPopupWindowView::getInputContent() const
{
    return this->m_pInputText;
}
