#ifndef CUBEUPDOWNBUTTONSCONTAINER_HPP
#define CUBEUPDOWNBUTTONSCONTAINER_HPP

#include <gui_generated/containers/cubeUpDownButtonsContainerBase.hpp>

class cubeUpDownButtonsContainer : public cubeUpDownButtonsContainerBase
{
public:
    cubeUpDownButtonsContainer();
    virtual ~cubeUpDownButtonsContainer() {}

    virtual void initialize();

    void startFadeAnimation(uint8_t endAlpha, uint8_t duration, uint16_t delay, EasingEquation alphaProgressionEquation = &EasingEquations::linearEaseNone);
    void setAlpha(uint8_t alpha);
    uint8_t getAlpha();
protected:
    Callback<cubeUpDownButtonsContainer, const FadeAnimator<Image>&> imageFadeEndedCallback;
    void imageFadeEndedHandler(const FadeAnimator<Image>& src);
};

#endif // CUBEUPDOWNBUTTONSCONTAINER_HPP
