#include <gui/containers/truckCustomContainer.hpp>
#include <BitmapDatabase.hpp>
#include <math.h>

TruckCustomContainer::TruckCustomContainer():
    animationStarted(false),
    lastDayColorWasBlue(false),
    tickCounter(0),
    animationStartX(0),
    animationStartWidth(0)
{

}

void TruckCustomContainer::initialize()
{
    TruckCustomContainerBase::initialize();
}

void TruckCustomContainer::setLights(bool active)
{
    int fadeValue = active ? 255 : 0;
    frontLightImage.startFadeAnimation(fadeValue, TRUCK_LIGHTS_FADE_DURATION, EasingEquations::linearEaseOut);
    backLightImage.startFadeAnimation(fadeValue, TRUCK_LIGHTS_FADE_DURATION, EasingEquations::linearEaseOut);
}

void TruckCustomContainer::setNewColor(const bool nightMode)
{
    if (nightMode)
    {
        truckScalableImage.setBitmap(BITMAP_TRUCK_PINK_NIGHT_ID);
    }
    else
    {
        if (lastDayColorWasBlue)
        {
            truckScalableImage.setBitmap(BITMAP_TRUCK_YELLOW_ID);
            lastDayColorWasBlue = false;
        }
        else
        {
            truckScalableImage.setBitmap(BITMAP_TRUCK_BLUE_ID);
            lastDayColorWasBlue = true;
        }
    }
}

void TruckCustomContainer::startAnimation()
{
    animationStarted = true;
    tickCounter = 0;
    animationStartX = truckScalableImage.getX();
    animationStartWidth = truckScalableImage.getWidth();
}

bool TruckCustomContainer::isAnimationFinished()
{
    return !animationStarted;
}

void TruckCustomContainer::animationTick()
{
    if (animationStarted)
    {
        tickCounter++;
        const uint16_t expansion = (uint16_t)(sin(PI / ANIMATION_DURATION * tickCounter) * ANIMATION_PEAK_EXPANSION);
        truckScalableImage.invalidate();
        truckScalableImage.setWidth(animationStartWidth + expansion);
        truckScalableImage.setX(animationStartX - expansion / 2); //shift image half of the expansion, to keep it centered

        if (tickCounter == ANIMATION_DURATION)
        {
            animationStarted = false;
            truckScalableImage.setWidth(animationStartWidth);
            truckScalableImage.setX(animationStartX);
        }
        truckScalableImage.invalidate();
    }
}
