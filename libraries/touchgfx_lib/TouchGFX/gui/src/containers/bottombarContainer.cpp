#include <gui/containers/bottombarContainer.hpp>
#include "texts/TextKeysAndLanguages.hpp"
#include "gui/common/constans.hpp"
#include "gui/common/FloatEasingEquations.hpp"

bottombarContainer::bottombarContainer() :
    animationEndedCallback(this, &bottombarContainer::animationEndedHandler)
{

}

void bottombarContainer::initialize()
{
    bottombarContainerBase::initialize();

    transitionSelectionWheel.setAnimationEndedCallback(animationEndedCallback);
}

void bottombarContainer::handleTickEvent()
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

void bottombarContainer::bottomBarClicked(int x, int y)
{
    if (transitionSelectionWheel.getRect().intersect(x, y))
    {
        emitScrollWheelPressedCallback();
    }
}


void bottombarContainer::transitionSelectionWheelUpdateItem(transitionItem& item, int16_t itemIndex)
{
    TransitionInfo info;
    info.transitionId = itemIndex;
    emitGetTransitionInfoCallback(&info);
    item.setIcon(info.iconId);
    item.setName(info.textId);
}

void bottombarContainer::animateArrow(SlideMenu::State value)
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

void bottombarContainer::setNumberOfTransitionsItems(uint16_t numberOfItems)
{
    transitionSelectionWheel.setNumberOfItems(numberOfItems);
}

uint16_t bottombarContainer::getSelectedTransition()
{
    return transitionSelectionWheel.getSelectedItem();
}

void bottombarContainer::fade(uint8_t endAlpha, uint16_t duration, EasingEquation alphaProgressionEquation)
{
    buttonIndicator.startFadeAnimation(endAlpha, duration, alphaProgressionEquation);
    top.startFadeAnimation(endAlpha, duration, alphaProgressionEquation);
}

void bottombarContainer::animationEndedHandler()
{
    emitScrollWheelAnimationEndedCallback();
}
