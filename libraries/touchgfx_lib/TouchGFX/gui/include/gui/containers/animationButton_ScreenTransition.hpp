#ifndef ANIMATIONBUTTON_SCREENTRANSITION_HPP
#define ANIMATIONBUTTON_SCREENTRANSITION_HPP

#include <gui_generated/containers/animationButton_ScreenTransitionBase.hpp>

class animationButton_ScreenTransition : public animationButton_ScreenTransitionBase
{
public:
    animationButton_ScreenTransition();
    virtual ~animationButton_ScreenTransition() {}

    virtual void initialize();

    virtual void handleClickEvent(const ClickEvent& evt);
    virtual void handleTickEvent();
protected:

    bool normalSpeed;

    int16_t tiltCounter;
    int16_t returnCounter;
};

#endif // ANIMATIONBUTTON_SCREENTRANSITION_HPP
