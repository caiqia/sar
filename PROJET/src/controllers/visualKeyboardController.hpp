#ifndef DEF_VISUALKEYBOARDCONTROLLER
#define DEF_VISUALKEYBOARDCONTROLLER

#include <map>
#include <string>

#include "abstractController.hpp"
#include "enums/keysEnum.hpp"

class VisualKeyboardController : public AbstractController
{
public:
    VisualKeyboardController(Controller* controller);
    ~VisualKeyboardController();

    virtual VisualKeyboardController* control();

    virtual VisualKeyboardController* inputControl();

    VisualKeyboardController* selectNextKey(const Keys& keyFrom);
    VisualKeyboardController* selectPreviousKey(const Keys& keyFrom);
    VisualKeyboardController* selectNextRow(const Keys& keyFrom);
    VisualKeyboardController* selectPreviousRow(const Keys& keyFrom);

    static void askForInput(std::string* stringToReturn);

    VisualKeyboardController* pushCurrentKey();
    VisualKeyboardController* addCurrentSelectedCharacter();
    VisualKeyboardController* removeLastCharacter();
    VisualKeyboardController* validate();

    bool isDisplayed();
    VisualKeyboardController* setDisplayed(bool isDisplayed);

    Keys getCurrentSelectedKey();
    VisualKeyboardController* setCurrentSelectedKey(Keys key);

    KeyboardPannel getCurrentPannel();
    VisualKeyboardController* setCurrentPannel(KeyboardPannel pannel);

    std::string getCurrentInput();

    VisualKeyboardController* addDoubledKey(const KeyboardPannel& pannel, const Keys& key1, const Keys& key2);
    Keys getDoubledKey(const KeyboardPannel& pannel, const Keys& key) const;
    bool isDoublerKey(const KeyboardPannel& pannel, const Keys& key) const;
    bool isDoubledKey(const KeyboardPannel& pannel, const Keys& key) const;

    static std::map<KeyboardPannel, std::map <Keys, std::string>> keyResolver;
    static VisualKeyboardController* m_pThis;

protected:
    bool m_displayed = false;

    Keys m_selectedKey = Keys::KEY1;

    KeyboardPannel m_currentPannel = KeyboardPannel::MAIN;

    std::string m_currentInput;
    std::string* m_stringToUpdate = nullptr;

    static std::map<Keys, int> keyNumberResolver;
    static std::map<int, Keys> keyNumberReverser;

    std::map<KeyboardPannel, std::map<Keys, Keys>> m_doubledKeys;
    std::map<KeyboardPannel, std::map<Keys, Keys>> m_doubledKeysReverser;
};

#endif
