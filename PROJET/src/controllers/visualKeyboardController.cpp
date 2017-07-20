#include "visualKeyboardController.hpp"
#include "controller.hpp"

#include <iostream>

using namespace std;

VisualKeyboardController* VisualKeyboardController::m_pThis = nullptr;

map<Keys, int> VisualKeyboardController::keyNumberResolver =
{
    {Keys::KEY1, 1},
    {Keys::KEY2, 2},
    {Keys::KEY3, 3},
    {Keys::KEY4, 4},
    {Keys::KEY5, 5},
    {Keys::KEY6, 6},
    {Keys::KEY7, 7},
    {Keys::KEY8, 8},
    {Keys::KEY9, 9},
    {Keys::KEY10, 10},
    {Keys::KEY11, 11},
    {Keys::KEY12, 12},
    {Keys::KEY13, 13},
    {Keys::KEY14, 14},
    {Keys::KEY15, 15},
    {Keys::KEY16, 16},
    {Keys::KEY17, 17},
    {Keys::KEY18, 18},
    {Keys::KEY19, 19},
    {Keys::KEY20, 20},
    {Keys::KEY21, 21},
    {Keys::KEY22, 22},
    {Keys::KEY23, 23},
    {Keys::KEY24, 24},
    {Keys::KEY25, 25},
    {Keys::KEY26, 26},
    {Keys::KEY27, 27},
    {Keys::KEY28, 28},
    {Keys::KEY29, 29},
    {Keys::KEY30, 30},
    {Keys::KEY31, 31},
    {Keys::KEY32, 32},
    {Keys::KEY33, 33}
};

map<int, Keys> VisualKeyboardController::keyNumberReverser =
{
    {1, Keys::KEY1},
    {2, Keys::KEY2},
    {3, Keys::KEY3},
    {4, Keys::KEY4},
    {5, Keys::KEY5},
    {6, Keys::KEY6},
    {7, Keys::KEY7},
    {8, Keys::KEY8},
    {9, Keys::KEY9},
    {10, Keys::KEY10},
    {11, Keys::KEY11},
    {12, Keys::KEY12},
    {13, Keys::KEY13},
    {14, Keys::KEY14},
    {15, Keys::KEY15},
    {16, Keys::KEY16},
    {17, Keys::KEY17},
    {18, Keys::KEY18},
    {19, Keys::KEY19},
    {20, Keys::KEY20},
    {21, Keys::KEY21},
    {22, Keys::KEY22},
    {23, Keys::KEY23},
    {24, Keys::KEY24},
    {25, Keys::KEY25},
    {26, Keys::KEY26},
    {27, Keys::KEY27},
    {28, Keys::KEY28},
    {29, Keys::KEY29},
    {30, Keys::KEY30},
    {31, Keys::KEY31},
    {32, Keys::KEY32},
    {33, Keys::KEY33}
};

map<KeyboardPannel, map <Keys, string>> VisualKeyboardController::keyResolver =
{
    {
        KeyboardPannel::MAIN,
            {
                {Keys::KEY1, "a"},
                {Keys::KEY2, "z"},
                {Keys::KEY3, "e"},
                {Keys::KEY4, "r"},
                {Keys::KEY5, "t"},
                {Keys::KEY6, "y"},
                {Keys::KEY7, "u"},
                {Keys::KEY8, "i"},
                {Keys::KEY9, "o"},
                {Keys::KEY10, "p"},
                {Keys::KEY11, "BACK"},
                {Keys::KEY12, "q"},
                {Keys::KEY13, "s"},
                {Keys::KEY14, "d"},
                {Keys::KEY15, "f"},
                {Keys::KEY16, "g"},
                {Keys::KEY17, "h"},
                {Keys::KEY18, "j"},
                {Keys::KEY19, "k"},
                {Keys::KEY20, "l"},
                {Keys::KEY21, "m"},
                {Keys::KEY22, "VALIDATE"},
                {Keys::KEY23, "SHIFT"},
                {Keys::KEY24, "NUMBER"},
                {Keys::KEY25, "w"},
                {Keys::KEY26, "x"},
                {Keys::KEY27, "c"},
                {Keys::KEY28, "v"},
                {Keys::KEY29, "b"},
                {Keys::KEY30, "n"},
                {Keys::KEY31, "SPACE"},
                {Keys::KEY32, "ACCENTUATED"},
                {Keys::KEY33, "VALIDATE"}
            }
    },
    {
        KeyboardPannel::MAIN_SHIFT,
            {
                {Keys::KEY1, "A"},
                {Keys::KEY2, "Z"},
                {Keys::KEY3, "E"},
                {Keys::KEY4, "R"},
                {Keys::KEY5, "T"},
                {Keys::KEY6, "Y"},
                {Keys::KEY7, "U"},
                {Keys::KEY8, "I"},
                {Keys::KEY9, "O"},
                {Keys::KEY10, "P"},
                {Keys::KEY11, "BACK"},
                {Keys::KEY12, "Q"},
                {Keys::KEY13, "S"},
                {Keys::KEY14, "D"},
                {Keys::KEY15, "F"},
                {Keys::KEY16, "G"},
                {Keys::KEY17, "H"},
                {Keys::KEY18, "J"},
                {Keys::KEY19, "K"},
                {Keys::KEY20, "L"},
                {Keys::KEY21, "M"},
                {Keys::KEY22, "VALIDATE"},
                {Keys::KEY23, "SHIFT"},
                {Keys::KEY24, "NUMBER"},
                {Keys::KEY25, "W"},
                {Keys::KEY26, "X"},
                {Keys::KEY27, "C"},
                {Keys::KEY28, "V"},
                {Keys::KEY29, "B"},
                {Keys::KEY30, "N"},
                {Keys::KEY31, "SPACE"},
                {Keys::KEY32, "ACCENTUATED"},
                {Keys::KEY33, "VALIDATE"}
            }
    },
    {
        KeyboardPannel::ACCENTUATED,
            {
                {Keys::KEY1, "à"},
                {Keys::KEY2, "á"},
                {Keys::KEY3, "â"},
                {Keys::KEY4, "ã"},
                {Keys::KEY5, "ä"},
                {Keys::KEY6, "å"},
                {Keys::KEY7, "æ"},
                {Keys::KEY8, "ç"},
                {Keys::KEY9, "è"},
                {Keys::KEY10, "é"},
                {Keys::KEY11, "BACK"},
                {Keys::KEY12, "ê"},
                {Keys::KEY13, "ë"},
                {Keys::KEY14, "ì"},
                {Keys::KEY15, "í"},
                {Keys::KEY16, "î"},
                {Keys::KEY17, "ï"},
                {Keys::KEY18, "ñ"},
                {Keys::KEY19, "ò"},
                {Keys::KEY20, "ó"},
                {Keys::KEY21, "ô"},
                {Keys::KEY22, "VALIDATE"},
                {Keys::KEY23, "SHIFT"},
                {Keys::KEY24, "õ"},
                {Keys::KEY25, "ö"},
                {Keys::KEY26, "ù"},
                {Keys::KEY27, "ú"},
                {Keys::KEY28, "û"},
                {Keys::KEY29, "ü"},
                {Keys::KEY30, "ý"},
                {Keys::KEY31, "œ"},
                {Keys::KEY32, "NORMAL"},
                {Keys::KEY33, "VALIDATE"}
            }
    },
    {
        KeyboardPannel::ACCENTUATED_SHIFT,
            {
                {Keys::KEY1, "À"},
                {Keys::KEY2, "Á"},
                {Keys::KEY3, "Â"},
                {Keys::KEY4, "Ã"},
                {Keys::KEY5, "Ä"},
                {Keys::KEY6, "Å"},
                {Keys::KEY7, "Æ"},
                {Keys::KEY8, "Ç"},
                {Keys::KEY9, "È"},
                {Keys::KEY10, "É"},
                {Keys::KEY11, "BACK"},
                {Keys::KEY12, "Ê"},
                {Keys::KEY13, "Ë"},
                {Keys::KEY14, "Ì"},
                {Keys::KEY15, "Í"},
                {Keys::KEY16, "Î"},
                {Keys::KEY17, "Ï"},
                {Keys::KEY18, "Ñ"},
                {Keys::KEY19, "Ò"},
                {Keys::KEY20, "Ó"},
                {Keys::KEY21, "Ô"},
                {Keys::KEY22, "VALIDATE"},
                {Keys::KEY23, "SHIFT"},
                {Keys::KEY24, "Õ"},
                {Keys::KEY25, "Ö"},
                {Keys::KEY26, "Ù"},
                {Keys::KEY27, "Ú"},
                {Keys::KEY28, "Û"},
                {Keys::KEY29, "Ü"},
                {Keys::KEY30, "Ý"},
                {Keys::KEY31, "Œ"},
                {Keys::KEY32, "NORMAL"},
                {Keys::KEY33, "VALIDATE"}
            }
    },
    {
        KeyboardPannel::SPECIALCHARS,
            {
                {Keys::KEY1, "1"},
                {Keys::KEY2, "2"},
                {Keys::KEY3, "3"},
                {Keys::KEY4, "4"},
                {Keys::KEY5, "5"},
                {Keys::KEY6, "6"},
                {Keys::KEY7, "7"},
                {Keys::KEY8, "8"},
                {Keys::KEY9, "9"},
                {Keys::KEY10, "0"},
                {Keys::KEY11, "BACK"},
                {Keys::KEY12, "@"},
                {Keys::KEY13, "#"},
                {Keys::KEY14, "*"},
                {Keys::KEY15, "%"},
                {Keys::KEY16, "&"},
                {Keys::KEY17, "-"},
                {Keys::KEY18, "+"},
                {Keys::KEY19, "("},
                {Keys::KEY20, ")"},
                {Keys::KEY21, "/"},
                {Keys::KEY22, "VALIDATE"},
                {Keys::KEY23, "SPECIAL"},
                {Keys::KEY24, "NORMAL"},
                {Keys::KEY25, "\""},
                {Keys::KEY26, "'"},
                {Keys::KEY27, ","},
                {Keys::KEY28, ";"},
                {Keys::KEY29, "."},
                {Keys::KEY30, ":"},
                {Keys::KEY31, "!"},
                {Keys::KEY32, "?"},
                {Keys::KEY33, "VALIDATE"}
            }
    },
    {
        KeyboardPannel::SPECIALCHARS_SHIFT,
            {
                {Keys::KEY1, "~"},
                {Keys::KEY2, "`"},
                {Keys::KEY3, "|"},
                {Keys::KEY4, "·"},
                {Keys::KEY5, "√"},
                {Keys::KEY6, "π"},
                {Keys::KEY7, "÷"},
                {Keys::KEY8, "×"},
                {Keys::KEY9, "<"},
                {Keys::KEY10, ">"},
                {Keys::KEY11, "BACK"},
                {Keys::KEY12, "€"},
                {Keys::KEY13, "£"},
                {Keys::KEY14, "¥"},
                {Keys::KEY15, "$"},
                {Keys::KEY16, "¢"},
                {Keys::KEY17, "ˆ"},
                {Keys::KEY18, "="},
                {Keys::KEY19, "{"},
                {Keys::KEY20, "}"},
                {Keys::KEY21, "\\"},
                {Keys::KEY22, "VALIDATE"},
                {Keys::KEY23, "NUMBER"},
                {Keys::KEY24, "NORMAL"},
                {Keys::KEY25, "_"},
                {Keys::KEY26, "©"},
                {Keys::KEY27, "®"},
                {Keys::KEY28, "±"},
                {Keys::KEY29, "«"},
                {Keys::KEY30, "»"},
                {Keys::KEY31, "["},
                {Keys::KEY32, "]"},
                {Keys::KEY33, "VALIDATE"}
            }
    }
};

VisualKeyboardController::VisualKeyboardController(Controller* controller)
: AbstractController(controller, VISUAL_KEYBOARD_CONTROLLER)
{
    VisualKeyboardController::m_pThis = this;
    this->getController()->viewData.keyMap.clear();
    this->getController()->viewData.keyMap = this->keyResolver;

    for(const auto& ent1 : this->keyResolver)
    {
        for(const auto& ent2 : ent1.second)
        {
            for(const auto& ent3 : ent1.second)
            {
                if(ent2.first != ent3.first && ent2.second == ent3.second && !isDoublerKey(ent1.first, ent2.first))
                {
                    this->addDoubledKey(ent1.first, ent2.first, ent3.first);
                }
            }
        }
    }

}
VisualKeyboardController::~VisualKeyboardController()
{

}

VisualKeyboardController* VisualKeyboardController::control()
{
    AbstractController::control();

    if(this->isDisplayed())
    {
        this->getController()->viewData.displayVisualKeyboard = true;
        this->getController()->setOverlayLevel(OverlayLevel::VISUAL_KEYBOARD);

        this->getController()->viewData.currentKey = this->getCurrentSelectedKey();
        this->getController()->viewData.currentPannel = this->getCurrentPannel();

        this->getController()->viewData.visualKeyboardInput = this->getCurrentInput();
    }
    else
    {
        this->getController()->viewData.displayVisualKeyboard = false;
    }
    return this;
}

VisualKeyboardController* VisualKeyboardController::inputControl()
{
    AbstractController::inputControl();

    for(auto& ent : this->getController()->getAdvancedControls())
    {
        if(this->getController()->getGamepadControl(ent.first, GamepadActions::B))
        {
            this->pushCurrentKey();
        }
        else if(this->getController()->getGamepadControl(ent.first, GamepadActions::RIGHT))
        {
            this->selectNextKey(this->getCurrentSelectedKey());
        }
        else if(this->getController()->getGamepadControl(ent.first, GamepadActions::LEFT))
        {
            this->selectPreviousKey(this->getCurrentSelectedKey());
        }
        else if(this->getController()->getGamepadControl(ent.first, GamepadActions::UP))
        {
            this->selectPreviousRow(this->getCurrentSelectedKey());
        }
        else if(this->getController()->getGamepadControl(ent.first, GamepadActions::DOWN))
        {
            this->selectNextRow(this->getCurrentSelectedKey());
        }

    }
    return this;
}

VisualKeyboardController* VisualKeyboardController::pushCurrentKey()
{
    string currentKey = VisualKeyboardController::keyResolver[this->getCurrentPannel()][this->getCurrentSelectedKey()];
    if(currentKey == "VALIDATE")
    {
        this->validate();
    }
    else if(currentKey == "BACK")
    {
        this->removeLastCharacter();
    }
    else if(currentKey == "SHIFT")
    {
        switch(this->getCurrentPannel())
        {
            case KeyboardPannel::MAIN :
                this->setCurrentPannel(KeyboardPannel::MAIN_SHIFT);
                this->getController()->viewData.shiftActive = true;
                break;
            case KeyboardPannel::MAIN_SHIFT :
                this->setCurrentPannel(KeyboardPannel::MAIN);
                this->getController()->viewData.shiftActive = false;
                break;
            case KeyboardPannel::ACCENTUATED :
                this->setCurrentPannel(KeyboardPannel::ACCENTUATED_SHIFT);
                this->getController()->viewData.shiftActive = true;
                break;
            case KeyboardPannel::ACCENTUATED_SHIFT :
                this->setCurrentPannel(KeyboardPannel::ACCENTUATED);
                this->getController()->viewData.shiftActive = false;
                break;
            case KeyboardPannel::SPECIALCHARS :
                this->setCurrentPannel(KeyboardPannel::SPECIALCHARS_SHIFT);
                this->getController()->viewData.shiftActive = true;
                break;
            case KeyboardPannel::SPECIALCHARS_SHIFT :
                this->setCurrentPannel(KeyboardPannel::SPECIALCHARS);
                this->getController()->viewData.shiftActive = false;
                break;
        }
    }
    else if(currentKey == "NUMBER")
    {
        this->setCurrentPannel(KeyboardPannel::SPECIALCHARS);
        this->getController()->viewData.shiftActive = false;
    }
    else if(currentKey == "SPECIAL")
    {
        this->setCurrentPannel(KeyboardPannel::SPECIALCHARS_SHIFT);
        this->getController()->viewData.shiftActive = false;
    }
    else if(currentKey == "ACCENTUATED")
    {
        this->setCurrentPannel(KeyboardPannel::ACCENTUATED);
        this->getController()->viewData.shiftActive = false;
    }
    else if(currentKey == "NORMAL")
    {
        this->setCurrentPannel(KeyboardPannel::MAIN);
        this->getController()->viewData.shiftActive = false;
    }
    else
    {
        this->addCurrentSelectedCharacter();
    }
}

VisualKeyboardController* VisualKeyboardController::addCurrentSelectedCharacter()
{
    string currentKey = VisualKeyboardController::keyResolver[this->getCurrentPannel()][this->getCurrentSelectedKey()];
    if(currentKey == "SPACE")
    {
        currentKey = " ";
    }

    this->m_currentInput = this->m_currentInput + currentKey;
    return this;
}

VisualKeyboardController* VisualKeyboardController::removeLastCharacter()
{
    if(this->m_currentInput.size() > 0)
    {
        this->m_currentInput.pop_back();
    }

    return this;
}

VisualKeyboardController* VisualKeyboardController::validate()
{
    this->m_stringToUpdate->assign(this->m_currentInput);
    this->setCurrentPannel(KeyboardPannel::MAIN);
    this->setCurrentSelectedKey(Keys::KEYFIRST);
    this->getController()->viewData.shiftActive = false;
    this->setDisplayed(false);
}

void VisualKeyboardController::askForInput(string* stringToReturn)
{
    VisualKeyboardController::m_pThis->m_stringToUpdate = stringToReturn;
    VisualKeyboardController::m_pThis->m_currentInput = *stringToReturn;
    VisualKeyboardController::m_pThis->setDisplayed(true);
    return;
}

VisualKeyboardController* VisualKeyboardController::selectNextKey(const Keys& keyFrom)
{

    if(this->isDoublerKey(this->getCurrentPannel(), keyFrom))
    {
        this->selectNextKey(this->getDoubledKey(this->getCurrentPannel(), keyFrom));
        return this;
    }

    Keys currentKey = keyFrom;

    Keys newSelectedKey;

    if(currentKey != Keys::KEYLAST)
    {
        newSelectedKey = VisualKeyboardController::keyNumberReverser[VisualKeyboardController::keyNumberResolver[currentKey] + 1];
    }
    else
    {
        newSelectedKey = VisualKeyboardController::keyNumberReverser[Keys::KEYFIRST];
    }
    this->setCurrentSelectedKey(newSelectedKey);

    return this;
}

VisualKeyboardController* VisualKeyboardController::selectPreviousKey(const Keys& keyFrom)
{

    if(this->isDoublerKey(this->getCurrentPannel(), keyFrom))
    {
        this->selectPreviousKey(this->getDoubledKey(this->getCurrentPannel(), keyFrom));
        return this;
    }

    Keys currentKey = keyFrom;

    Keys newSelectedKey;

    if(currentKey != Keys::KEYFIRST)
    {
        newSelectedKey = VisualKeyboardController::keyNumberReverser[VisualKeyboardController::keyNumberResolver[currentKey] - 1];
    }
    else
    {
        newSelectedKey = VisualKeyboardController::keyNumberReverser[Keys::KEYLAST];
    }
    this->setCurrentSelectedKey(newSelectedKey);

    return this;
}

VisualKeyboardController* VisualKeyboardController::selectNextRow(const Keys& keyFrom)
{
    if(this->isDoublerKey(this->getCurrentPannel(), keyFrom))
    {
        this->selectNextRow(this->getDoubledKey(this->getCurrentPannel(), keyFrom));
        return this;
    }

    Keys currentKey = keyFrom;

    Keys newSelectedKey;

    if(VisualKeyboardController::keyNumberResolver[currentKey] + 11 <= Keys::KEYLAST)
    {
        newSelectedKey = VisualKeyboardController::keyNumberReverser[VisualKeyboardController::keyNumberResolver[currentKey] + 11];
    }
    else
    {
        int keyDiff = (VisualKeyboardController::keyNumberResolver[currentKey] + 11) - Keys::KEYLAST;
        newSelectedKey = VisualKeyboardController::keyNumberReverser[Keys::KEYFIRST + keyDiff - 1];
    }

    this->setCurrentSelectedKey(newSelectedKey);

    return this;
}

VisualKeyboardController* VisualKeyboardController::selectPreviousRow(const Keys& keyFrom)
{
    if(this->isDoublerKey(this->getCurrentPannel(), keyFrom))
    {
        this->selectPreviousRow(this->getDoubledKey(this->getCurrentPannel(), keyFrom));
        return this;
    }

    Keys currentKey = keyFrom;

    Keys newSelectedKey;

    if(VisualKeyboardController::keyNumberResolver[currentKey] - 11 >= Keys::KEYFIRST)
    {
        newSelectedKey = VisualKeyboardController::keyNumberReverser[VisualKeyboardController::keyNumberResolver[currentKey] - 11];
    }
    else
    {
        int keyDiff = (11 - VisualKeyboardController::keyNumberResolver[currentKey]) - Keys::KEYFIRST;
        newSelectedKey = VisualKeyboardController::keyNumberReverser[KEYLAST - keyDiff - 1];
    }

    this->setCurrentSelectedKey(newSelectedKey);

    return this;
}

 bool VisualKeyboardController::isDisplayed()
 {
     return this->m_displayed;
 }

 VisualKeyboardController* VisualKeyboardController::setDisplayed(bool isDisplayed)
 {
     this->m_displayed = isDisplayed;
     return this;
 }

 Keys VisualKeyboardController::getCurrentSelectedKey()
 {
     return this->m_selectedKey;
 }

 VisualKeyboardController* VisualKeyboardController::setCurrentSelectedKey(Keys key)
 {
     this->m_selectedKey = key;
     return this;
 }

 KeyboardPannel VisualKeyboardController::getCurrentPannel()
 {
    return this->m_currentPannel;
 }

 VisualKeyboardController* VisualKeyboardController::setCurrentPannel(KeyboardPannel pannel)
 {
     this->m_currentPannel = pannel;
     return this;
 }

 string VisualKeyboardController::getCurrentInput()
 {
     return this->m_currentInput;
 }

 VisualKeyboardController* VisualKeyboardController::addDoubledKey(const KeyboardPannel& pannel, const Keys& key1, const Keys& key2)
 {
     this->m_doubledKeys[pannel][key1] = key2;
     this->m_doubledKeysReverser[pannel][key2] = key1;
     return this;
 }

 Keys VisualKeyboardController::getDoubledKey(const KeyboardPannel& pannel, const Keys& key) const
 {
     if(this->m_doubledKeys.count(pannel) > 0)
     {
         if(this->m_doubledKeys.at(pannel).count(key) > 0)
         {
             return this->m_doubledKeys.at(pannel).at(key);
         }
     }

     if(this->m_doubledKeysReverser.count(pannel) > 0)
     {
         if(this->m_doubledKeysReverser.at(pannel).count(key) > 0)
         {
             return this->m_doubledKeysReverser.at(pannel).at(key);
         }
     }
     return Keys::KEYNULL;
 }

 bool VisualKeyboardController::isDoublerKey(const KeyboardPannel& pannel, const Keys& key) const
 {
     if(this->m_doubledKeysReverser.count(pannel) > 0)
     {
         if(this->m_doubledKeysReverser.at(pannel).count(key) > 0)
         {
             return true;
         }
     }
     return false;
 }

 bool VisualKeyboardController::isDoubledKey(const KeyboardPannel& pannel, const Keys& key) const
 {
     if(this->m_doubledKeys.count(pannel) > 0)
     {
         if(this->m_doubledKeys.at(pannel).count(key) > 0)
         {
             return true;
         }
     }
     return false;
 }
