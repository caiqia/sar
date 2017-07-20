#include "popupWindowController.hpp"
#include "controller.hpp"

using namespace std;

PopupWindowController::PopupWindowController(Controller* controller)
: AbstractController(controller, POPUP_WINDOW_CONTROLLER)
{

}

PopupWindowController::~PopupWindowController()
{

}

PopupWindowController* PopupWindowController::control()
{
    AbstractController::control();

    if(this->isActive())
    {
        this->getController()->setOverlayLevel(OverlayLevel::POPUP_WINDOW);
        this->getController()->viewData.displayPopupWindow = true;
        this->getController()->viewData.popupWindowSelectedButton = this->getSelectedButton();
    }
    else
    {
        this->getController()->viewData.displayPopupWindow = false;
    }
    return this;
}
PopupWindowController* PopupWindowController::inputControl()
{
    AbstractController::inputControl();

    for(const auto& ent : this->getController()->getAdvancedControls())
    {
        if(this->getController()->getGamepadControl(ent.first, GamepadActions::RIGHT))
        {
            this->selectNextButton();
        }
        if(this->getController()->getGamepadControl(ent.first, GamepadActions::LEFT))
        {
            this->selectPreviousButton();
        }
        if(this->getController()->getGamepadControl(ent.first, GamepadActions::B))
        {
            this->validate();
        }
    }
    return this;
}

PopupWindowController* PopupWindowController::isActive(const bool& isActive)
{
    this->m_popupWindowActive = isActive;
    return this;
}

bool PopupWindowController::isActive() const
{
    return this->m_popupWindowActive;
}

PopupWindowController* PopupWindowController::setWindowTitle(const string& windowTitle)
{
    this->m_windowTitle = windowTitle;
    return this;
}

string PopupWindowController::getWindowTitle() const
{
    return this->m_windowTitle;
}

PopupWindowController* PopupWindowController::setWindowContent(const string windowContent)
{
    this->m_windowContent = windowContent;
    return this;
}

string PopupWindowController::getWindowContent() const
{
    return this->m_windowContent;
}

PopupWindowController* PopupWindowController::addButton(const std::string& button)
{
    this->m_buttons.insert(pair<int, string>(this->m_buttonCounter, button));
    this->m_buttonCounter++;
    return this;
}

PopupWindowController* PopupWindowController::clearButtons()
{
    this->m_buttons.clear();
    this->m_buttonCounter = 0;
    return this;
}

map<int, string> PopupWindowController::getButtons() const
{
    return this->m_buttons;
}

PopupWindowController* PopupWindowController::setSelectedButton(const int& selectedButton)
{
    this->m_selectedButton = selectedButton;
    return this;
}

int PopupWindowController::getSelectedButton() const
{
    return this->m_selectedButton;
}

std::string PopupWindowController::getSelectedButtonString() const
{
    if(this->m_buttons.count(this->getSelectedButton()) > 0)
    {
        return this->m_buttons.at(this->getSelectedButton());
    }
    return "";
}

int PopupWindowController::getNumberButtons() const
{
    return this->m_buttons.size();
}

PopupWindowController* PopupWindowController::selectNextButton()
{
    if(this->getSelectedButton() >= this->getNumberButtons() - 1)
    {
        this->setSelectedButton(0);
    }
    else
    {
        this->setSelectedButton(this->getSelectedButton() + 1);
    }
    return this;
}

PopupWindowController* PopupWindowController::selectPreviousButton()
{
    if(this->getSelectedButton() <= 0)
    {
        this->setSelectedButton(this->getNumberButtons() - 1);
    }
    else
    {
        this->setSelectedButton(this->getSelectedButton() - 1);
    }
    return this;
}

PopupWindowController* PopupWindowController::popup(const std::string& title, const std::string& content, const vector<string>& buttons, std::string* outputString)
{
    this->setWindowTitle(title);
    this->setWindowContent(content);
    this->clearButtons();

    for(const auto& ent : buttons)
    {
        this->addButton(ent);
    }

    this->setSelectedButton(0);
    this->setOutputString(outputString);
    this->isActive(true);

    this->getController()->viewData.popupWindowTitle = this->getWindowTitle();
    this->getController()->viewData.popupWindowContent = this->getWindowContent();
    this->getController()->viewData.popupWindowButtons = this->getButtons();

    return this;
}

PopupWindowController* PopupWindowController::validate()
{
    *this->getOutputStringPointer() = this->getSelectedButtonString();
    this->isActive(false);
    this->getController()->viewData.popupWindowTitle = "";
    this->getController()->viewData.popupWindowContent = "";
    this->getController()->viewData.popupWindowButtons.clear();
    return this;
}

PopupWindowController* PopupWindowController::setOutputString(std::string* outputString)
{
    this->m_pOutputString = outputString;
    return this;
}

string* PopupWindowController::getOutputStringPointer() const
{
    return this->m_pOutputString;
}
