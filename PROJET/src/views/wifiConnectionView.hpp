#ifndef DEF WIFICONNECTIONVIEW
#define DEF_WIFICONNECTIONVIEW

#include "abstractView.hpp"

class WifiConnectionView : public AbstractView
{
public:
    WifiConnectionView(View* view);
    ~WifiConnectionView();

    virtual WifiConnectionView* render(const ViewData& viewData);

    virtual WifiConnectionView* loadImages();
    virtual WifiConnectionView* destroyImages();

    virtual WifiConnectionView* calculateMeasures();

protected:
    Image* m_pBa

};

#endif
