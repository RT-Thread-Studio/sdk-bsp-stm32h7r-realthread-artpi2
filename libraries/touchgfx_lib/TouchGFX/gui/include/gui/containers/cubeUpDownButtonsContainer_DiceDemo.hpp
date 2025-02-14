#ifndef CUBEUPDOWNBUTTONSCONTAINER_DICEDEMO_HPP
#define CUBEUPDOWNBUTTONSCONTAINER_DICEDEMO_HPP

#include <gui_generated/containers/cubeUpDownButtonsContainer_DiceDemoBase.hpp>

class cubeUpDownButtonsContainer_DiceDemo : public cubeUpDownButtonsContainer_DiceDemoBase
{
public:
    cubeUpDownButtonsContainer_DiceDemo();
    virtual ~cubeUpDownButtonsContainer_DiceDemo() {}

    virtual void initialize();

    void startFadeAnimation(uint8_t endAlpha, uint8_t duration, uint16_t delay, EasingEquation alphaProgressionEquation = &EasingEquations::linearEaseNone);
    void setAlpha(uint8_t alpha);
    uint8_t getAlpha();
protected:
    Callback<cubeUpDownButtonsContainer_DiceDemo, const FadeAnimator<Image>&> imageFadeEndedCallback;
    void imageFadeEndedHandler(const FadeAnimator<Image>& src);
};

#endif // CUBEUPDOWNBUTTONSCONTAINER_DICEDEMO_HPP
