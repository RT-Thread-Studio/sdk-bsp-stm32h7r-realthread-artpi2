#include "gui/Transitions/Transitions.hpp"

Transitions::Transitions()
{
}

Transitions::~Transitions()
{

}

void Transitions::handleTickEvent()
{
    if (stepCounter <= transitionDuration)
    {
        animationTick(stepCounter, transitionDuration);
    }
    else if (stepCounter > transitionDuration)
    {
        uint16_t bitmapId = endTransition();
        Application::getInstance()->unregisterTimerWidget(this);

        if (transitionEndedCallback && transitionEndedCallback->isValid())
        {
            transitionEndedCallback->execute(bitmapId);
        }
    }

    stepCounter++;
}

void Transitions::startTransition(Bitmap startBitmap, Bitmap endBitmap, uint16_t duration)
{
    setupTranstion(startBitmap, endBitmap);
    transitionDuration = duration;
    stepCounter = 0;

    Application::getInstance()->registerTimerWidget(this);
}

void Transitions::setTransitionIconId(uint16_t iconId)
{
    transitionIconId = iconId;
}

uint16_t Transitions::getTransitionIconId()
{
    return transitionIconId;
}

void Transitions::setTransitionTextId(uint16_t textId)
{
    transitionTextId = textId;
}

uint16_t Transitions::getTransitionTextId()
{
    return transitionTextId;
}

void Transitions::setTransitionTitleTextId(uint16_t textId)
{
    transitionTitleTextId = textId;
}

uint16_t Transitions::getTransitionTitleTextId()
{
    return transitionTitleTextId;
}

void Transitions::setTransitionEndedCallback(GenericCallback<uint16_t>& callback)
{
    transitionEndedCallback = &callback;
}
