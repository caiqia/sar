#ifndef DEF_INPUTPOPUPWINDOWCONTROLLER
#define DEF_INPUTPOPUPWINDOWCONTROLLER

#include "abstractController.hpp"

#include <map>
#include <string>
#include <vector>

class Controller;

class InputPopupWindowController : public AbstractController
{
public:
    InputPopupWindowController(Controller* controller);
    ~InputPopupWindowController();

    virtual InputPopupWindowController* control();
    virtual InputPopupWindowController* inputControl();

    InputPopupWindowController* isActive(const bool& isActive);
    bool isActive() const;

    InputPopupWindowController* setWindowTitle(const std::string& windowTitle);
    std::string getWindowTitle() const;

    InputPopupWindowController* setHelpText(const std::string& helpText);
    std::string getHelpText() const;

    InputPopupWindowController* addButton(const std::string& button);
    InputPopupWindowController* clearButtons();
    std::map<int, std::string> getButtons() const;

    InputPopupWindowController* setSelectedButton(const int& selectedButton);
    int getSelectedButton() const;
    std::string getSelectedButtonString() const;
    int getNumberButtons() const;

    InputPopupWindowController* setButtonsSelected(bool selected);
    bool getButtonsSelected() const;

    InputPopupWindowController* selectNextButton();
    InputPopupWindowController* selectPreviousButton();
    InputPopupWindowController* popup(const std::string& title, const std::string& helpText, const std::vector<std::string>& buttons, std::string* outputString, std::string* userInput);
    InputPopupWindowController* validate();

    InputPopupWindowController* setOutputString(std::string* outputString);
    std::string* getOutputStringPointer() const;

    InputPopupWindowController* setUserInputString(std::string* userInput);
    std::string* getUserInputString() const;

protected:
    bool m_popupWindowActive = false;
    std::string m_windowTitle;
    std::string m_helpText;
    std::map<int, std::string> m_buttons;

    int m_selectedButton = 0;
    int m_buttonCounter = 0;
    bool m_buttonsSelected = false;

    std::string* m_pOutputString;
    std::string* m_pUserInput;
};

#endif
