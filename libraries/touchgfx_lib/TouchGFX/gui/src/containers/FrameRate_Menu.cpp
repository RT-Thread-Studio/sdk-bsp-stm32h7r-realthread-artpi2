#include <gui/containers/FrameRate_Menu.hpp>

FrameRate_Menu::FrameRate_Menu()
{

}

void FrameRate_Menu::initialize()
{
    FrameRate_MenuBase::initialize();
}

void FrameRate_Menu::updateShownFPS()
{
    Unicode::snprintf(fpsValueBuffer, FPSVALUE_SIZE, "%d", fps);
    fpsValue.invalidate();
}
