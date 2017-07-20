#ifndef DEF_HOMEVIEW
#define DEF_HOMEVIEW

#include "abstractView.hpp"

class View;

class HomeView : public AbstractView
{
public:
    HomeView(View* view);
    ~HomeView();

    HomeView* render(const ViewData& viewData);

    HomeView* loadImages();
    HomeView* destroyImages();

    HomeView* setGrid(const ViewData& viewData);

    HomeView* calculateMeasures();

    Image* getBackground() const;
    HomeView* setBackground(Image* background);

    RoundedRectangle* getContentBox() const;
    HomeView* setContentBox(RoundedRectangle* contentBox);

    Text* getTitleText() const;
    HomeView* setTitleText(Text* titleText);

    RoundedRectangle* getSelector() const;
    HomeView* setSelector(RoundedRectangle* selector);

    Text* getSubText(int system) const;
    std::map<int, Text*> getSubTexts() const;
    HomeView* addSubText(int system, Text* subText);

    Image* getDownArrow() const;
    HomeView* setDownArrow(Image* downArrow);

    Image* getUpArrow() const;
    HomeView* setUpArrow(Image* upArrow);

    Image* getUpArrowGreyed() const;
    HomeView* setUpArrowGreyed(Image* upArrowGreyed);

    Image* getDownArrowGreyed() const;
    HomeView* setDownArrowGreyed(Image* downArrowGreyed);

private:
    View* m_pView;

    Image* m_pBackground = nullptr;

    RoundedRectangle* m_pContentBox = nullptr;

    Text* m_pTitleText = nullptr;

    RoundedRectangle* m_pSelector = nullptr;

    std::map<int, Text*> m_pSubTexts;

    Image* m_pDownArrow = nullptr;
    Image* m_pUpArrow = nullptr;

    Image* m_pDownArrowGreyed = nullptr;
    Image* m_pUpArrowGreyed = nullptr;

};

#endif
