#include <gui/containers/cubeUpDownButtonsContainer.hpp>

cubeUpDownButtonsContainer::cubeUpDownButtonsContainer()
{

}

void cubeUpDownButtonsContainer::initialize()
{
    cubeUpDownButtonsContainerBase::initialize();
}

void cubeUpDownButtonsContainer::startFadeAnimation(uint8_t endAlpha, uint8_t duration, uint16_t delay, EasingEquation alphaProgressionEquation)
{
    buttonUp.setFadeAnimationDelay(delay);
    buttonDown.setFadeAnimationDelay(delay);
    background.setFadeAnimationDelay(delay);

    buttonUp.startFadeAnimation(endAlpha, duration, alphaProgressionEquation);
    buttonDown.startFadeAnimation(endAlpha, duration, alphaProgressionEquation);
    background.startFadeAnimation(endAlpha, duration, alphaProgressionEquation);
}

void cubeUpDownButtonsContainer::setAlpha(uint8_t alpha)
{
    buttonUp.setAlpha(alpha);
    buttonDown.setAlpha(alpha);
    background.setAlpha(alpha);
}

uint8_t cubeUpDownButtonsContainer::getAlpha()
{
    return background.getAlpha();
}

void cubeUpDownButtonsContainer::imageFadeEndedHandler(const FadeAnimator<Image>& src)
{
    emitFadeAnimationEndedCallback();
}
