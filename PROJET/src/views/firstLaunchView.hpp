#ifndef DEF_FIRSTLAUNCHVIEW
#define DEF_FIRSTLAUNCHVIEW

#include "surface.hpp"
#include "text.hpp"
#include <map>
#include <string>
#include "abstractView.hpp"

class View;

class FirstLaunchView: public AbstractView
{
public:
    FirstLaunchView(View* view);
    ~FirstLaunchView();

    virtual FirstLaunchView* render(const ViewData& viewData);

    virtual FirstLaunchView* loadImages();
    virtual FirstLaunchView* destroyImages();
    virtual FirstLaunchView* calculateMeasures();

    FirstLaunchView* setBackGround(Image* background);
    Image* getBackGround() const;

    FirstLaunchView* setExplanationText(WrappedText* explanationtext);
    WrappedText* getExplanationText() const;

    FirstLaunchView* setTitleText(Text* titletext);
    Text* getTitleText() const;

protected:

    Image* m_pBackground = nullptr;
    Text* m_pTitleText = nullptr;
    WrappedText* m_pExplanationText = nullptr;

};

#endif
