#include <gui/menulauncherscreen_screen/MenuLauncherScreenView.hpp>
#include <gui/menulauncherscreen_screen/MenuLauncherScreenPresenter.hpp>

MenuLauncherScreenPresenter::MenuLauncherScreenPresenter(MenuLauncherScreenView& v)
    : view(v)
{

}

void MenuLauncherScreenPresenter::activate()
{

}

void MenuLauncherScreenPresenter::deactivate()
{

}

void MenuLauncherScreenPresenter::setSelectedDemoNumber(int value)
{
    model->setSelectedDemoNumber(value);
}

int MenuLauncherScreenPresenter::getSelectedDemoNumber()
{
    return model->getSelectedDemoNumber();
}
