#include <gui/containers/bottombarContainer_ScreenTransition.hpp>
#include "texts/TextKeysAndLanguages.hpp"
#include "gui/common/constans.hpp"
#include "gui/common/FloatEasingEquations.hpp"


bottombarContainer_ScreenTransition::bottombarContainer_ScreenTransition() :
    animationEndedCallback(this, &bottombarContainer_ScreenTransition::animationEndedHandler)
{

}

void bottombarContainer_ScreenTransition::initialize()
{
    bottombarContainer_ScreenTransitionBase::initialize();

    transitionSelectionWheel.setAnimationEndedCallback(animationEndedCallback);
}

void bottombarContainer_ScreenTransition::handleTickEvent()
{
    if (indicatorStepCounter > -1)
    {
        float newAngle = FloatEasingEquations::floatCubicEaseOut(indicatorStepCounter, startAngle, endAngle - startAngle, INDICATOR_MOVE_DURATION);
        buttonIndicator.updateZAngle(newAngle);
        buttonIndicator.invalidate();

        indicatorStepCounter++;
        if (indicatorStepCounter > INDICATOR_MOVE_DURATION)
        {
            indicatorStepCounter = -1;
            Application::getInstance()->unregisterTimerWidget(this);
        }
    }
}

void bottombarContainer_ScreenTransition::transitionSelectionWheelUpdateItem(transitionItem_ScreenTransition& item, int16_t itemIndex)
{
    TransitionInfo info;
    info.transitionId = itemIndex;

    if (transitionSelectionWheel.isAnimating())
    {
        emitScrollWheelPressedCallback();
    }

    emitGetTransitionInfoCallback(&info);
    item.setIcon(info.iconId);
    item.setName(info.textId);
}

void bottombarContainer_ScreenTransition::animateArrow(SlideMenu::State value)
{
    startAngle = buttonIndicator.getZAngle();
    indicatorStepCounter = 0;

    if (value == SlideMenu::EXPANDED)
    {
        endAngle = 0;
    }
    else
    {
        endAngle = PI;
    }

    Application::getInstance()->registerTimerWidget(this);
}

void bottombarContainer_ScreenTransition::setNumberOfTransitionsItems(uint16_t numberOfItems)
{
    transitionSelectionWheel.setNumberOfItems(numberOfItems);
}

uint16_t bottombarContainer_ScreenTransition::getSelectedTransition()
{
    return transitionSelectionWheel.getSelectedItem();
}

void bottombarContainer_ScreenTransition::fade(uint8_t endAlpha, uint16_t duration, EasingEquation alphaProgressionEquation)
{
    buttonIndicator.startFadeAnimation(endAlpha, duration, alphaProgressionEquation);
    top.startFadeAnimation(endAlpha, duration, alphaProgressionEquation);
}

void bottombarContainer_ScreenTransition::animationEndedHandler()
{
    emitScrollWheelAnimationEndedCallback();
}
