#ifndef BOTTOMBARCONTAINER_SCREENTRANSITION_HPP
#define BOTTOMBARCONTAINER_SCREENTRANSITION_HPP

#include <gui_generated/containers/bottombarContainer_ScreenTransitionBase.hpp>

class bottombarContainer_ScreenTransition : public bottombarContainer_ScreenTransitionBase
{
public:
    bottombarContainer_ScreenTransition();
    virtual ~bottombarContainer_ScreenTransition() {}

    virtual void initialize();

    virtual void handleTickEvent();

    virtual void transitionSelectionWheelUpdateItem(transitionItem_ScreenTransition& item, int16_t itemIndex);
    virtual void animateArrow(SlideMenu::State value);

    void setNumberOfTransitionsItems(uint16_t numberOfItems);
    uint16_t getSelectedTransition();

    void fade(uint8_t endAlpha, uint16_t duration, EasingEquation alphaProgressionEquation = &EasingEquations::linearEaseNone);

protected:
    Callback<bottombarContainer_ScreenTransition> animationEndedCallback;
    void animationEndedHandler();

    float startAngle;
    float endAngle;
    float indicatorStepCounter;
};

#endif // BOTTOMBARCONTAINER_SCREENTRANSITION_HPP
