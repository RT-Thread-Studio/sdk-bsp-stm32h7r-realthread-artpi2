#include <gui/containers/cubeUpDownButtonsContainer_DiceDemo.hpp>

cubeUpDownButtonsContainer_DiceDemo::cubeUpDownButtonsContainer_DiceDemo()
{

}

void cubeUpDownButtonsContainer_DiceDemo::initialize()
{
    cubeUpDownButtonsContainer_DiceDemoBase::initialize();
}

void cubeUpDownButtonsContainer_DiceDemo::startFadeAnimation(uint8_t endAlpha, uint8_t duration, uint16_t delay, EasingEquation alphaProgressionEquation)
{
    buttonUp.setFadeAnimationDelay(delay);
    buttonDown.setFadeAnimationDelay(delay);
    background.setFadeAnimationDelay(delay);

    buttonUp.startFadeAnimation(endAlpha, duration, alphaProgressionEquation);
    buttonDown.startFadeAnimation(endAlpha, duration, alphaProgressionEquation);
    background.startFadeAnimation(endAlpha, duration, alphaProgressionEquation);
}

void cubeUpDownButtonsContainer_DiceDemo::setAlpha(uint8_t alpha)
{
    buttonUp.setAlpha(alpha);
    buttonDown.setAlpha(alpha);
    background.setAlpha(alpha);
}

uint8_t cubeUpDownButtonsContainer_DiceDemo::getAlpha()
{
    return background.getAlpha();
}

void cubeUpDownButtonsContainer_DiceDemo::imageFadeEndedHandler(const FadeAnimator<Image>& src)
{
    emitFadeAnimationEnddedCallback();
}
