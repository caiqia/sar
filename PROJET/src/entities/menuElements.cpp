#include "menuElements.hpp"

#include "../views/font.hpp"

#include <iostream>

using namespace std;

const NostalColors HOVERED_COLOR = NostalColors::WHITE;

const string RIGHT_ARROW = "/nostal/IMG/RESSOURCES/MENU/RIGHT_ARROW.png";
const string RIGHT_ARROW_WHITE = "/nostal/IMG/RESSOURCES/MENU/RIGHT_ARROW_W.png";

const string WIFI_BLUE_25 = "/nostal/IMG/RESSOURCES/WIFI/WIFI_BLUE_25.png";
const string WIFI_BLUE_50 = "/nostal/IMG/RESSOURCES/WIFI/WIFI_BLUE_50.png";
const string WIFI_BLUE_75 = "/nostal/IMG/RESSOURCES/WIFI/WIFI_BLUE_75.png";
const string WIFI_BLUE_100 = "/nostal/IMG/RESSOURCES/WIFI/WIFI_BLUE_100.png";
const string WIFI_LOCKED_BLUE_25 = "/nostal/IMG/RESSOURCES/WIFI/WIFI_LOCKED_BLUE_25.png";
const string WIFI_LOCKED_BLUE_50 = "/nostal/IMG/RESSOURCES/WIFI/WIFI_LOCKED_BLUE_50.png";
const string WIFI_LOCKED_BLUE_75 = "/nostal/IMG/RESSOURCES/WIFI/WIFI_LOCKED_BLUE_75.png";
const string WIFI_LOCKED_BLUE_100 = "/nostal/IMG/RESSOURCES/WIFI/WIFI_LOCKED_BLUE_100.png";
const string WIFI_WHITE_25 = "/nostal/IMG/RESSOURCES/WIFI/WIFI_WHITE_25.png";
const string WIFI_WHITE_50 = "/nostal/IMG/RESSOURCES/WIFI/WIFI_WHITE_50.png";
const string WIFI_WHITE_75 = "/nostal/IMG/RESSOURCES/WIFI/WIFI_WHITE_75.png";
const string WIFI_WHITE_100 = "/nostal/IMG/RESSOURCES/WIFI/WIFI_WHITE_100.png";
const string WIFI_LOCKED_WHITE_25 = "/nostal/IMG/RESSOURCES/WIFI/WIFI_LOCKED_WHITE_25.png";
const string WIFI_LOCKED_WHITE_50 = "/nostal/IMG/RESSOURCES/WIFI/WIFI_LOCKED_WHITE_50.png";
const string WIFI_LOCKED_WHITE_75 = "/nostal/IMG/RESSOURCES/WIFI/WIFI_LOCKED_WHITE_75.png";
const string WIFI_LOCKED_WHITE_100 = "/nostal/IMG/RESSOURCES/WIFI/WIFI_LOCKED_WHITE_100.png";


/***************
* Menu element *
****************/

MenuElement::MenuElement(SDL_Renderer* renderer, Color* color, const int& width, const int& height, const int& posX, const int& posY, const MenuEntityType& entityType)
: Surface (renderer, color, width, height, posX, posY)
{
    this->initMeasures();
    this->setOriginalColor(color);

    this->setEntityType(entityType);

    this->setRightArrow(new Image(renderer, RIGHT_ARROW));
    this->getRightArrow()->setSize(this->getMeasure("arrowWidth"), this->getMeasure("arrowHeight"), ResizeMode::FIT);
    this->getRightArrow()->center(this->getMeasure("rightArrowX"), this->getMeasure("rightArrowXMax"), this->getMeasure("arrowY"), this->getMeasure("arrowYMax"));

    this->setRightArrowWhite(new Image(renderer, RIGHT_ARROW_WHITE));
    this->getRightArrowWhite()->setSize(this->getMeasure("arrowWidth"), this->getMeasure("arrowHeight"), ResizeMode::FIT);
    this->getRightArrowWhite()->center(this->getMeasure("rightArrowX"), this->getMeasure("rightArrowXMax"), this->getMeasure("arrowY"), this->getMeasure("arrowYMax"));
}

MenuElement::~MenuElement()
{
    this->setRightArrow(nullptr);
    this->setRightArrowWhite(nullptr);
}

MenuElement* MenuElement::render()
{
    Surface::render();

    if(this->getRightArrow() != nullptr && !this->isHovered())
    {
        this->getRightArrow()->render();
    }

    else if(this->getRightArrowWhite() != nullptr && this->isHovered())
    {
        this->getRightArrowWhite()->render();
    }

    return this;
}

MenuElement* MenuElement::setHovered()
{
    this->m_hovered = true;
    this->setColor(Color::getColorFromMap(HOVERED_COLOR));
    return this;
}

MenuElement* MenuElement::unsetHovered()
{
    this->m_hovered = false;
    this->setColor(this->getOriginalColor());
    return this;
}


bool MenuElement::isHovered() const
{
    return this->m_hovered;
}

MenuElement* MenuElement::initMeasures()
{
    this->setMeasure("paddingHorz", this->getWidth() / 12);
    this->setMeasure("paddingVert", this->getHeight() / 4);

    this->setMeasure("arrowWidth", this->getWidth() / 12);
    this->setMeasure("arrowHeight", this->getHeight() - (2 * this->getMeasure("paddingVert")));

    this->setMeasure("arrowY", this->getPosY() + this->getMeasure("paddingVert"));
    this->setMeasure("arrowYMax", this->getMeasure("arrowY") + this->getMeasure("arrowHeight"));

    this->setMeasure("leftArrowX", this->getPosX() + (this->getWidth() * 11 / 24));
    this->setMeasure("leftArrowXMax", this->getMeasure("leftArrowX") + this->getMeasure("arrowWidth"));

    this->setMeasure("rightArrowX", this->getPosX() + (this->getWidth() * 21 / 24));
    this->setMeasure("rightArrowXMax", this->getMeasure("rightArrowX") + this->getMeasure("arrowWidth"));
    return this;
}

MenuElement* MenuElement::setMeasure(const string& measure, const int& value)
{
    if(this->m_measures.count(measure) > 0)
    {
        this->m_measures.erase(measure);
    }
    this->m_measures.insert(pair<string, int> (measure, value));
    return this;
}

int MenuElement::getMeasure(const string& measure) const
{
    if(this->m_measures.count(measure) > 0)
    {
        return this->m_measures.at(measure);
    }
    else
    {
        return 0; // error
    }
}

MenuElement* MenuElement::setPosition(const int& posX, const int& posY)
{
    Surface::setPosition(posX, posY);
    this->initMeasures();

    if(this->getRightArrow() != nullptr)
    {
        this->getRightArrow()->center(this->getMeasure("rightArrowX"), this->getMeasure("rightArrowXMax"), this->getMeasure("arrowY"), this->getMeasure("arrowYMax"));
    }

    if(this->getRightArrowWhite() != nullptr)
    {
        this->getRightArrowWhite()->center(this->getMeasure("rightArrowX"), this->getMeasure("rightArrowXMax"), this->getMeasure("arrowY"), this->getMeasure("arrowYMax"));
    }

    return this;
}

MenuElement* MenuElement::setOriginalColor(Color* color)
{
    this->m_originalColor = color;
    return this;
}

Color* MenuElement::getOriginalColor() const
{
    return this->m_originalColor;
}

MenuElement* MenuElement::setRightArrow(Image* rightArrowImage)
{
    if(this->m_pRightArrow != nullptr)
    {
        delete this->m_pRightArrow;
    }

    this->m_pRightArrow = rightArrowImage;
    return this;
}

Image* MenuElement::getRightArrow() const
{
    return this->m_pRightArrow;
}

MenuElement* MenuElement::setRightArrowWhite(Image* rightArrowImage)
{
    if(this->m_pRightArrowWhite != nullptr)
    {
        delete this->m_pRightArrowWhite;
    }
    this->m_pRightArrowWhite = rightArrowImage;
    return this;
}

Image* MenuElement::getRightArrowWhite() const
{
    return this->m_pRightArrowWhite;
}

MenuElement* MenuElement::setEntityType(const MenuEntityType& entityType)
{
    this->m_entityType = entityType;
}

MenuEntityType MenuElement::getEntityType() const
{
    return this->m_entityType;
}

/***************
 * Text Button *
 ***************/

 TextButton::TextButton(SDL_Renderer* renderer, Color* color, const int& width, const int& height, const int& posX, const int& posY, const std::string& text)
 : MenuElement(renderer, color, width, height, posX, posY, MenuEntityType::TYPE_BUTTON)
 {
     this->initMeasures();
     this->setText(text);

     int font = Font::getBiggestFontToFit(FontFamily::ROBOTO, this->getText(), this->getMeasure("txtMaxWidth"), this->getMeasure("txtMaxHeight"));
     Text* textObjet = new Text(renderer, this->getText(), Font::getFontFromMap(FontFamily::ROBOTO, font), color, this->getMeasure("txtPosX"), this->getMeasure("txtPosY"));
     this->setText(textObjet);
 }

 TextButton::~TextButton()
 {
     this->setText(nullptr);
 }

 TextButton* TextButton::render()
 {
     MenuElement::render();

     if(this->getTextObject() != nullptr)
     {
         this->getTextObject()->render();
     }

     return this;
 }

TextButton* TextButton::initMeasures()
{
    MenuElement::initMeasures();
    this->setMeasure("txtMaxWidth", this->getWidth() * 9 / 12);
    this->setMeasure("txtMaxHeight", this->getHeight() - (2 * this->getMeasure("paddingVert")));
    this->setMeasure("txtPosX", this->getPosX() + this->getMeasure("paddingHorz"));
    this->setMeasure("txtPosY", this->getPosY() + this->getMeasure("paddingVert"));
    this->setMeasure("txtPosXMax", this->getMeasure("txtPosX") + this->getMeasure("txtMaxWidth"));
    this->setMeasure("txtPosYMax", this->getMeasure("txtPosY") + this->getMeasure("txtMaxHeight"));

    return this;
}

 TextButton* TextButton::setText(const std::string& text)
 {
     this->m_text = text;

     if(this->m_pText != nullptr)
     {
         int font = Font::getBiggestFontToFit(FontFamily::ROBOTO, this->getText(), this->getMeasure("txtMaxWidth"), this->getMeasure("txtMaxHeight"));
         this->m_pText->setText(this->getText());
         this->m_pText->setFont(Font::getFontFromMap(FontFamily::ROBOTO, font));
         this->m_pText->reload();
     }
     return this;
 }

 std::string TextButton::getText() const
 {
     return this->m_text;
 }

TextButton* TextButton::setText(Text* textObjectPointer)
{
 if(this->m_pText != nullptr)
 {
     delete this->m_pText;
 }

 this->m_pText = textObjectPointer;
 return this;
}

TextButton* TextButton::setColor(Color* color)
{
    MenuElement::setColor(color);
    this->getTextObject()->setColor(color);
    this->getTextObject()->reload();
    return this;
}

Text* TextButton::getTextObject() const
{
 return this->m_pText;
}

TextButton* TextButton::setPosition(const int& posX, const int& posY)
{
    MenuElement::setPosition(posX, posY);

    if(this->getTextObject() != nullptr)
    {
        this->getTextObject()->centerVert(this->getMeasure("txtPosY"), this->getMeasure("txtPosYMax"));
    }

    return this;
}

/**************
 * WifiButton *
 **************/

WifiButton::WifiButton(SDL_Renderer* renderer, Color* color, const int& width, const int& height, const int& posX, const int& posY, const string& text, const WifiSignalPower& wifiSignalPower, const bool hasAuth)
: MenuElement(renderer, color, width, height, posX, posY, MenuEntityType::TYPE_WIFI_BUTTON)
{
    this->initMeasures();
    this->setText(text);
    this->setSignalPower(wifiSignalPower);
    this->hasAuth(hasAuth);

    int font = Font::getBiggestFontToFit(FontFamily::ROBOTO, this->getText(), this->getMeasure("textWidth"), this->getMeasure("textHeight"));
    Text* textObjet = new Text(renderer, this->getText(), Font::getFontFromMap(FontFamily::ROBOTO, font), color, this->getMeasure("textX"), this->getMeasure("textY"));
    this->setText(textObjet);

    this->loadWifiImage();

    return;

}

WifiButton::~WifiButton()
{
    this->setText(nullptr);

    this->setWifiImage(nullptr);

    this->setHoveredWifiImage(nullptr);
}

WifiButton* WifiButton::loadWifiImage()
{
    string imageToLoad, hoverImageToLoad;
    if(this->hasAuth())
    {
        switch (this->getSignalPower())
        {
            case WifiSignalPower::VERY_WEAK_SIGNAL:
            {
                imageToLoad = WIFI_LOCKED_BLUE_25;
                hoverImageToLoad = WIFI_LOCKED_WHITE_25;
                break;
            }

            case WifiSignalPower::WEAK_SIGNAL:
            {
                imageToLoad = WIFI_LOCKED_BLUE_25;
                hoverImageToLoad = WIFI_LOCKED_WHITE_25;
                break;
            }

            case WifiSignalPower::MEDIUM_SIGNAL:
            {
                imageToLoad = WIFI_LOCKED_BLUE_75;
                hoverImageToLoad = WIFI_LOCKED_WHITE_75;
                break;
            }

            case WifiSignalPower::STRONG_SIGNAL:
            {
                imageToLoad = WIFI_LOCKED_BLUE_100;
                hoverImageToLoad = WIFI_LOCKED_WHITE_100;
                break;
            }
        }
    }

    else
    {
        switch (this->getSignalPower())
        {
            case WifiSignalPower::VERY_WEAK_SIGNAL:
            {
                imageToLoad = WIFI_BLUE_25;
                hoverImageToLoad = WIFI_WHITE_25;
                break;
            }

            case WifiSignalPower::WEAK_SIGNAL:
            {
                imageToLoad = WIFI_BLUE_25;
                hoverImageToLoad = WIFI_WHITE_25;
                break;
            }

            case WifiSignalPower::MEDIUM_SIGNAL:
            {
                imageToLoad = WIFI_BLUE_75;
                hoverImageToLoad = WIFI_WHITE_75;
                break;
            }

            case WifiSignalPower::STRONG_SIGNAL:
            {
                imageToLoad = WIFI_BLUE_100;
                hoverImageToLoad = WIFI_WHITE_100;
                break;
            }
        }
    }

    Image* wifiImage = nullptr;
    Image* hoveredWifiImage = nullptr;

    if(imageToLoad != "")
    {
        wifiImage = new Image(this->getRenderer(), imageToLoad);
    }

    if(hoverImageToLoad != "")
    {
        hoveredWifiImage = new Image(this->getRenderer(), hoverImageToLoad);
    }

    if(wifiImage != nullptr)
    {
        wifiImage->setSize(this->getMeasure("wifiInfoWidth"), this->getMeasure("wifiInfoHeight"), ResizeMode::FIT);
        wifiImage->center(this->getMeasure("wifiInfoX"), this->getMeasure("wifiInfoXMax"), this->getMeasure("wifiInfoY"), this->getMeasure("wifiInfoYMax"));
        this->setWifiImage(wifiImage);
    }

    if(hoveredWifiImage != nullptr)
    {
        hoveredWifiImage->setSize(this->getMeasure("wifiInfoWidth"), this->getMeasure("wifiInfoHeight"), ResizeMode::FIT);
        hoveredWifiImage->center(this->getMeasure("wifiInfoX"), this->getMeasure("wifiInfoXMax"), this->getMeasure("wifiInfoY"), this->getMeasure("wifiInfoYMax"));
        this->setHoveredWifiImage(hoveredWifiImage);
    }

    return this;
}

WifiButton* WifiButton::render()
{
    MenuElement::render();
    if(this->getTextObject() != nullptr)
    {
        this->getTextObject()->render();
    }

    if(!this->isHovered() && this->getWifiImage() != nullptr)
    {
        this->getWifiImage()->render();
    }

    else if(this->isHovered() && this->getHoveredWifiImage() != nullptr)
    {
        this->getHoveredWifiImage()->render();
    }
}

WifiButton* WifiButton::initMeasures()
{
    MenuElement::initMeasures();

    this->setMeasure("wifiInfoHeight", this->getHeight() - (2 * this->getMeasure("paddingVert")));
    this->setMeasure("wifiInfoWidth", this->getMeasure("wifiInfoHeight"));
    this->setMeasure("wifiInfoX", this->getPosX() + this->getMeasure("paddingHorz"));
    this->setMeasure("wifiInfoY", this->getPosY() + this->getMeasure("paddingVert"));
    this->setMeasure("wifiInfoXMax", this->getMeasure("wifiInfoX")+ this->getMeasure("wifiInfoWidth"));
    this->setMeasure("wifiInfoYMax", this->getMeasure("wifiInfoY")+ this->getMeasure("wifiInfoHeight"));

    this->setMeasure("textWidth", this->getWidth() - this->getMeasure("wifiInfoWidth") - this->getMeasure("arrowWidth") - (this->getMeasure("paddingHorz") * 4));
    this->setMeasure("textHeight", this->getHeight() - (2 * this->getMeasure("paddingVert")));
    this->setMeasure("textX", this->getMeasure("wifiInfoX") + this->getMeasure("wifiInfoWidth") + this->getMeasure("paddingHorz"));
    this->setMeasure("textXMax", this->getMeasure("textX") + this->getMeasure("textWidth"));
    this->setMeasure("textY", this->getPosY() + this->getMeasure("paddingVert"));
    this->setMeasure("textYMax", this->getMeasure("textY") + this->getMeasure("textHeight"));
    return this;
}

WifiButton* WifiButton::setColor(Color* color)
{
    MenuElement::setColor(color);

    this->getTextObject()->setColor(color);
    this->getTextObject()->reload();

    return this;
}

WifiButton* WifiButton::setPosition(const int& posX, const int& posY)
{
    MenuElement::setPosition(posX, posY);

    if(this->getTextObject() != nullptr)
    {
        this->getTextObject()->centerVert(this->getMeasure("textY"), this->getMeasure("textYMax"));
    }

    if(this->getWifiImage() != nullptr)
    {
        this->getWifiImage()->setSize(this->getMeasure("wifiInfoWidth"), this->getMeasure("wifiInfoHeight"), ResizeMode::FIT);
        this->getWifiImage()->center(this->getMeasure("wifiInfoX"), this->getMeasure("wifiInfoXMax"), this->getMeasure("wifiInfoY"), this->getMeasure("wifiInfoYMax"));
    }

    if(this->getHoveredWifiImage() != nullptr)
    {
        this->getHoveredWifiImage()->setSize(this->getMeasure("wifiInfoWidth"), this->getMeasure("wifiInfoHeight"), ResizeMode::FIT);
        this->getHoveredWifiImage()->center(this->getMeasure("wifiInfoX"), this->getMeasure("wifiInfoXMax"), this->getMeasure("wifiInfoY"), this->getMeasure("wifiInfoYMax"));
    }
}

bool WifiButton::hasAuth() const
{
    return this->m_hasAuth;
}

WifiButton* WifiButton::hasAuth(const bool hasAuth)
{
    this->m_hasAuth = hasAuth;
    return this;
}

WifiSignalPower WifiButton::getSignalPower() const
{
    return this->m_signalPower;
}

WifiButton* WifiButton::setSignalPower(const WifiSignalPower& signalPower)
{
    this->m_signalPower = signalPower;
    return this;
}

WifiButton* WifiButton::setText(const string& text)
{
    this->m_text = text;

    if(this->m_pText != nullptr)
    {
        int font = Font::getBiggestFontToFit(FontFamily::ROBOTO, this->getText(), this->getMeasure("textWidth"), this->getMeasure("textHeight"));
        this->m_pText->setText(this->getText());
        this->m_pText->setFont(Font::getFontFromMap(FontFamily::ROBOTO, font));
        this->m_pText->reload();
        this->m_pText->centerVert(this->getMeasure("textY"), this->getMeasure("textYMax"));
    }

    return this;
}

string WifiButton::getText() const
{
    return this->m_text;
}



WifiButton* WifiButton::setText(Text* textObjectPointer)
{
    if(this->m_pText != nullptr)
    {
        delete this->m_pText;
    }

    this->m_pText = textObjectPointer;

    return this;
}

Text* WifiButton::getTextObject() const
{
    return this->m_pText;
}

WifiButton* WifiButton::setWifiImage(Image* wifiImage)
{
    if(this->m_pWifiImage != nullptr)
    {
        delete this->m_pWifiImage;
    }

    this->m_pWifiImage = wifiImage;

    return this;
}

Image* WifiButton::getWifiImage() const
{
    return this->m_pWifiImage;
}

WifiButton* WifiButton::setHoveredWifiImage(Image* hoveredWifiImage)
{
    if(this->m_pHoveredWifiImage != nullptr)
    {
        delete this->m_pHoveredWifiImage;
    }

    this->m_pHoveredWifiImage = hoveredWifiImage;

    return this;
}

Image* WifiButton::getHoveredWifiImage() const
{
    return this->m_pHoveredWifiImage;
}

 /***********************
  * LabelledMenuElement *
  ***********************/

LabelledMenuElement::LabelledMenuElement(SDL_Renderer* render, Color* color, int width, int height, int posX, int posY, std::string label, const MenuEntityType& entityType, LabelPosition labelPosition)
: MenuElement(render, color, width, height, posX, posY, entityType)
{
    this->setLabel(label, labelPosition);

    this->initMeasures();

    int font = Font::getBiggestFontToFit(FontFamily::ROBOTO, this->getLabelText(), this->getMeasure("labelAreaWidth"), this->getMeasure("labelAreaHeight"));
    Text* labelTxt = new Text(this->getRenderer(), this->getLabelText(), Font::getFontFromMap(FontFamily::ROBOTO, font), color, this->getMeasure("labelAreaX"), this->getMeasure("labelAreaY"));
    this->setLabelTextObject(labelTxt);
}

LabelledMenuElement::~LabelledMenuElement()
{
    if(this->m_pLabel != nullptr)
    {
        delete this->m_pLabel;
    }
}

LabelledMenuElement* LabelledMenuElement::render()
{
    MenuElement::render();

    if(this->m_pLabel != nullptr)
    {
        this->m_pLabel->render();
    }

    return this;
}

 LabelledMenuElement* LabelledMenuElement::setColor(Color* color)
 {
     MenuElement::setColor(color);
     this->getLabelTextObject()->setColor(color);
     this->getLabelTextObject()->reload();
     return this;
 }

LabelledMenuElement* LabelledMenuElement::setLabel(std::string labelText, LabelPosition labelPosition)
{
    this->m_labelPosition = labelPosition;
    this->initMeasures();

    this->m_labelText = labelText;
    if(this->m_pLabel != nullptr)
    {
        int font = Font::getBiggestFontToFit(FontFamily::ROBOTO, this->getLabelText(), this->getMeasure("labelAreaWidth"), this->getMeasure("labelAreaHeight"));
        this->m_pLabel->setText(this->getLabelText());
        this->m_pLabel->setFont(Font::getFontFromMap(FontFamily::ROBOTO, font));
        this->m_pLabel->reload();
    }

    return this;
}

std::string LabelledMenuElement::getLabelText()
{
    return this->m_labelText;
}

LabelPosition LabelledMenuElement::getLabelPosition()
{
    return this->m_labelPosition;
}

LabelledMenuElement* LabelledMenuElement::setLabelTextObject(Text* labelTextObject)
{
    if(this->m_pLabel != nullptr)
    {
        delete this->m_pLabel;
    }

    this->m_pLabel = labelTextObject;
    return this;
}

Text* LabelledMenuElement::getLabelTextObject()
{
    return this->m_pLabel;
}

LabelledMenuElement* LabelledMenuElement::initMeasures()
{
    MenuElement::initMeasures();

    if(this->getLabelPosition() == LabelPosition::SIDE)
    {
        this->setMeasure("labelAreaWidth", this->getWidth() * 7 / 24);
        this->setMeasure("labelAreaHeight", this->getHeight()- (2 * this->getMeasure("paddingVert")));

        this->setMeasure("labelAreaX", this->getPosX() + this->getMeasure("paddingHorz"));
        this->setMeasure("labelAreaY", this->getPosY() + this->getMeasure("paddingVert"));
        this->setMeasure("labelAreaXMax", this->getMeasure("labelAreaX") + this->getMeasure("labelAreaWidth"));
        this->setMeasure("labelAreaYMax", this->getMeasure("labelAreaY") + this->getMeasure("labelAreaHeight"));

        this->setMeasure("contentWidth", this->getWidth() / 3);
        this->setMeasure("contentHeight", this->getHeight()- (2 * this->getMeasure("paddingVert")));
        this->setMeasure("contentX", this->getPosX() + this->getWidth() / 2);
        this->setMeasure("contentXMax", this->getMeasure("contentX") + this->getMeasure("contentWidth"));
        this->setMeasure("contentY", this->getPosY() + this->getMeasure("paddingVert"));
        this->setMeasure("contentYMax", this->getMeasure("contentY") + this->getMeasure("contentHeight"));
    }

    return this;
}

LabelledMenuElement* LabelledMenuElement::setPosition(const int& posX, const int& posY)
{
    MenuElement::setPosition(posX, posY);

    if(this->getLabelTextObject() != nullptr)
    {
        this->getLabelTextObject()->centerVert(this->getMeasure("labelAreaY"), this->getMeasure("labelAreaYMax"));
    }
    return this;
}

/***********
 * TextBox *
 ***********/

TextBox::TextBox(SDL_Renderer* renderer, Color* color, int width, int height, int posX, int posY, string label, string content, bool isPassword, LabelPosition labelPosition)
 : LabelledMenuElement(renderer, color, width, height, posX, posY, label, MenuEntityType::TYPE_TEXT_BOX, labelPosition)
{
    this->setContent(content);
    this->isPassword(isPassword);

    this->initMeasures();

    if(isPassword)
    {
        content = this->passwordize(content);
    }

    int font = Font::getBiggestFontToFit(FontFamily::ROBOTO, content, this->getMeasure("contentWidth"), this->getMeasure("contentHeight"));
    Text* contentTxt = new Text(this->getRenderer(), content, Font::getFontFromMap(FontFamily::ROBOTO, font), color, this->getMeasure("contentX"), this->getMeasure("contentY"));
    this->setTextBoxContent(contentTxt);

}

TextBox::~TextBox()
{

    if(this->m_pTextBoxContent != nullptr)
    {
        delete this->m_pTextBoxContent;
    }
}

TextBox* TextBox::render()
{
    LabelledMenuElement::render();

    if(this->m_pTextBoxContent != nullptr)
    {
        this->m_pTextBoxContent->render();
    }
    return this;
}

TextBox* TextBox::setColor(Color* color)
{
    LabelledMenuElement::setColor(color);

    if(this->getTextBoxContent() != nullptr)
    {
        this->getTextBoxContent()->setColor(color);
        this->getTextBoxContent()->reload();
    }
}

TextBox* TextBox::setContent(std::string content)
{
    this->m_content = content;

    if(this->m_pTextBoxContent != nullptr)
    {
        if(this->isPassword())
        {
            content = this->passwordize(content);
        }

        int font = Font::getBiggestFontToFit(FontFamily::ROBOTO, content, this->getMeasure("contentWidth"), this->getMeasure("contentHeight"));
        this->m_pTextBoxContent->setText(content);
        this->m_pTextBoxContent->setFont(Font::getFontFromMap(FontFamily::ROBOTO, font));
        this->m_pTextBoxContent->reload();
    }

    return this;
}

std::string TextBox::getContent()
{
    return this->m_content;
}

TextBox* TextBox::isPassword(bool isPassword)
{
    this->m_isPassword = isPassword;
    this->setContent(this->getContent());
    return this;
}

bool TextBox::isPassword()
{
    return this->m_isPassword;
}

TextBox* TextBox::setTextBoxContent(Text* textBoxContent)
{
    if(this->m_pTextBoxContent != nullptr)
    {
        delete this->m_pTextBoxContent;
    }

    this->m_pTextBoxContent = textBoxContent;
    return this;
}

Text* TextBox::getTextBoxContent()
{
    return this->m_pTextBoxContent;
}

TextBox* TextBox::initMeasures()
{
    LabelledMenuElement::initMeasures();
    return this;
}

string TextBox::passwordize(string original)
{
    string passwordized;

    for(auto const & ent : original)
    {
        passwordized += "*";
    }

    return passwordized;
}

TextBox* TextBox::setPosition(const int& posX, const int& posY)
{
    LabelledMenuElement::setPosition(posX, posY);
    if(this->getTextBoxContent() != nullptr)
    {
        this->getTextBoxContent()->centerVert(this->getMeasure("contentY"), this->getMeasure("contentYMax"));
    }
    return this;
}

/******************
 * MultipleChoice *
 ******************/

MultipleChoice::MultipleChoice(SDL_Renderer* renderer, Color* color, int width, int height, int posX, int posY, std::string label, std::string currentChoice, bool canGoLeft, bool canGoRight, LabelPosition labelPosition)
 : LabelledMenuElement(renderer, color, width, height, posX, posY, label, MenuEntityType::TYPE_MULTIPLE_CHOICE, labelPosition)
{

}

MultipleChoice::~MultipleChoice()
{

}

MultipleChoice* MultipleChoice::render()
{
    LabelledMenuElement::render();
    return this;
}

MultipleChoice* MultipleChoice::setCurrentChoice(std::string currentChoice)
{
    this->m_currentChoice = currentChoice;
}

std::string MultipleChoice::getCurrentChoice()
{
    return this->m_currentChoice;
}

MultipleChoice* MultipleChoice::setAvailableDirections(bool left, bool right)
{
    this->m_canGoLeft = left;
    this->m_canGoRight = right;

    return this;
}

bool MultipleChoice::canGoLeft()
{
    return this->m_canGoLeft;
}

bool MultipleChoice::canGoRight()
{
    return this->m_canGoRight;
}

MultipleChoice* MultipleChoice::setLeftArrow(Image* leftArrowImage)
{
    if(this->m_pLeftArrow != nullptr)
    {
        delete this->m_pLeftArrow;
    }

    this->m_pLeftArrow = leftArrowImage;

    return this;
}

Image* MultipleChoice::getLeftArrow()
{
    return this->m_pLeftArrow;
}

MultipleChoice* MultipleChoice::setRightArrow(Image* rightArrowImage)
{
    if(this->m_pRightArrow != nullptr)
    {
        delete this->m_pRightArrow;
    }

    this->m_pRightArrow = rightArrowImage;
    return this;
}

Image* MultipleChoice::getRightArrow()
{
    return this->m_pRightArrow;
}

MultipleChoice* MultipleChoice::setChoiceText(Text* choiceText)
{
    if(this->m_pChoiceText != nullptr)
    {
        delete this->m_pChoiceText;
    }

    this->m_pChoiceText = choiceText;
    return this;
}

Text* MultipleChoice::getChoiceText()
{
    return this->m_pChoiceText;
}

MultipleChoice* MultipleChoice::initMeasures()
{
    LabelledMenuElement::initMeasures();
}
