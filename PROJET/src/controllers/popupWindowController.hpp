#ifndef DEF_POPUPWINDOWCONTROLLER
#define DEF_POPUPWINDOWCONTROLLER

#include "abstractController.hpp"

#include <map>
#include <string>
#include <vector>

class Controller;

class PopupWindowController : public AbstractController
{
public:
    PopupWindowController(Controller* controller);
    ~PopupWindowController();

    virtual PopupWindowController* control();

    virtual PopupWindowController* inputControl();

    PopupWindowController* isActive(const bool& isActive);
    bool isActive() const;

    PopupWindowController* setWindowTitle(const std::string& windowTitle);
    std::string getWindowTitle() const;

    PopupWindowController* setWindowContent(const std::string windowContent);
    std::string getWindowContent() const;

    PopupWindowController* addButton(const std::string& button);
    PopupWindowController* clearButtons();
    std::map<int, std::string> getButtons() const;

    PopupWindowController* setSelectedButton(const int& selectedButton);
    int getSelectedButton() const;
    std::string getSelectedButtonString() const;
    int getNumberButtons() const;

    PopupWindowController* selectNextButton();
    PopupWindowController* selectPreviousButton();
    PopupWindowController* popup(const std::string& title, const std::string& content, const std::vector<std::string>& buttons, std::string* outputString);
    PopupWindowController* validate();

    PopupWindowController* setOutputString(std::string* outputString);
    std::string* getOutputStringPointer() const;

protected:
    bool m_popupWindowActive = false;
    std::string m_windowTitle;
    std::string m_windowContent;
    std::map<int, std::string> m_buttons;

    int m_selectedButton = 0;
    int m_buttonCounter = 0;

    std::string* m_pOutputString;
};

#endif
