#ifndef DEF_MENUELEMENTS
#define DEF_MENUELEMENTS

#include "../views/surface.hpp"
#include <string>
#include "menuEntities.hpp"
#include "../views/text.hpp"
#include "../views/image.hpp"
#include "../entities/network.hpp"

class MenuElement : public Surface
{
public:
    MenuElement(SDL_Renderer* renderer, Color* color, const int& width, const int& height, const int& posX, const int& posY, const MenuEntityType& entityType);
    ~MenuElement();

    virtual MenuElement* render();

    virtual MenuElement* setHovered();
    virtual MenuElement* unsetHovered();
    bool isHovered() const;

    virtual MenuElement* initMeasures();
    virtual MenuElement* setMeasure(const std::string& measure, const int& value);
    int getMeasure(const std::string& measure) const;

    virtual MenuElement* setPosition(const int& posX, const int& posY) = 0;

    MenuElement* setOriginalColor(Color* color);
    Color* getOriginalColor() const;

    MenuElement* setRightArrow(Image* rightArrowImage);
    Image* getRightArrow() const;

    MenuElement* setRightArrowWhite(Image* rightArrowImage);
    Image* getRightArrowWhite() const;

    MenuElement* setEntityType(const MenuEntityType& entityType);
    MenuEntityType getEntityType() const;

protected:
    bool m_hovered = false;
    std::map<std::string, int> m_measures;
    Color* m_originalColor;

    Image* m_pRightArrow = nullptr;
    Image* m_pRightArrowWhite = nullptr;

    MenuEntityType m_entityType;
};

class TextButton : public MenuElement
{
public:
    TextButton(SDL_Renderer* renderer, Color* color, const int& width, const int& height, const int& posX, const int& posY, const std::string& text);
    ~TextButton();

    virtual TextButton* render();

    virtual TextButton* initMeasures();

    virtual TextButton* setColor(Color* color);

    TextButton* setText(const std::string& text);
    std::string getText() const;

    TextButton* setText(Text* textObjectPointer);
    Text* getTextObject() const;

    virtual TextButton* setPosition(const int& posX, const int& posY);

protected:
    std::string m_text;

    Text* m_pText = nullptr;
};

class WifiButton : public MenuElement
{
public:
    WifiButton(SDL_Renderer* renderer, Color* color, const int& width, const int& height, const int& posX, const int& posY, const std::string& text, const WifiSignalPower& wifiSignalPower, const bool hasAuth);
    ~WifiButton();

    virtual WifiButton* render();

    virtual WifiButton* initMeasures();

    virtual WifiButton* setColor(Color* color);

    virtual WifiButton* setPosition(const int& posX, const int& posY);
    WifiButton* loadWifiImage();

    bool hasAuth() const;
    WifiButton* hasAuth(const bool hasAuth);

    WifiSignalPower getSignalPower() const;
    WifiButton* setSignalPower(const WifiSignalPower& signalPower);


    WifiButton* setText(const std::string& text);
    std::string getText() const;

    WifiButton* setText(Text* textObjectPointer);
    Text* getTextObject() const;

    WifiButton* setWifiImage(Image* wifiImage);
    Image* getWifiImage() const;

    WifiButton* setHoveredWifiImage(Image* hoveredWifiImage);
    Image* getHoveredWifiImage() const;


protected:
    std::string m_text;
    Text* m_pText = nullptr;
    Image* m_pWifiImage = nullptr;
    Image* m_pHoveredWifiImage = nullptr;

    WifiSignalPower m_signalPower;
    bool m_hasAuth;
};

enum LabelPosition
{
    SIDE,
    TOP
};

class LabelledMenuElement : public MenuElement
{
public:
    LabelledMenuElement(SDL_Renderer* render, Color* color, int width, int height, int posX, int posY, std::string label, const MenuEntityType& entityType, LabelPosition labelPosition = LabelPosition::SIDE);
    ~LabelledMenuElement();

    virtual LabelledMenuElement* render();

    virtual LabelledMenuElement* setColor(Color* color);

    LabelledMenuElement* setLabel(std::string labelText, LabelPosition labelPosition);

    std::string getLabelText();
    LabelPosition getLabelPosition();

    LabelledMenuElement* setLabelTextObject(Text* labelTextObject);
    Text* getLabelTextObject();

    virtual LabelledMenuElement* initMeasures();

    virtual LabelledMenuElement* setPosition(const int& posX, const int& posY) = 0;

protected:
    std::string m_labelText;
    LabelPosition m_labelPosition = LabelPosition::SIDE;

    Text* m_pLabel = nullptr;
};

class TextBox : public LabelledMenuElement
{
public:
    TextBox(SDL_Renderer* renderer, Color* color, int width, int height, int posX, int posY, std::string label, std::string content, bool isPassword = false, LabelPosition labelPosition = LabelPosition::SIDE);
    ~TextBox();

    virtual TextBox* render();

    virtual TextBox* setColor(Color* color);

    TextBox* setContent(std::string);
    std::string getContent();

    TextBox* isPassword(bool isPassword);
    bool isPassword();

    TextBox* setTextBoxContent(Text* textBoxContent);
    Text* getTextBoxContent();

    virtual TextBox* initMeasures();

    std::string passwordize(std::string original);

    virtual TextBox* setPosition(const int& posX, const int& posY);

protected:
    std::string m_content;
    bool m_isPassword = false;

    Text* m_pTextBoxContent = nullptr;
};

class MultipleChoice : public LabelledMenuElement
{
public:
    MultipleChoice(SDL_Renderer* renderer, Color* color, int width, int height, int posX, int posY, std::string label, std::string currentChoice, bool canGoLeft = true, bool canGoRight = true, LabelPosition labelPosition = LabelPosition::SIDE);
    ~MultipleChoice();

    virtual MultipleChoice* render();

    MultipleChoice* setCurrentChoice(std::string currentChoice);
    std::string getCurrentChoice();

    MultipleChoice* setAvailableDirections(bool left, bool right);
    bool canGoLeft();
    bool canGoRight();

    MultipleChoice* setLeftArrow(Image* leftArrowImage);
    Image* getLeftArrow();

    MultipleChoice* setRightArrow(Image* rightArrowImage);
    Image* getRightArrow();

    MultipleChoice* setChoiceText(Text* choiceText);
    Text* getChoiceText();

    MultipleChoice* initMeasures();

protected:
    std::string m_currentChoice;
    bool m_canGoRight = true;
    bool m_canGoLeft = true;

    Image* m_pLeftArrow = nullptr;
    Image* m_pRightArrow = nullptr;
    Text* m_pChoiceText = nullptr;
};

class Slider : public LabelledMenuElement
{
public:
    Slider(SDL_Renderer* renderer, std::string label, int value);
    ~Slider();

    virtual Slider* render();

    Slider* setValue(int value);
    int getValue();

    Slider* setSlidderButton(Ellipse* slidderButton);
    Ellipse* getSliderButton();

    Slider* setSliderBar(Line* slidderBar);
    Line* getSlidderBar();

protected:
    int m_value;

    Ellipse* m_pSlidderButton = nullptr;
    Line* m_pSliderBar = nullptr;
};

#endif
