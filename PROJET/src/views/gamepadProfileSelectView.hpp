#ifndef DEF_GAMEPADPROFILESELECTVIEW
#define DEF_GAMEPADPROFILESELECTVIEW

#include"abstractView.hpp"

class View;

class GamepadProfileSelectView : public AbstractView
{
public:
    GamepadProfileSelectView(View* view);
    ~GamepadProfileSelectView();

    virtual GamepadProfileSelectView* render(const ViewData& viewData);

    virtual GamepadProfileSelectView* loadImages();
    virtual GamepadProfileSelectView* destroyImages();

    virtual GamepadProfileSelectView* calculateMeasures();

    GamepadProfileSelectView* setWindowHeader(RoundedRectangle* windowHeader);
    RoundedRectangle* getWindowHeader() const;

    GamepadProfileSelectView* setHeaderText(Text* headerText);
    GamepadProfileSelectView* setHeaderText(std::string text);
    Text* getHeaderText() const;

    GamepadProfileSelectView* setContentWindow(RoundedRectangle* contentWindow);
    RoundedRectangle* getContentWindow() const;

    GamepadProfileSelectView* setSelector(RoundedRectangle* selector);
    RoundedRectangle* getSelector() const;

    GamepadProfileSelectView* setSNESProfile(Image* SNESProfile);
    Image* getSNESProfile() const;

    GamepadProfileSelectView* setNESProfile(Image* NESProfile);
    Image* getNESProfile() const;

protected:
    RoundedRectangle* m_pWindowHeader = nullptr;
    Text* m_pHeaderText = nullptr;
    RoundedRectangle* m_pContentWindow = nullptr;

    int currentlyDisplayedPlayer = -1;

    RoundedRectangle* m_pSelector = nullptr;

    Image* m_pSNESProfile = nullptr;
    Image* m_pNESProfile = nullptr;
};

#endif
