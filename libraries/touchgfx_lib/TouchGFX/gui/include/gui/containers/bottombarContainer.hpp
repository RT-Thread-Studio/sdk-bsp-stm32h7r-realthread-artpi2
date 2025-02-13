#ifndef BOTTOMBARCONTAINER_HPP
#define BOTTOMBARCONTAINER_HPP

#include <gui_generated/containers/bottombarContainerBase.hpp>

class bottombarContainer : public bottombarContainerBase
{
public:
    bottombarContainer();
    virtual ~bottombarContainer() {}

    virtual void initialize();

    virtual void handleTickEvent();

    void bottomBarClicked(int x, int y);

    virtual void transitionSelectionWheelUpdateItem(transitionItem& item, int16_t itemIndex);
    virtual void animateArrow(SlideMenu::State value);

    void setNumberOfTransitionsItems(uint16_t numberOfItems);
    uint16_t getSelectedTransition();

    void fade(uint8_t endAlpha, uint16_t duration, EasingEquation alphaProgressionEquation = &EasingEquations::linearEaseNone);

protected:

    Callback<bottombarContainer> animationEndedCallback;
    void animationEndedHandler();

    float startAngle;
    float endAngle;
    float indicatorStepCounter;
};

#endif // BOTTOMBARCONTAINER_HPP
