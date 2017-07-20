#include <SDL2/SDL.h>
#include <map>
#include <boost/filesystem.hpp>

#include "../controllers/controller.hpp"
#include "view.hpp"
#include "abstractView.hpp"
#include "homeView.hpp"
#include "visualKeyboardView.hpp"
#include "menuView.hpp"
#include "errorview.hpp"
#include "gameSelectView.hpp"
#include "gamepadProfileSelectView.hpp"
#include "firstLaunchView.hpp"
#include "popupWindowView.hpp"
#include "../controllers/homeController.hpp"
#include "../controllers/errorController.hpp"
#include "../entities/log.hpp"
#include "../entities/window.hpp"
#include "../entities/error.hpp"
#include "../entities/system.hpp"
#include "../entities/menuElements.hpp"
#include "../entities/error.hpp"

using namespace std;
namespace fs = boost::filesystem;


//Main View, acts as a controller to route between the views

//init
View::View(Controller* controller, Window* window)
{

    this->setController(controller);
    this->getController()->setView(this);
    this->setWindow(window);
    this->setRenderer(window->getRenderer());
    this->setUsableSpace();
    this->setOverlayGrey();
    this->loadLogos();


    Log::addEntry(1, "View sucessfully created");
    return;
}

View::~View()
{
    for(auto & ent : Color::nostalColorMap)
    {
        if(ent.second != nullptr)
        {
            delete ent.second;
        }
    }

    for(auto& ent : this->getViews())
    {
        if(ent.second != nullptr)
        {
            delete ent.second;
        }
    }

    Log::addEntry(1, "View deleted");
}

//calls the render function of the current view and then renders the render queue
void View::render()
{
    if(this->getController()->getViewData()["param"]["firstLaunch"] == "false")
    {
        switch(this->getController()->viewData.currentView)
        {
            case CurrentViews::HOME:
                {
                    if(this->getView(ViewsID::HOME_VIEW) != nullptr)
                    {
                        this->getView(ViewsID::FIRST_LAUCH_VIEW)->render(this->getController()->viewData);
                        //this->getView(ViewsID::HOME_VIEW)->render(this->getController()->viewData);
                    }
                    break;
                }
            case CurrentViews::GAME_SELECT:

                if(this->getView(ViewsID::GAME_SELECT_VIEW) != nullptr)
                {
                    this->getView(ViewsID::GAME_SELECT_VIEW)->render(this->getController()->viewData);
                }

                break;
        }
    }
    else
    {
        this->getView(ViewsID::FIRST_LAUCH_VIEW)->render(this->getController()->viewData);
    }

    //If we have an active overlay, we grey the background
    if(this->getController()->viewData.overlayLevel == OverlayLevel::MENU)
    {
        this->addToRender(this->getOverlayGrey());
    }

    if(this->getController()->viewData.menuActive)
    {
        if(this->getView(ViewsID::MENU_VIEW) != nullptr)
        {

            this->getView(ViewsID::MENU_VIEW)->render(this->getController()->viewData);
        }
    }

    if(this->getController()->viewData.overlayLevel == OverlayLevel::VISUAL_KEYBOARD)
    {
        this->addToRender(this->getOverlayGrey());

    }

    if(this->getController()->viewData.displayVisualKeyboard)
    {
        if(this->getView(ViewsID::VISUAL_KEYBOARD_VIEW) != nullptr)
        {
            this->getView(ViewsID::VISUAL_KEYBOARD_VIEW)->render(this->getController()->viewData);
        }
    }

    if(this->getController()->viewData.overlayLevel == OverlayLevel::POPUP_WINDOW)
    {
        this->addToRender(this->getOverlayGrey());
    }


    if(this->getController()->viewData.displayPopupWindow)
    {
        if(this->getView(ViewsID::POPUP_WINDOW_VIEW) != nullptr)
        {
            this->getView(ViewsID::POPUP_WINDOW_VIEW)->render(this->getController()->viewData);
        }
    }

    if(this->getController()->viewData.overlayLevel == OverlayLevel::INPUT_POPUP_WINDOW)
    {
        this->addToRender(this->getOverlayGrey());
    }


    if(this->getController()->viewData.displayInputPopupWindow)
    {
        if(this->getView(ViewsID::INPUT_POPUP_WINDOW_VIEW) != nullptr)
        {
            this->getView(ViewsID::INPUT_POPUP_WINDOW_VIEW)->render(this->getController()->viewData);
        }
    }

    if(this->getController()->viewData.overlayLevel == OverlayLevel::ERROR)
    {
        this->addToRender(this->getOverlayGrey());
    }

    if(this->getController()->viewData.executionerrorAffich == true)
    {

        this->getView(ViewsID::EXECUTION_ERROR_VIEW)->render(this->getController()->viewData);
    }

    if(this->getController()->viewData.majorerrorAffich == true)
    {
        this->getView(ViewsID::MAJOR_ERROR_VIEW)->render(this->getController()->viewData);

    }

    if(this->getController()->viewData.overlayLevel == OverlayLevel::GAMEPAD_PROFILE_SELECTION)
    {
        this->addToRender(this->getOverlayGrey());
    }

    if(this->getController()->viewData.displayGamepadProfileSelection == true)
    {
        this->getView(GAMEPAD_PROFILE_SELECT_VIEW)->render(this->getController()->viewData);
    }


    this->renderAll();
}

Controller* View::getController()
{
    return this->m_pController;
}

void View::setController(Controller* controller)
{
    this->m_pController = controller;
    return;
}

Window* View::getWindow()
{
    return this->m_pWindow;
}

void View::setWindow(Window * window)
{
    this->m_pWindow = window;
    return;
}

SDL_Renderer* View::getRenderer()
{
    return this->m_pRenderer;
}

void View::setRenderer(SDL_Renderer* renderer)
{
    this->m_pRenderer = renderer;
    return;
}

View* View::addView(const ViewsID& viewID, AbstractView* view)
{
    if(this->m_views.count(viewID) > 0)
    {
        if(this->m_views.at(viewID) != nullptr)
        {
            delete this->m_views.at(viewID);
        }
    }

    this->m_views[viewID] = view;

    return this;
}

AbstractView* View::getView(const ViewsID& viewID) const
{
    if(this->m_views.count(viewID) > 0)
    {
        return this->m_views.at(viewID);
    }

    return nullptr;
}

map <ViewsID, AbstractView*> View::getViews() const
{
    return this->m_views;
}

FirstLaunchView* View::getFirstLaunchView()
{
    return this->m_pFirstLaunchView;
}

View* View::setFirstLaunchView(FirstLaunchView* firstLaunchView)
{
    this->m_pFirstLaunchView = firstLaunchView;
    return this;
}


void View::addToRender(Renderable* renderable)
{
    this->m_toRender.push_back(renderable);
    return ;
}

//Renders all elements in the render queue
void View::renderAll()
{
    SDL_SetRenderDrawColor(this->getRenderer(), 255, 255, 255, 255);
    SDL_RenderClear(this->getRenderer());

    for(vector<Renderable*>::size_type i = 0; i != this->m_toRender.size(); i++)
    {
        this->m_toRender[i]->render();
    }

    this->m_toRender.clear();

    SDL_RenderPresent(this->getRenderer());

    return;
}

Image* View::getBackground(const int& system) const
{
    if(m_backgrounds.count(system) > 0)
    {
        return this->m_backgrounds.at(system);
    }
    return nullptr;
}

map<int, Image*> View::getBackgrounds() const
{
    return this->m_backgrounds;
}
View* View::addBackground(const int& system, const string& file)
{
    SDL_Renderer* renderer = this->getRenderer();
    int width = this->getWindow()->getWidth();
    int height = this->getWindow()->getHeight();

    Image* pImage = new Image(renderer, file, width, height, 0, 0);

    if(pImage != nullptr)
    {
        this->m_backgrounds.erase(system);
    }
    this->m_backgrounds.insert(pair<int, Image*>(system, pImage));
    return this;
}

Image* View::getLogo(const int& system) const
{
    if(this->m_logos.count(system) > 0)
    {
        return this->m_logos.at(system);
    }
    return nullptr;
}

map<int, Image*> View::getLogos() const
{
    return this->m_logos;
}

View* View::addLogo(const int& system, const string& file)
{
    SDL_Renderer* renderer = this->getRenderer();
    Image* pImage = new Image(renderer, file);

    if(pImage != nullptr)
    {
        this->m_logos.erase(system);
    }
    this->m_logos.insert(pair<int, Image*>(system, pImage));

    return this;
}

Image* View::getGreyedLogo(const int& system) const
{
    if(this->m_greyedLogos.count(system) > 0)
    {
        return this->m_greyedLogos.at(system);
    }
    return nullptr;
}

map<int, Image*> View::getGreyedLogos() const
{
    return this->m_greyedLogos;
}

View* View::addGreyedLogo(const int& system, const string& file)
{
    SDL_Renderer* renderer = this->getRenderer();
    Image* pImage = new Image(renderer, file);

    if(pImage != nullptr)
    {
        this->m_greyedLogos.erase(system);
    }
    this->m_greyedLogos.insert(pair<int, Image*>(system, pImage));

    return this;
}

View* View::loadLogos()
{
    for(const auto& ent : System::instances)
    {
        if(fs::exists(ent.second->getLogoFile()))
        {
            this->addLogo(ent.first, ent.second->getLogoFile());
        }
        else
        {
            Error::addError(Error(ErrorType::MAJOR, "Le fichier : " + ent.second->getLogoFile() + " est absent du système", "I-0100"));
        }

        if(fs::exists(ent.second->getGreyedLogoFile()))
        {
            this->addGreyedLogo(ent.first, ent.second->getGreyedLogoFile());
        }
        else
        {
            Error::addError(Error(ErrorType::MAJOR, "Le fichier : " + ent.second->getLogoFile() + " est absent du système", "I-0100"));
        }
    }

    return this;
}

View* View::deInitWindow()
{
    for(auto& ent : this->getViews())
    {
        if(ent.second != nullptr)
        {
            ent.second->destroyImages();
        }
    }

    for(auto& ent : this->getLogos())
    {
        if(ent.second != nullptr)
        {
            delete ent.second;
        }
    }
    this->m_logos.clear();

    for(auto& ent : this->getGreyedLogos())
    {
        if(ent.second != nullptr)
        {
            delete ent.second;
        }
    }
    this->m_greyedLogos.clear();
    if(this->m_overlayGrey != nullptr)
    {
        this->m_overlayGrey = nullptr;
    }

    this->getWindow()->deInitWindow();
    return this;
}

View* View::reInitWindow()
{
    this->getWindow()->reInitWindow();
    this->setRenderer(this->getWindow()->getRenderer());

    this->setOverlayGrey();


    this->loadLogos();


    for(auto& ent : this->getViews())
    {
        if(ent.second != nullptr)
        {
            ent.second->loadImages();
        }
    }

    return this;
}

View* View::setOverlayGrey()
{
    Rectangle* pOverlay = new Rectangle(this->getRenderer(), Color::getColorFromMap(NostalColors::GREY_THIRD_TRANSPARENT), this->getWindow()->getWidth(), this->getWindow()->getHeight(), 0, 0);
    this->m_overlayGrey = pOverlay;
    return this;
}

Rectangle* View::getOverlayGrey()
{

    return this->m_overlayGrey;
}

int View::getUsableWidth()
{
    return this->m_usableWidth;
}

int View::getUsableHeight()
{
    return this->m_usableHeight;
}

int View::getUsableXOrig()
{
    return this->m_usableXOrigin;
}

int View::getUsableYOrig()
{
    return this->m_usableYOrigin;
}

View* View::setUsableSpace()
{
    int windowWidth = this->getWindow()->getWidth();
    int windowHeight = this->getWindow()->getHeight();
    int usableHeight, usableWidth, usableXOrigin, usableYOrigin;

    if(this->getWindow()->getAspectRatio() == "4/3")
    {
        usableHeight = windowHeight;
        usableWidth = windowWidth;
        usableXOrigin = 0;
        usableYOrigin = 0;
    }
    else
    {
        usableHeight = windowHeight;
        usableWidth = windowWidth - (windowWidth / 4);
        usableXOrigin = windowWidth / 8;
        usableYOrigin = 0;
    }

    this->m_usableHeight = usableHeight;
    this->m_usableWidth = usableWidth;
    this->m_usableXOrigin = usableXOrigin;
    this->m_usableYOrigin = usableYOrigin;

    return this;
}
