#ifndef FRAMERATE_MENU_HPP
#define FRAMERATE_MENU_HPP

#include <gui_generated/containers/FrameRate_MenuBase.hpp>
#include <gui/common/FrameRate.hpp>

class FrameRate_Menu : public FrameRate_MenuBase, public FrameRate
{
public:
    FrameRate_Menu();
    virtual ~FrameRate_Menu() {}

    virtual void initialize();
protected:
    virtual void updateShownFPS();
};

#endif // FRAMERATE_MENU_HPP
