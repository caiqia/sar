#ifndef DEF_ERRORVIEW
#define DEF_ERRORVIEW

#include "view.hpp"
#include "surface.hpp"
#include "text.hpp"
#include "image.hpp"
#include "font.hpp"
#include "../controllers/viewData.hpp"
#include "../controllers/controller.hpp"

#include <vector>
#include <map>
#include <string>


class View;



class ExecutionErrorView : public AbstractView
{
public:
    ExecutionErrorView(View* view);
    ~ExecutionErrorView();

    virtual ExecutionErrorView* render(const ViewData& viewData);
    virtual ExecutionErrorView* loadImages();
    virtual ExecutionErrorView* destroyImages();
    virtual ExecutionErrorView* calculateMeasures();

    RoundedRectangle* getWindowHeader() const;
    ExecutionErrorView* setWindowHeader(RoundedRectangle* windowheader);

    Text* getHeaderText() const;
    ExecutionErrorView* setHeaderText(Text* titleText);

    RoundedRectangle* getContentWindow() const;
    ExecutionErrorView* setContentWindow(RoundedRectangle* contentwindow);

    Text* getContent() const;
    ExecutionErrorView* setContent(Text* content);

    Text* getButtonContent() const;
    ExecutionErrorView* setButtonContent(Text* buttoncontent);

    RoundedRectangle* getButton() const;
    ExecutionErrorView* setButton(RoundedRectangle* button);

    Text* getInfo() const;
    ExecutionErrorView* setInfo(Text* info);


protected:

    RoundedRectangle* m_pWindowHeader = nullptr;

    Text* m_pHeaderText = nullptr;

    RoundedRectangle* m_pContentWindow = nullptr;

    Text* m_pContent = nullptr;

    RoundedRectangle* m_pButton = nullptr;

    Text* m_pButtonContent = nullptr;

    Text* m_info = nullptr;



};

class MajorErrorView : public AbstractView
{
public:
    MajorErrorView(View* view);
    ~MajorErrorView();

    virtual MajorErrorView* render(const ViewData& viewData);
    virtual MajorErrorView* loadImages();
    virtual MajorErrorView* destroyImages();

    Text* getInfoText() const;
    MajorErrorView* setInfoText(Text* infoText);

    Text* getTitleText() const;
    MajorErrorView* setTitleText(Text* titleText);

    Text* getErrorMessage() const;
    MajorErrorView* setErrorMessage(Text* ErrorMessage);


    MajorErrorView* setWindow(Rectangle* window);
    Rectangle* getWindow() const;



protected:

  Text* m_infoText = nullptr;

  Text* m_titleText = nullptr;

  Text* m_errormessage = nullptr;

  Rectangle* m_window = nullptr;


};
#endif
