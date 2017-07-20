#include "menuView.hpp"
#include "view.hpp"

#include <iostream>

using namespace std;

const int MENU_ELEMENTS_MAX = 5;

const int MENU_BACKGROUND_SLOPE = 20;

const NostalColors MENU_HEADER_COLOR = NostalColors::LIGHT_BLUE;
const NostalColors MENU_ELEMENTS_COLOR = NostalColors::LIGHT_BLUE;
const NostalColors MENU_HEADER_TEXT_COLOR = NostalColors::WHITE;
const NostalColors MENU_BACKGROUND_COLOR = NostalColors::WHITE;
const NostalColors MENU_SELECTOR_COLOR = NostalColors::LIGHT_BLUE;

const string MENU_HEADER_TEXT_CONTENT = "Menu";

const string UPPER_ARROW_FILE = "/nostal/IMG/RESSOURCES/MENU/UP_ARROW.png";
const string LOWER_ARROW_FILE = "/nostal/IMG/RESSOURCES/MENU/DOWN_ARROW.png";

MenuView::MenuView(View* view)
: AbstractView(view, ViewsID::MENU_VIEW)
{
}

MenuView::~MenuView()
{
    this->destroyImages();
}

MenuView* MenuView::render(const ViewData& viewData)
{
    AbstractView::render(viewData);

    if(this->getBackground() != nullptr)
    {
        this->getView()->addToRender(this->getBackground());
    }

    if(this->getHeader() != nullptr)
    {
        this->getView()->addToRender(this->getHeader());
    }

    if(this->getHeaderText() != nullptr)
    {
        if(viewData.menuTitle != this->getHeaderText()->getText())
        {
            int fontSize = Font::getBiggestFontToFit(FontFamily::ROBOTO, viewData.menuTitle, this->getMeasure("menuHeaderTextWidth"), this->getMeasure("menuHeaderTextHeight"));
            this->getHeaderText()->setText(viewData.menuTitle);
            this->getHeaderText()->setFont(Font::getFontFromMap(FontFamily::ROBOTO, fontSize));
            this->getHeaderText()->reload();
            this->getHeaderText()->center(this->getMeasure("menuHeaderTextX"), this->getMeasure("menuHeaderTextXMax"), this->getMeasure("menuHeaderTextY"), this->getMeasure("menuHeaderTextYMax"));
        }

        this->getView()->addToRender(this->getHeaderText());
    }

    if(this->getUpperArrow() != nullptr && !this->isFirstPage(viewData))
    {
        this->getView()->addToRender(this->getUpperArrow());
    }

    if(this->getLowerArrow() != nullptr && !this->isLastPage(viewData))
    {
        this->getView()->addToRender(this->getLowerArrow());
    }

    this->renderMenuElements(viewData);

    return this;
}

MenuView* MenuView::renderMenuElements(const ViewData& viewData)
{

    bool hasToReload = false;
    if(viewData.menuEntities.count(viewData.currentMenu) > 0)
    {
        if(viewData.menuEntities.at(viewData.currentMenu).size() != this->getMenuElements(viewData.currentMenu).size())
        {
            hasToReload = true;
        }
        if(!hasToReload)
        {
            for(const auto& ent : viewData.menuEntities.at(viewData.currentMenu))
            {
                if(ent.second->getEntityType() != this->getMenuElement(viewData.currentMenu, ent.first)->getEntityType())
                {
                    hasToReload = true;
                }
            }
        }
    }

    if(hasToReload)
    {
        this->loadElements();
    }

    if(viewData.menuEntities.count(viewData.currentMenu) > 0)
    {
        int i =0;
        for(const auto& ent : viewData.menuEntities.at(viewData.currentMenu))
        {
            if(ent.first >= (this->getPage(viewData) * MENU_ELEMENTS_MAX) && ent.first < ((this->getPage(viewData) + 1) * MENU_ELEMENTS_MAX))
            {
                MenuElement* element = this->getMenuElement(viewData.currentMenu, ent.first);
                if(element != nullptr)
                {
                    int posX = this->getMeasure("menuElementX");
                    int posY = this->getMeasure("menuElementY") + (this->getMeasure("menuElementHeight") * i);

                    if(element->getPosX() != posX || element->getPosY() != posY)
                    {
                        element->setPosition(posX, posY);
                    }


                    if(viewData.selectedItem == ent.first)
                    {
                        element->setHovered();

                        if(this->getSelector() != nullptr)
                        {
                            this->getSelector()->setPosition(posX, posY);
                            this->getView()->addToRender(this->getSelector());
                        }
                    }
                    else if(element->isHovered() && viewData.selectedItem != ent.first)
                    {
                        element->unsetHovered();
                    }

                    switch(ent.second->getEntityType())
                    {
                        case MenuEntityType::TYPE_BUTTON :
                        {
                            TextButton* textButton = static_cast<TextButton*>(element);
                            const MenuButtonEntity* buttonEntity = static_cast<const MenuButtonEntity*>(ent.second);

                            if(buttonEntity->getLabel() != textButton->getText())
                            {
                                textButton->setText(buttonEntity->getLabel());
                            }
                            break;
                        }

                        case MenuEntityType::TYPE_TEXT_BOX :
                        {
                            TextBox* textBox = static_cast<TextBox*>(element);
                            const TextBoxEntity* boxEntity = static_cast<const TextBoxEntity*>(ent.second);

                            if(boxEntity->getLabel() != textBox->getLabelText())
                            {
                                textBox->setLabel(boxEntity->getLabel(), textBox->getLabelPosition());
                            }

                            if(boxEntity->getContent() != textBox->getContent())
                            {
                                textBox->setContent(boxEntity->getContent());
                            }

                            if(boxEntity->isPassword() != textBox->isPassword())
                            {
                                textBox->isPassword(boxEntity->isPassword());
                            }

                            break;
                        }

                        case MenuEntityType::TYPE_WIFI_BUTTON :
                        {
                            WifiButton* wifiButton = static_cast<WifiButton*>(element);
                            const WifiButtonEntity* wifiButtonEntity = static_cast<const WifiButtonEntity*>(ent.second);

                            if(wifiButtonEntity->hasAuth() != wifiButton->hasAuth())
                            {
                                wifiButton->hasAuth(wifiButtonEntity->hasAuth());
                            }

                            if(wifiButtonEntity->getSignalPower() != wifiButton->getSignalPower())
                            {
                                wifiButton->setSignalPower(wifiButtonEntity->getSignalPower());
                            }

                            if(wifiButtonEntity->getLabel() != wifiButton->getText())
                            {
                                wifiButton->setText(wifiButtonEntity->getLabel());
                            }

                            break;
                        }
                    }

                    this->getView()->addToRender(element);
                }
                ++i;
            }
        }
    }
    return this;
}

MenuView* MenuView::loadImages()
{
    this->setHeader(new RoundedRectangle(this->getView()->getRenderer(), Color::getColorFromMap(MENU_HEADER_COLOR), this->getMeasure("menuWidth"), this->getMeasure("menuHeaderHeight"), this->getMeasure("menuX"), this->getMeasure("menuHeaderY"), RoundedAngles::TOP_ANGLES, 100));
    this->setBackground(new RoundedRectangle(this->getView()->getRenderer(), Color::getColorFromMap(MENU_BACKGROUND_COLOR), this->getMeasure("menuWidth"), this->getMeasure("menuContentMaxHeight"), this->getMeasure("menuX"), this->getMeasure("menuContentY"), RoundedAngles::BOTTOM_ANGLES, MENU_BACKGROUND_SLOPE));

    int headerTextFontSize = Font::getBiggestFontToFit(FontFamily::ROBOTO, MENU_HEADER_TEXT_CONTENT, this->getMeasure("menuHeaderTextWidth"), this->getMeasure("menuHeaderTextHeight"));
    this->setHeaderText(new Text(this->getView()->getRenderer(), MENU_HEADER_TEXT_CONTENT, Font::getFontFromMap(FontFamily::ROBOTO, headerTextFontSize), Color::getColorFromMap(MENU_HEADER_TEXT_COLOR), this->getMeasure("menuHeaderTextX"), this->getMeasure("menuHeaderTextHeight")));
    this->getHeaderText()->center(this->getMeasure("menuHeaderTextX"), this->getMeasure("menuHeaderTextXMax"), this->getMeasure("menuHeaderTextY"), this->getMeasure("menuHeaderTextYMax"));

    this->setUpperArrow(new Image(this->getView()->getRenderer(), UPPER_ARROW_FILE));
    if(this->getUpperArrow() != nullptr)
    {
        this->getUpperArrow()->setSize(this->getMeasure("arrowAreaWidth"), this->getMeasure("arrowHeight"), ResizeMode::FIT);
        this->getUpperArrow()->center(this->getMeasure("arrowAreaX"), this->getMeasure("arrowAreaXMax"), this->getMeasure("upperArrowAreaY"), this->getMeasure("upperArrowAreaYMax"));
    }

    this->setLowerArrow(new Image(this->getView()->getRenderer(), LOWER_ARROW_FILE));
    if(this->getLowerArrow() != nullptr)
    {
        this->getLowerArrow()->setSize(this->getMeasure("arrowAreaWidth"), this->getMeasure("arrowHeight"), ResizeMode::FIT);
        this->getLowerArrow()->center(this->getMeasure("arrowAreaX"), this->getMeasure("arrowAreaXMax"), this->getMeasure("lowerArrowAreaY"), this->getMeasure("lowerArrowAreaYMax"));
    }

    this->setSelector(new Rectangle(this->getView()->getRenderer(), Color::getColorFromMap(MENU_SELECTOR_COLOR), this->getMeasure("menuElementWidth"), this->getMeasure("menuElementHeight"), this->getMeasure("menuElementX"), this->getMeasure("menuElementY")));

    this->loadElements();

    return this;
}

MenuView* MenuView::loadElements()
{
    for(const auto& ent1 : this->getMenuElements())
    {
        for(const auto& ent2 : ent1.second)
        {
            if(ent2.second != nullptr)
            {
                delete ent2.second;
            }
        }
    }
    this->m_menuElements.clear();

    for(const auto& ent1 : this->getView()->getController()->viewData.menuEntities)
    {
        for(const auto& ent2 : ent1.second)
        {
            MenuElement* menuElement = nullptr;

            switch(ent2.second->getEntityType())
            {
                case MenuEntityType::TYPE_BUTTON :
                {
                    const MenuButtonEntity* menuButton = static_cast<const MenuButtonEntity*>(ent2.second);
                    if(menuButton != nullptr)
                    {
                        menuElement = new TextButton(this->getView()->getRenderer(), Color::getColorFromMap(MENU_ELEMENTS_COLOR), this->getMeasure("menuElementWidth"), this->getMeasure("menuElementHeight"), this->getMeasure("menuElementX"), this->getMeasure("menuElementY"), menuButton->getLabel());
                    }
                    break;
                }
                case MenuEntityType::TYPE_TEXT_BOX :
                {
                    const TextBoxEntity* textBox = static_cast<const TextBoxEntity*>(ent2.second);
                    if(textBox != nullptr)
                    {
                        menuElement = new TextBox(this->getView()->getRenderer(), Color::getColorFromMap(MENU_ELEMENTS_COLOR), this->getMeasure("menuElementWidth"), this->getMeasure("menuElementHeight"), this->getMeasure("menuElementX"), this->getMeasure("menuElementY"), textBox->getLabel(), textBox->getContent(), textBox->isPassword());
                    }
                }
                case MenuEntityType::TYPE_WIFI_BUTTON :
                {
                    const WifiButtonEntity* wifiButton = static_cast<const WifiButtonEntity*>(ent2.second);
                    if(wifiButton != nullptr)
                    {
                        menuElement = new WifiButton(this->getView()->getRenderer(), Color::getColorFromMap(MENU_ELEMENTS_COLOR), this->getMeasure("menuElementWidth"), this->getMeasure("menuElementHeight"), this->getMeasure("menuElementX"), this->getMeasure("menuElementY"), wifiButton->getLabel(), wifiButton->getSignalPower(), wifiButton->hasAuth());
                    }
                }
            }

            if(menuElement != nullptr)
            {
                this->addMenuElement(ent1.first, ent2.first, menuElement);
            }
        }
    }
    return this;
}

MenuView* MenuView::destroyImages()
{
    AbstractView::destroyImages();

    if(this->getBackground() != nullptr)
    {
        this->setBackground(nullptr);
    }

    if(this->getHeader() != nullptr)
    {
        this->setHeader(nullptr);
    }

    if(this->getHeaderText() != nullptr)
    {
        this->setHeaderText(nullptr);
    }

    if(this->getUpperArrow() != nullptr)
    {
        this->setUpperArrow(nullptr);
    }

    if(this->getLowerArrow() != nullptr)
    {
        this->setLowerArrow(nullptr);
    }

    if(this->getSelector() != nullptr)
    {
        this->setSelector(nullptr);
    }

    for(const auto& ent1 : this->getMenuElements())
    {
        for(const auto& ent2 : ent1.second)
        {
            if(ent2.second != nullptr)
            {
                delete ent2.second;
            }
        }
    }
    this->m_menuElements.clear();

    return this;
}

MenuView* MenuView::calculateMeasures()
{
    AbstractView::calculateMeasures();

    this->addMeasure("menuWidth", this->getMeasure("compensatedWidth") * 3 / 8);
    this->addMeasure("menuX", this->getMeasure("compensatedX") + (this->getMeasure("compensatedWidth") * 5 / 16));
    this->addMeasure("menuXMax", this->getMeasure("menuX") + this->getMeasure("menuWidth"));

    this->addMeasure("menuHeaderHeight", this->getMeasure("compensatedHeight") / 16);
    this->addMeasure("menuHeaderY", this->getMeasure("compensatedY") + (this->getMeasure("compensatedHeight") / 4));
    this->addMeasure("menuHeaderYMax", this->getMeasure("menuHeaderY") + this->getMeasure("menuHeaderHeight"));

    this->addMeasure("menuHeaderTextHorizontalMargin", this->getMeasure("menuHeaderHeight"));
    this->addMeasure("menuHeaderTextWidth", this->getMeasure("menuWidth") - (this->getMeasure("menuHeaderTextHorizontalMargin") * 2));
    this->addMeasure("menuHeaderTextVerticalMargin", this->getMeasure("menuHeaderHeight") / 5);
    this->addMeasure("menuHeaderTextHeight", this->getMeasure("menuHeaderHeight") - (this->getMeasure("menuHeaderTextVerticalMargin") * 2));
    this->addMeasure("menuHeaderTextX", this->getMeasure("menuX") + this->getMeasure("menuHeaderTextHorizontalMargin"));
    this->addMeasure("menuHeaderTextXMax", this->getMeasure("menuHeaderTextX") + this->getMeasure("menuHeaderTextWidth"));
    this->addMeasure("menuHeaderTextY", this->getMeasure("menuHeaderY") + this->getMeasure("menuHeaderTextVerticalMargin"));
    this->addMeasure("menuHeaderTextYMax", this->getMeasure("menuHeaderTextY") + this->getMeasure("menuHeaderTextHeight"));

    this->addMeasure("menuContentMaxHeight", (this->getMeasure("compensatedHeight") / 2) - this->getMeasure("menuHeaderHeight"));
    this->addMeasure("menuContentY", this->getMeasure("menuHeaderY") + this->getMeasure("menuHeaderHeight"));

    if(this->getMeasure("menuWidth") > this->getMeasure("menuContentMaxHeight"))
    {
        this->addMeasure("menuContentVerticalPadding", this->getMeasure("menuContentMaxHeight") / MENU_BACKGROUND_SLOPE);
    }
    else
    {
        this->addMeasure("menuContentVerticalPadding", this->getMeasure("menuWidth") / MENU_BACKGROUND_SLOPE);
    }


    this->addMeasure("arrowAreaWidth", this->getMeasure("menuWidth") - (this->getMeasure("menuContentVerticalPadding") * 2));
    this->addMeasure("arrowAreaHeight", this->getMeasure("menuContentVerticalPadding"));
    this->addMeasure("arrowAreaX", this->getMeasure("menuX") + this->getMeasure("menuContentVerticalPadding"));
    this->addMeasure("arrowAreaXMax", this->getMeasure("arrowAreaX") + this->getMeasure("arrowAreaWidth"));
    this->addMeasure("upperArrowAreaY", this->getMeasure("menuContentY"));
    this->addMeasure("arrowHeight", this->getMeasure("arrowAreaHeight") / 3);
    this->addMeasure("upperArrowAreaYMax", this->getMeasure("upperArrowAreaY") + this->getMeasure("arrowAreaHeight"));
    this->addMeasure("lowerArrowAreaYMax", this->getMeasure("menuContentY") + this->getMeasure("menuContentMaxHeight"));
    this->addMeasure("lowerArrowAreaY", this->getMeasure("lowerArrowAreaYMax") - this->getMeasure("arrowAreaHeight"));

    this->addMeasure("menuElementWidth", this->getMeasure("menuWidth"));
    this->addMeasure("menuElementX", this->getMeasure("menuX"));
    this->addMeasure("menuElementHeight", ((this->getMeasure("menuContentMaxHeight") - (this->getMeasure("arrowAreaHeight") * 2)) / MENU_ELEMENTS_MAX) - 1);
    this->addMeasure("menuElementY", this->getMeasure("menuContentY") + this->getMeasure("arrowAreaHeight"));
    return this;
}

RoundedRectangle* MenuView::getBackground() const
{
    return this->m_pBackground;
}

MenuView* MenuView::setBackground(RoundedRectangle* background)
{
    if(this->m_pBackground != nullptr)
    {
        delete this->m_pBackground;
    }
    this->m_pBackground = background;
    return this;
}

RoundedRectangle* MenuView::getHeader() const
{
    return this->m_pHeader;
}

MenuView* MenuView::setHeader(RoundedRectangle* header)
{
    if(this->m_pHeader != nullptr)
    {
        delete this->m_pHeader;
    }
    this->m_pHeader = header;
    return this;
}

Text* MenuView::getHeaderText() const
{
    return this->m_pHeaderText;
}

MenuView* MenuView::setHeaderText(Text* headerText)
{
    if(this->m_pHeaderText != nullptr)
    {
        delete this->m_pHeaderText;
    }
    this->m_pHeaderText = headerText;
    return this;
}

Image* MenuView::getUpperArrow() const
{
    return this->m_pUpperArrow;
}

MenuView* MenuView::setUpperArrow(Image* upperArrow)
{
    if(this->m_pUpperArrow != nullptr)
    {
        delete this->m_pUpperArrow;
    }
    this->m_pUpperArrow = upperArrow;
    return this;
}

Image* MenuView::getLowerArrow() const
{
    return this->m_pLowerArrow;
}

MenuView* MenuView::setLowerArrow(Image* lowerArrow)
{
    if(this->m_pLowerArrow != nullptr)
    {
        delete this->m_pLowerArrow;
    }

    this->m_pLowerArrow = lowerArrow;
    return this;
}

Rectangle* MenuView::getSelector() const
{
    return this->m_pSelector;
}

MenuView* MenuView::setSelector(Rectangle* selector)
{
    if(this->m_pSelector != nullptr)
    {
        delete this->m_pSelector;
    }

    this->m_pSelector = selector;
    return this;
}

MenuElement* MenuView::getMenuElement(const CurrentMenu& menu, const int& elementID) const
{
    if(this->m_menuElements.count(menu) > 0)
    {
        if(this->m_menuElements.at(menu).count(elementID) > 0)
        {
            return this->m_menuElements.at(menu).at(elementID);
        }
    }
    return nullptr;
}

map<int, MenuElement*> MenuView::getMenuElements(const CurrentMenu& menu)
{
    if(this->m_menuElements.count(menu) > 0)
    {
        return this->m_menuElements.at(menu);
    }
    //Throw exception
}

map<CurrentMenu, map<int, MenuElement*>> MenuView::getMenuElements() const
{
    return this->m_menuElements;
}

MenuView* MenuView::addMenuElement(const CurrentMenu& menu, const int& elementID, MenuElement* menuElement)
{
    if(this->m_menuElements.count(menu) > 0)
    {
        if(this->m_menuElements.at(menu).count(elementID) > 0)
        {
            delete this->m_menuElements.at(menu).at(elementID);
            this->m_menuElements.at(menu).erase(elementID);
        }
    }

    this->m_menuElements[menu][elementID] = menuElement;

    return this;
}

int MenuView::getNumberElements(const ViewData& viewData) const
{
    if(viewData.menuEntities.count(viewData.currentMenu) > 0)
    {
        return viewData.menuEntities.at(viewData.currentMenu).size();
    }
    return 0;
}

int MenuView::getPage(const ViewData& viewData) const
{
    if(viewData.menuEntities.count(viewData.currentMenu) > 0)
    {
        if(this->getNumberElements(viewData) > 0)
        {
            return (viewData.selectedItem / MENU_ELEMENTS_MAX);
        }
    }
    return 0;
}

bool MenuView::isFirstPage(const ViewData& viewData) const
{
    if(this->getPage(viewData) == 0)
    {
        return true;
    }
    return false;
}

bool MenuView::isLastPage(const ViewData& viewData) const
{
    if(this->getPage(viewData) == ((this->getNumberElements(viewData) - 1) / MENU_ELEMENTS_MAX))
    {
        return true;
    }
    return false;
}
