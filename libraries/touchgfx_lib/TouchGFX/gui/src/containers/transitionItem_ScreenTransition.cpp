#include <gui/containers/transitionItem_ScreenTransition.hpp>

transitionItem_ScreenTransition::transitionItem_ScreenTransition()
{

}

void transitionItem_ScreenTransition::initialize()
{
    transitionItem_ScreenTransitionBase::initialize();
}

void transitionItem_ScreenTransition::setIcon(uint16_t iconId)
{
    transitionIcon.setBitmap(Bitmap(iconId));
}

void transitionItem_ScreenTransition::setName(uint16_t textId)
{
    transitionName.setTypedText(TypedText(textId));
}

void transitionItem_ScreenTransition::fade(uint8_t endAlpha, uint16_t duration, EasingEquation alphaProgressionEquation)
{
    transitionIcon.startFadeAnimation(endAlpha, duration, alphaProgressionEquation);
}
