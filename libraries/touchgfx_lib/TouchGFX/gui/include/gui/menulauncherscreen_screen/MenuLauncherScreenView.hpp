#ifndef MENULAUNCHERSCREENVIEW_HPP
#define MENULAUNCHERSCREENVIEW_HPP

#include <gui_generated/menulauncherscreen_screen/MenuLauncherScreenViewBase.hpp>
#include <gui/menulauncherscreen_screen/MenuLauncherScreenPresenter.hpp>

class MenuLauncherScreenView : public MenuLauncherScreenViewBase
{
public:
    MenuLauncherScreenView();
    virtual ~MenuLauncherScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    virtual void sphereWheelAnimationStarted();
    virtual void sphereWheelAnimationEndded(uint16_t value);
    virtual void sphereWheelItemSelected(uint16_t value);

protected:
};

#endif // MENULAUNCHERSCREENVIEW_HPP
