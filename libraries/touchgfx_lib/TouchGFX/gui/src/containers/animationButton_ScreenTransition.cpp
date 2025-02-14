#include <gui/containers/animationButton_ScreenTransition.hpp>
#include <gui/common/FloatEasingEquations.hpp>
#include "touchgfx/EasingEquations.hpp"
#include "texts/TextKeysAndLanguages.hpp"

animationButton_ScreenTransition::animationButton_ScreenTransition() :
    normalSpeed(true),
    tiltCounter(-1),
    returnCounter(-1)
{

}

void animationButton_ScreenTransition::initialize()
{
    animationButton_ScreenTransitionBase::initialize();
    setTouchable(true);
}

void animationButton_ScreenTransition::handleClickEvent(const ClickEvent& evt)
{
    if (evt.getType() == ClickEvent::RELEASED  && !speedText.isFadeAnimationRunning())
    {
        speedText.startFadeAnimation(0, 30, EasingEquations::expoEaseOut);
        tiltCounter = 0;
        Application::getInstance()->registerTimerWidget(this);
    }
}

void animationButton_ScreenTransition::handleTickEvent()
{
    float durationTilt = 30;
    float durationReturn = 20;

    float startTiltBackground;
    float changeTiltBackground;

    int startSliderX;
    int changeSliderX;

    float startNeedle;
    float changeNeedle;

    float startReturn;
    float changeReturn;

    if (normalSpeed)
    {
        startTiltBackground = 0;
        changeTiltBackground = 0.5f;

        startSliderX = 55;
        changeSliderX = 31 - 55;

        startNeedle = 0;
        changeNeedle = -(PI / 2);

        startReturn = 0.5f;
        changeReturn = -0.5f;
    }
    else
    {
        startTiltBackground = 0;
        changeTiltBackground = -0.5f;

        startSliderX = 31;
        changeSliderX = 55 - 31;

        startNeedle = -(PI / 2);
        changeNeedle = PI / 2;

        startReturn = -0.5f;
        changeReturn = 0.5f;
    }

    if (tiltCounter > -1)
    {
        float newAngle = FloatEasingEquations::floatCubicEaseOut(tiltCounter, startTiltBackground, changeTiltBackground, durationTilt);
        backgroundAnimationSpeed.updateYAngle(newAngle);
        backgroundAnimationSpeed.invalidate();

        sliderAnimationSpeed.updateYAngle(newAngle);
        sliderAnimationSpeed.invalidate();

        needleAnimationSpeed.updateYAngle(newAngle);
        needleAnimationSpeed.invalidate();

        int newX = EasingEquations::bounceEaseOut(tiltCounter, startSliderX, changeSliderX, (int)durationTilt);
        sliderAnimationSpeed.setX(newX);
        sliderAnimationSpeed.invalidate();

        newAngle = FloatEasingEquations::floatCubicEaseOut(tiltCounter, startNeedle, changeNeedle, durationTilt);
        needleAnimationSpeed.updateZAngle(newAngle);
        needleAnimationSpeed.invalidate();

        tiltCounter++;

        if (tiltCounter > durationTilt)
        {
            int newTextId = normalSpeed ? T_SLOWMOTION : T_NORMALSPEED;
            speedText.setTypedText(TypedText(newTextId));
            speedText.startFadeAnimation(255, (int)durationReturn, EasingEquations::expoEaseIn);
            tiltCounter = -1;
            returnCounter = 0;
        }
    }

    if (returnCounter > -1)
    {
        float newAngle = FloatEasingEquations::floatBackEaseOut(returnCounter, startReturn, changeReturn, durationReturn);
        backgroundAnimationSpeed.updateYAngle(newAngle);
        backgroundAnimationSpeed.invalidate();

        sliderAnimationSpeed.updateYAngle(newAngle);
        sliderAnimationSpeed.invalidate();

        needleAnimationSpeed.updateYAngle(newAngle);
        needleAnimationSpeed.invalidate();

        returnCounter++;

        if (returnCounter > durationReturn)
        {
            returnCounter = -1;

            normalSpeed = !normalSpeed;

            Application::getInstance()->unregisterTimerWidget(this);
            emitButtonPressedCallback(normalSpeed);
        }
    }
}
