#include "inputPopupWindowController.hpp"

using namespace std;

InputPopupWindowController::InputPopupWindowController(Controller* controller)
: AbstractController(controller, INPUT_POPUP_WINDOW_CONTROLLER)
{

}

InputPopupWindowController::~InputPopupWindowController()
{

}

InputPopupWindowController* InputPopupWindowController::control()
{
    if(this->isActive())
    {
        this->getController()->setOverlayLevel(OverlayLevel::INPUT_POPUP_WINDOW);
        this->getController()->viewData.displayInputPopupWindow = true;
        this->getController()->viewData.inputPopupWindowSelectedButton = this->getSelectedButton();
        this->getController()->viewData.inputPopupWindowSelectedButtons = this->getButtonsSelected();
        this->getController()->viewData.inputPopupWindowInput = *this->getUserInputString();
    }
    else
    {
        this->getController()->viewData.displayInputPopupWindow = false;
    }
}

InputPopupWindowController* InputPopupWindowController::inputControl()
{

}

InputPopupWindowController* InputPopupWindowController::isActive(const bool& isActive)
{
    this->m_popupWindowActive = isActive;
    return this;
}

bool InputPopupWindowController::isActive() const
{
    return this->m_popupWindowActive;
}

InputPopupWindowController* InputPopupWindowController::setWindowTitle(const string& windowTitle)
{
    this->m_windowTitle = windowTitle;
}

string InputPopupWindowController::getWindowTitle() const
{
    return this->m_windowTitle;
}

InputPopupWindowController* InputPopupWindowController::setHelpText(const string& helpText)
{
    this->m_helpText = helpText;
    return this;
}

string InputPopupWindowController::getHelpText() const
{
    return this->m_helpText;
}


InputPopupWindowController* InputPopupWindowController::addButton(const string& button)
{
    this->m_buttons.insert(pair<int, string>(this->m_buttonCounter, button));
    this->m_buttonCounter++;
    return this;
}

InputPopupWindowController* InputPopupWindowController::clearButtons()
{
    this->m_buttons.clear();
    this->m_buttonCounter = 0;
    return this;
}

map<int, string> InputPopupWindowController::getButtons() const
{
    return this->m_buttons;
}

InputPopupWindowController* InputPopupWindowController::setSelectedButton(const int& selectedButton)
{
    this->m_selectedButton = selectedButton;
    return this;
}

int InputPopupWindowController::getSelectedButton() const
{
    return this->m_selectedButton;
}

string InputPopupWindowController::getSelectedButtonString() const
{
    if(this->m_buttons.count(this->getSelectedButton()) > 0)
    {
        return this->m_buttons.at(this->getSelectedButton());
    }
    return "";

}

int InputPopupWindowController::getNumberButtons() const
{
    return this->m_buttons.size();
}

InputPopupWindowController* InputPopupWindowController::setButtonsSelected(bool selected)
{
    this->m_buttonsSelected = selected;
    return this;
}
bool InputPopupWindowController::getButtonsSelected() const
{
    return this->m_buttonsSelected;
}

InputPopupWindowController* InputPopupWindowController::selectNextButton()
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

InputPopupWindowController* InputPopupWindowController::selectPreviousButton()
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

InputPopupWindowController* InputPopupWindowController::popup(const string& title, const string& helpText, const vector<string>& buttons, string* outputString, string* userInput)
{
    this->setWindowTitle(title);
    this->setHelpText(helpText);
    this->clearButtons();

    for(const auto& ent : buttons)
    {
        this->addButton(ent);
    }

    this->setSelectedButton(0);
    this->setUserInputString(userInput);
    this->setOutputString(outputString);
    this->isActive(true);

    this->getController()->viewData.inputPopupWindowTitle = this->getWindowTitle();
    this->getController()->viewData.inputPopupWindowHelpText = this->getHelpText();
    this->getController()->viewData.inputPopupWindowInput = *this->getUserInputString();
    this->getController()->viewData.inputPopupWindowButtons = this->getButtons();

    return this;
}

InputPopupWindowController* InputPopupWindowController::validate()
{
    *this->getOutputStringPointer() = this->getSelectedButtonString();
    this->isActive(false);
    this->getController()->viewData.inputPopupWindowTitle = "";
    this->getController()->viewData.inputPopupWindowHelpText = "";
    this->getController()->viewData.inputPopupWindowButtons.clear();
    return this;
}

InputPopupWindowController* InputPopupWindowController::setOutputString(string* outputString)
{
    this->m_pOutputString = outputString;
    return this;
}

string* InputPopupWindowController::getOutputStringPointer() const
{
    return this->m_pOutputString;
}

InputPopupWindowController* InputPopupWindowController::setUserInputString(std::string* userInput)
{
    this->m_pUserInput = userInput;
    return this;
}

std::string* InputPopupWindowController::getUserInputString() const
{
    return this->m_pUserInput;
}
