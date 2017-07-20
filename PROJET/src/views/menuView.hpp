#ifndef DEF_MENUVIEW
#define DEF_MENUVIEW

#include "abstractView.hpp"
#include "../entities/menuElements.hpp"

class View;

class MenuView : public AbstractView
{
public:
    MenuView(View* view);
    ~MenuView();

    virtual MenuView* render(const ViewData& viewData);
    virtual MenuView* renderMenuElements(const ViewData& viewData);

    virtual MenuView* loadImages();
    MenuView* loadElements();
    virtual MenuView* destroyImages();

    MenuView* calculateMeasures();

    RoundedRectangle* getBackground() const;
    MenuView* setBackground(RoundedRectangle* background);

    RoundedRectangle* getHeader() const;
    MenuView* setHeader(RoundedRectangle* header);

    Text* getHeaderText() const;
    MenuView* setHeaderText(Text* headerText);

    Image* getUpperArrow() const;
    MenuView* setUpperArrow(Image* upperArrow);

    Image* getLowerArrow() const;
    MenuView* setLowerArrow(Image* lowerArrow);

    Rectangle* getSelector() const;
    MenuView* setSelector(Rectangle* selector);

    MenuElement* getMenuElement(const CurrentMenu& menu, const int& elementID) const;
    std::map<int, MenuElement*> getMenuElements(const CurrentMenu& menu);
    std::map<CurrentMenu, std::map<int, MenuElement*>> getMenuElements() const;
    MenuView* addMenuElement(const CurrentMenu& menu, const int& elementID, MenuElement* menuElement);

    int getNumberElements(const ViewData& viewData) const;
    int getPage(const ViewData& viewData) const;
    bool isFirstPage(const ViewData& viewData) const;
    bool isLastPage(const ViewData& viewData) const;

protected:
    RoundedRectangle* m_pBackground = nullptr;
    RoundedRectangle* m_pHeader = nullptr;

    Text* m_pHeaderText = nullptr;

    Image* m_pUpperArrow = nullptr;
    Image* m_pLowerArrow = nullptr;

    Rectangle* m_pSelector = nullptr;

    std::map<CurrentMenu, std::map<int, MenuElement*>> m_menuElements;

};

#endif
