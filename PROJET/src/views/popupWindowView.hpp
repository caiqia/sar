#ifndef DEF_POPUPWINDOWVIEW
#define DEF_POPUPWINDOWVIEW

#include "abstractView.hpp"

#include <map>

class AbstractPopupWindowView : public AbstractView
{
public:
    AbstractPopupWindowView(View* view, const ViewsID& viewID);
    ~AbstractPopupWindowView();

    virtual AbstractPopupWindowView* render(const ViewData& viewData) = 0;

    virtual AbstractPopupWindowView* loadImages() = 0;

    virtual AbstractPopupWindowView* destroyImages() = 0;

    virtual AbstractPopupWindowView* calculateMeasures();

    AbstractPopupWindowView* setWindowHeader(RoundedRectangle* windowHeader);
    RoundedRectangle* getWindowHeader() const;

    AbstractPopupWindowView* setHeaderText(Text* headerText);
    AbstractPopupWindowView* setHeaderText(std::string text);
    Text* getHeaderText() const;

    AbstractPopupWindowView* setContentWindow(RoundedRectangle* contentWindow);
    RoundedRectangle* getContentWindow() const;

    AbstractPopupWindowView* createButtons(const std::map<int, std::string>& buttons);

    AbstractPopupWindowView* addButton(const int& buttonNumber, RoundedRectangle* button);
    RoundedRectangle* getButton(const int& buttonNumber) const;
    std::map<int, RoundedRectangle*> getButtons() const;

    AbstractPopupWindowView* addButtonContent(const int& buttonNumber, Text* buttonContent);
    Text* getButtonContent(const int& buttonNumber) const;
    std::map<int, Text*> getButtonsContent() const;

protected:
    RoundedRectangle* m_pWindowHeader = nullptr;
    Text* m_pHeaderText = nullptr;
    RoundedRectangle* m_pContentWindow = nullptr;

    std::map<int, RoundedRectangle*> m_pButtons;
    std::map<int, Text*> m_pButtonsContent;
};

class PopupWindowView : public AbstractPopupWindowView
{
public:
    PopupWindowView(View* view);
    ~PopupWindowView();

    virtual PopupWindowView* render(const ViewData& viewData);

    virtual PopupWindowView* loadImages();

    virtual PopupWindowView* destroyImages();

    virtual PopupWindowView* calculateMeasures();

    PopupWindowView* setContent(WrappedText* content);
    PopupWindowView* setContent(const std::string& content);
    WrappedText* getContent() const;

protected:
    WrappedText* m_pContent = nullptr;

};

class InputPopupWindowView : public AbstractPopupWindowView
{
public:
    InputPopupWindowView(View* view);
    ~InputPopupWindowView();

    virtual InputPopupWindowView* render(const ViewData& viewData);

    virtual InputPopupWindowView* loadImages();

    virtual InputPopupWindowView* destroyImages();

    virtual InputPopupWindowView* calculateMeasures();

    InputPopupWindowView* setHelpText(Text* helpText);
    InputPopupWindowView* setHelpText(const std::string& helpText);
    Text* getHelpText() const;

    InputPopupWindowView* setInputBox(RoundedRectangle* inputBox);
    RoundedRectangle* getInputBox() const;

    InputPopupWindowView* setInputContent(Text* inputContent);
    InputPopupWindowView* setInputContent(const std::string& inputContent);
    Text* getInputContent() const;

protected:
    Text* m_pHelpText = nullptr;

    RoundedRectangle* m_pInputBox = nullptr;
    Text* m_pInputText = nullptr;
};

#endif
