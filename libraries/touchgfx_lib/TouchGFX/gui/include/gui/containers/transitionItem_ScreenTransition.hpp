#ifndef TRANSITIONITEM_SCREENTRANSITION_HPP
#define TRANSITIONITEM_SCREENTRANSITION_HPP

#include <gui_generated/containers/transitionItem_ScreenTransitionBase.hpp>

class transitionItem_ScreenTransition : public transitionItem_ScreenTransitionBase
{
public:
    transitionItem_ScreenTransition();
    virtual ~transitionItem_ScreenTransition() {}

    virtual void initialize();

    void setIcon(uint16_t iconId);
    void setName(uint16_t textId);
    void fade(uint8_t endAlpha, uint16_t duration, EasingEquation alphaProgressionEquation = &EasingEquations::linearEaseNone);
protected:
};

#endif // TRANSITIONITEM_SCREENTRANSITION_HPP
