#ifndef ANIMATIONBUTTON_HPP
#define ANIMATIONBUTTON_HPP

#include <gui_generated/containers/animationButtonBase.hpp>

class animationButton : public animationButtonBase
{
public:
    animationButton();
    virtual ~animationButton() {}

    virtual void initialize();

    virtual void handleClickEvent(const ClickEvent& evt);
    virtual void handleTickEvent();
protected:

    bool normalSpeed;

    int16_t tiltCounter;
    int16_t returnCounter;
};

#endif // ANIMATIONBUTTON_HPP
