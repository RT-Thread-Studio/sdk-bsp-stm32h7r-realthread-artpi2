#ifndef MAINWHEEL_HPP
#define MAINWHEEL_HPP

#include <gui_generated/containers/MainWheelBase.hpp>


#include <touchgfx/mixins/FadeAnimator.hpp>
#include <gui/containers/MainWheelIcon.hpp>

class MainWheel : public MainWheelBase
{
public:
    MainWheel();
    virtual ~MainWheel() {}

    virtual void initialize();

    // Functions to overide TouchGFX interaction functions
    void handleClickEvent(const ClickEvent& evt) override;
    void handleDragEvent(const DragEvent& evt) override;

    // handleTickEvent enables the UI to act everytime there is a tick, wich comes from the Vsync
    void handleTickEvent() override;

    void startWheelAnimation();

    float getElementAngle(const uint8_t index) const;

    uint32_t getElementTextId(const uint8_t index) const;

    void switchActiveIcons(bool active);

    uint16_t switchToSelectedIcon(bool active, uint32_t myEnum);

    void confirmButtonClicked();

    static const int NUMBER_OF_WHEEL_ELEMENTS = 5;
    static const int NUMBER_OF_SUB_DEMOS = 5;

protected:
    static const int16_t ORIGIN_X = 260; // X coordidate of the menu wheel center
    static const int16_t ORIGIN_Y = 240; // Y coordidate of the menu wheel center

    struct menuElement
    {
        FadeAnimator<MainWheelIcon> icon;
        float angle;
        float size;
        uint32_t textId;
    };

    void updateIconSize(menuElement& element);
    void updateDrawingOfIcon(menuElement& element);
    float calculateAngleFromCoordinate(int16_t x, int16_t y);
    float normalizeAngle(float angle);

    float lastAngle;

    menuElement wheelElements[NUMBER_OF_WHEEL_ELEMENTS];

    float rotatingSteps;
    float rotatingStepCounter;
    float moveDistance;
    float startValue;

    int centerIcon;

    bool rotating;
    bool dragEventHappend;

    float oldAngle;
    bool animateCircleBounce;
    bool animateCircleOut;

    bool activeIconsSelected;
};

#endif // MAINWHEEL_HPP
