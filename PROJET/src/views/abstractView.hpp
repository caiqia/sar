#ifndef DEF_ABSTRACTVIEW
#define DEF_ABSTRACTVIEW

#include "view.hpp"
#include "surface.hpp"
#include "text.hpp"
#include "image.hpp"
#include "font.hpp"
#include "wrappedText.hpp"
#include "../controllers/viewData.hpp"
#include "../controllers/controller.hpp"

#include <vector>
#include <map>
#include <string>

class AbstractView
{
public:
    AbstractView(View* view, ViewsID viewID);
    ~AbstractView();

    virtual AbstractView* render(const ViewData& viewData) = 0;

    virtual AbstractView* loadImages() = 0;

    virtual AbstractView* destroyImages() = 0;

    virtual AbstractView* calculateMeasures();

     AbstractView* init();

    AbstractView* deInit();

    AbstractView* setView(View* view);
    View* getView() const;

    AbstractView* addMeasure(const std::string& label,const int& value);
    int getMeasure(const std::string& label) const;
    std::map <std::string, int> getMeasures() const;

    AbstractView* setViewID(const ViewsID& viewID);
    ViewsID getViewID() const;

protected:
    View* m_pView;

    ViewsID m_viewID;

    std::map <std::string, int> m_measures;
};

#endif
