#include <gui/containers/ChimneyCustomContainer.hpp>
#include <BitmapDatabase.hpp>
#include <math.h>

ChimneyCustomContainer::ChimneyCustomContainer() :
    animationStarted(false),
    tickCounter(0),
    animationStartY(0),
    animationStartHeight(0)
{

}

void ChimneyCustomContainer::initialize()
{
    ChimneyCustomContainerBase::initialize();

    chimneyScalableImage.setVisible(false);
}

void ChimneyCustomContainer::startAnimation(const bool nightMode)
{
    if (nightMode)
    {
        chimneyScalableImage.setBitmap(BITMAP_CHIMNEY_NIGHT_ID);
    }
    else
    {
        chimneyScalableImage.setBitmap(BITMAP_CHIMNEY_DAY_ID);
    }

    animationStarted = true;
    tickCounter = 0;
    animationStartY = chimneyScalableImage.getY();
    animationStartHeight = chimneyScalableImage.getHeight();
    chimneyScalableImage.setVisible(true);
}

void ChimneyCustomContainer::tick()
{
    if (animationStarted)
    {
        tickCounter++;
        const uint16_t expansion = (uint16_t)(sin(PI / ANIMATION_DURATION * tickCounter) * ANIMATION_PEAK_EXPANSION);
        chimneyScalableImage.setHeight(animationStartHeight + expansion);
        chimneyScalableImage.setY(animationStartY - expansion); //shift image the same as the expansion, to keep it excatly on top of the building

        if (tickCounter == ANIMATION_DURATION)
        {
            animationStarted = false;
            chimneyScalableImage.setHeight(animationStartHeight);
            chimneyScalableImage.setY(animationStartY);
            chimneyScalableImage.setVisible(false);
        }
        invalidate();
    }
}
