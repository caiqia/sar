#ifndef DEF_MENUENTITIES
#define DEF_MENUENTITIES

#include "../entities/network.hpp"
#include <string>

enum EntityID
{
    PARAMETERS_BTN,
        AUDIO_PARAMETERS_BTN,
            VOLUME_SLIDER,
            AUDIO_OUTPUT_MULTIPLE_CHOICE,
        VIDEO_PARAMETERS_BTN,
        NETWORK_PARAMETERS_BTN,
            NETWORK_STATUS_BTN,
            WIRELESS_CONNEXION_BTN,
                SCAN_WIFI_BTN,
                WIFI_BUTTON,
        GAME_PARAMETERS_BTN,
        INPUT_PARAMETERS_BTN,
    INFORMATION_BTN,
        CREDITS_INFORMATION_BTN,
        SYSTEM_INFORMATION_BTN,
        HELP_INFORMATION_BTN,
    UPDATE_BTN,
    QUIT_BTN,

};

enum MenuEntityType
{
    TYPE_BUTTON,
    TYPE_WIFI_BUTTON,
    TYPE_TEXT_BOX,
    TYPE_SLIDER,
    TYPE_MULTIPLE_CHOICE
};

class MenuEntity
{
public:
    MenuEntity(const MenuEntityType& entityType, const EntityID& ID, const std::string& label);
    ~MenuEntity();

    MenuEntity* setEntityType(const MenuEntityType& entityType);
    MenuEntityType getEntityType() const;

    MenuEntity* setLabel(const std::string& label);
    std::string getLabel() const;

    MenuEntity* isHovered(const bool& hovered);
    bool isHovered() const;

    MenuEntity* setID(const EntityID& ID);
    EntityID getID() const;

protected:
    std::string m_label;

    EntityID m_ID;

    bool m_hovered;

    MenuEntityType m_entityType;
};

class MenuButtonEntity : public MenuEntity
{
public:
    MenuButtonEntity(const EntityID& ID, const std::string& label);
    ~MenuButtonEntity();

protected:

};

class WifiButtonEntity : public MenuEntity
{
public:
    WifiButtonEntity(const EntityID& ID, const std::string& label, const WifiSignalPower& signalPower, bool hasAuth);
    ~WifiButtonEntity();

    WifiSignalPower getSignalPower() const;
    WifiButtonEntity* setSignalPower(const WifiSignalPower& wifiSignalPower);

    bool hasAuth() const;
    WifiButtonEntity* hasAuth(const bool hasAuth);

protected:
    WifiSignalPower m_signalPower;
    bool m_hasAuth;
};

class TextBoxEntity : public MenuEntity
{
public:
    TextBoxEntity(const EntityID& ID, const std::string& label, const std::string& content, bool isPassword = false);
    ~TextBoxEntity();

    std::string getContent() const;
    std::string* getContentPointer();
    TextBoxEntity* setContent(const std::string& content);

    bool isPassword() const;
    TextBoxEntity* isPassword(const bool& isPassword);

protected:
    std::string m_content;
    bool m_isPassword;
};

#endif
