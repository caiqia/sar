#ifndef DEF_VISUALKEYBOARDVIEW
#define DEF_VISUALKEYBOARDVIEW

#include "abstractView.hpp"

class VisualKeyboardView : public AbstractView
{
public:
    VisualKeyboardView(View* view);
    ~VisualKeyboardView();

    virtual VisualKeyboardView* render(const ViewData& viewData);

    virtual VisualKeyboardView* loadImages();

    virtual VisualKeyboardView* destroyImages();

    virtual VisualKeyboardView* calculateMeasures();

    VisualKeyboardView* renderKeyboard(const ViewData& viewData);

    VisualKeyboardView* addKey(const std::string& character, Text* key);
    Text* getKey(const std::string& character) const;
    std::map<std::string, Text*> getKeys() const;

    VisualKeyboardView* setBackground(RoundedRectangle* background);
    RoundedRectangle* getBackground() const;

    VisualKeyboardView* setInputBar(RoundedRectangle* inputBar);
    RoundedRectangle* getInputBar() const;

    VisualKeyboardView* setInputText(Text* text);
    VisualKeyboardView* setInputText(const std::string& text);
    std::string getInputText() const;
    Text* getInputTextObject() const;

    VisualKeyboardView* addKeyBackground(const KeyboardPannel& pannel, const Keys& key, RoundedRectangle* image);
    RoundedRectangle* getKeyBackground(const KeyboardPannel& pannel, const Keys& key) const;
    std::map<Keys, RoundedRectangle*> getKeyBackgrounds(const KeyboardPannel& pannel) const;
    std::map<KeyboardPannel, std::map<Keys, RoundedRectangle*>> getKeyBackgrounds() const;


    bool isSpecialKey(const std::string& character) const;
    std::string getSpecialCharacter(const std::string& character) const;

    VisualKeyboardView* addDoubledKey(const KeyboardPannel& pannel, const Keys& key1, const Keys& key2);
    Keys getDoubledKey(const KeyboardPannel& pannel, const Keys& key) const;
    bool isDoublerKey(const KeyboardPannel& pannel, const Keys& key) const;
    bool isDoubledKey(const KeyboardPannel& pannel, const Keys& key) const;


protected:
    std::map<std::string, Text*> m_keys;

    std::map<std::string, std::string> m_specialKeysConverter = {{"BACK", "←"}, {"SHIFT", "↑"}, {"VALIDATE", "OK ‣"}, {"SPECIAL", "$<√ ‣"}, {"ACCENTUATED", "àéï ‣"}, {"SPACE", "␣"}, {"NORMAL", "aei ‣"}, {"NUMBER", "1?# ‣"}};

    RoundedRectangle* m_pBackground = nullptr;

    RoundedRectangle* m_pInputBar = nullptr;

    Text* m_pInputText = nullptr;

    std::map<KeyboardPannel, std::map<Keys, RoundedRectangle*>> m_keyBackgrounds;

    std::map<KeyboardPannel, std::map<Keys, Keys>> m_doubledKeys;
    std::map<KeyboardPannel, std::map<Keys, Keys>> m_doubledKeysReverser;

};
#endif
