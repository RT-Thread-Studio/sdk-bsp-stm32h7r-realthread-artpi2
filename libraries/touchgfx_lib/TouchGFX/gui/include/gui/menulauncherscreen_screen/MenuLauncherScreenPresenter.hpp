#ifndef MENULAUNCHERSCREENPRESENTER_HPP
#define MENULAUNCHERSCREENPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class MenuLauncherScreenView;

class MenuLauncherScreenPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    MenuLauncherScreenPresenter(MenuLauncherScreenView& v);

    /**
     * The activate function is called automatically when this screen is "switched in"
     * (ie. made active). Initialization logic can be placed here.
     */
    virtual void activate();

    /**
     * The deactivate function is called automatically when this screen is "switched out"
     * (ie. made inactive). Teardown functionality can be placed here.
     */
    virtual void deactivate();

    virtual ~MenuLauncherScreenPresenter() {};

    void setSelectedDemoNumber(int value);
    int getSelectedDemoNumber();

private:
    MenuLauncherScreenPresenter();

    MenuLauncherScreenView& view;
};

#endif // MENULAUNCHERSCREENPRESENTER_HPP
