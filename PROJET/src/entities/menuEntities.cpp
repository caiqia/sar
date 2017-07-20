#include "menuEntities.hpp"

using namespace std;

MenuEntity::MenuEntity(const MenuEntityType& entityType, const EntityID& ID, const string& label)
{
    this->setEntityType(entityType);
    this->setLabel(label);
    this->setID(ID);
}

MenuEntity::~MenuEntity()
{

}

MenuEntity* MenuEntity::setEntityType(const MenuEntityType& entityType)
{
    this->m_entityType = entityType;
}

MenuEntityType MenuEntity::getEntityType() const
{
    return this->m_entityType;
}

MenuEntity* MenuEntity::setLabel(const string& label)
{
    this->m_label = label;
    return this;
}

string MenuEntity::getLabel() const
{
    return this->m_label;
}

MenuEntity* MenuEntity::isHovered(const bool& hovered)
{
    this->m_hovered = hovered;
    return this;
}

bool MenuEntity::isHovered() const
{
    return this->m_hovered;
}

MenuEntity* MenuEntity::setID(const EntityID& ID)
{
    this->m_ID = ID;
    return this;
}

EntityID MenuEntity::getID() const
{
    return this->m_ID;
}

/* Button */

MenuButtonEntity::MenuButtonEntity(const EntityID& ID, const std::string& label)
: MenuEntity(MenuEntityType::TYPE_BUTTON, ID, label)
{

}

MenuButtonEntity::~MenuButtonEntity()
{

}

/* ####
    WifiButton
   #### */

WifiButtonEntity::WifiButtonEntity(const EntityID& ID, const std::string& label, const WifiSignalPower& signalPower, bool hasAuth)
: MenuEntity(MenuEntityType::TYPE_WIFI_BUTTON, ID, label)
{
    this->setSignalPower(signalPower);
    this->hasAuth(hasAuth);
}

WifiButtonEntity::~WifiButtonEntity()
{

}

WifiSignalPower WifiButtonEntity::getSignalPower() const
{
    return this->m_signalPower;
}

WifiButtonEntity* WifiButtonEntity::setSignalPower(const WifiSignalPower& wifiSignalPower)
{
    this->m_signalPower = wifiSignalPower;
    return this;
}

bool WifiButtonEntity::hasAuth() const
{
    return this->m_hasAuth;
}

WifiButtonEntity* WifiButtonEntity::hasAuth(const bool hasAuth)
{
    this->m_hasAuth = hasAuth;
    return this;
}

/* ###
    TextBoxEntity
    ### */

TextBoxEntity::TextBoxEntity(const EntityID& ID, const string& label, const string& content, bool isPassword)
: MenuEntity(MenuEntityType::TYPE_TEXT_BOX, ID, label)
{
    this->setContent(content);
    this->isPassword(isPassword);
}

TextBoxEntity::~TextBoxEntity()
{

}

string TextBoxEntity::getContent() const
{
    return this->m_content;
}

string* TextBoxEntity::getContentPointer()
{
    return &this->m_content;
}

TextBoxEntity* TextBoxEntity::setContent(const string& content)
{
    this->m_content = content;
    return this;
}

bool TextBoxEntity::isPassword() const
{
    return this->m_isPassword;
}

TextBoxEntity* TextBoxEntity::isPassword(const bool& isPassword)
{
    this->m_isPassword = isPassword;
    return this;
}
