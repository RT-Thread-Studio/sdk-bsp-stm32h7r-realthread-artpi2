#ifndef MenuView_HPP
#define MenuView_HPP

#include <gui_generated/menu_screen/MenuViewBase.hpp>
#include <gui/menu_screen/MenuPresenter.hpp>

class MenuView : public MenuViewBase
{
public:
    MenuView();
    virtual ~MenuView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
    virtual void handleTickEvent();

    // Functions created as actions in Designer
    virtual void startWheelAnimation();
    virtual void changeScreenTo(uint8_t selectedDemoIndex);
    virtual void changePreviewWheel();

protected:

    float wheelAngles[MainWheel::NUMBER_OF_WHEEL_ELEMENTS];
    uint16_t convertTextIdToBitmapId(const uint32_t textId);
};

#endif // MenuView_HPP
