#ifndef DEF_STARTUPLOADINGVIEW
#define DEF_STARTUPLOADINGVIEW

#include "abstractView.hpp"

class StartupLoadingView : public AbstractView
{
public:
    StartupLoadingView(View* view);
    ~StartupLoadingView();

    virtual StartupLoadingView* render(const ViewData& viewData);

    virtual StartupLoadingView* loadImages();

    virtual StartupLoadingView* destroyImages();

    virtual StartupLoadingView* calculateMeasures();

    Image* getBackground() const;
    StartupLoadingView* setBackground(Image* background);

    Image* getLogo() const;
    StartupLoadingView* setLogo(Image* logo);

    Text* getLoadingText() const;
    StartupLoadingView* setLoadingText(Text* loadingText);

    RoundedRectangle* getProgressBarBackground() const;
    StartupLoadingView* setProgressBarBackground(RoundedRectangle* progressBarBackground);

    RoundedRectangle* getProgressBar() const;
    StartupLoadingView* setProgressBar(RoundedRectangle* progressBar);

protected:
    Image* m_pBackground = nullptr;
    Image* m_pLogo = nullptr;
    Text* m_pLoadingText = nullptr;

    RoundedRectangle* m_pProgressBarBackground = nullptr;
    RoundedRectangle* m_pProgressBar = nullptr;
};

#endif
