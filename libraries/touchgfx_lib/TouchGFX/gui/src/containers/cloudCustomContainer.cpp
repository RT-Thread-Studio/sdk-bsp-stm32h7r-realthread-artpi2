#include <gui/containers/cloudCustomContainer.hpp>
#include <touchgfx/events/ClickEvent.hpp>

cloudCustomContainer::cloudCustomContainer():
    imageClickedCallback(this, &cloudCustomContainer::imageClickHandler),
    fadeInRunning(false),
    fadeOutRunning(false),
    cloudsAreMoving(false),
    moveCounter(false)
{

}

void cloudCustomContainer::initialize()
{
    cloudCustomContainerBase::initialize();
    cloudLeftImage.setAlpha(CLOUDS_FADE_FULL);
    cloudRightImage.setAlpha(CLOUDS_FADE_FULL);
    cloudOutsideImage.setAlpha(CLOUDS_FADE_FULL);
    cloudLeftImage.setClickAction(imageClickedCallback);
    cloudRightImage.setClickAction(imageClickedCallback);
    cloudOutsideImage.setClickAction(imageClickedCallback);
}

void cloudCustomContainer::fadeIn()
{
    fadeInRunning = true;
    fadeOutRunning = false;
}

void cloudCustomContainer::fadeOut()
{
    fadeInRunning = false;
    fadeOutRunning = true;
}

void cloudCustomContainer::moveClouds()
{
    cloudsAreMoving = true;
}

void cloudCustomContainer::stopClouds()
{
    cloudsAreMoving = false;
}

void cloudCustomContainer::repositionClouds()
{
    cloudLeftImage.setX(73);
    cloudRightImage.setX(445);
    cloudOutsideImage.setX(-200);
}

void cloudCustomContainer::handleTick()
{
    //Move clouds one pixel to the right every third tick
    if (cloudsAreMoving)
    {
        moveCounter++;
        if (moveCounter % 3 == 0)
        {
            moveCloud(cloudRightImage);
            moveCloud(cloudLeftImage);
            moveCloud(cloudOutsideImage);
        }
    }

    // handle fade animation
    if (fadeInRunning)
    {
        fadeCloudIn(cloudLeftImage);
        fadeCloudIn(cloudRightImage);
        fadeCloudIn(cloudOutsideImage);
        fadeInRunning = cloudLeftImage.getAlpha() < CLOUDS_FADE_NONE ||
                        cloudRightImage.getAlpha() < CLOUDS_FADE_NONE ||
                        cloudOutsideImage.getAlpha() < CLOUDS_FADE_NONE;
    }
    else if (fadeOutRunning)
    {
        fadeCloudOut(cloudLeftImage);
        fadeCloudOut(cloudRightImage);
        fadeCloudOut(cloudOutsideImage);

        fadeOutRunning = cloudLeftImage.getAlpha() > CLOUDS_FADE_FULL ||
                         cloudRightImage.getAlpha() > CLOUDS_FADE_FULL ||
                         cloudOutsideImage.getAlpha() > CLOUDS_FADE_FULL;
    }
}

bool cloudCustomContainer::isFadeInProgress()
{
    return fadeInRunning || fadeOutRunning;
}

void cloudCustomContainer::changeTransparency(touchgfx::Image& cloud)
{
    if (cloud.getAlpha() == CLOUDS_FADE_NONE)
    {
        cloud.setAlpha(CLOUDS_FADE_LIGHT);
    }
    else if (cloud.getAlpha() == CLOUDS_FADE_LIGHT)
    {
        cloud.setAlpha(CLOUDS_FADE_STRONG);
    }
    else if (cloud.getAlpha() == CLOUDS_FADE_STRONG)
    {
        cloud.setAlpha(CLOUDS_FADE_NONE);
    }
}

void cloudCustomContainer::moveCloud(touchgfx::Image& cloud)
{
    cloud.invalidate();
    cloud.setX(cloud.getX() + 1);
    if (cloud.getX() >= getWidth())
    {
        // when the clouds moves out of the screen reset them to the left of the screen
        cloud.setX(-cloud.getWidth());
        cloud.setAlpha(CLOUDS_FADE_NONE);
    }
    cloud.invalidate();
}

void cloudCustomContainer::fadeCloudIn(touchgfx::Image& cloud)
{
    if (cloud.getAlpha() < CLOUDS_FADE_NONE - CLOUDS_FADE_SPEED)
    {
        cloud.setAlpha(cloud.getAlpha() + CLOUDS_FADE_SPEED);
    }
    else
    {
        cloud.setAlpha(CLOUDS_FADE_NONE);
    }
}

void cloudCustomContainer::fadeCloudOut(touchgfx::Image& cloud)
{
    if (cloud.getAlpha() > CLOUDS_FADE_FULL + CLOUDS_FADE_SPEED)
    {
        cloud.setAlpha(cloud.getAlpha() - CLOUDS_FADE_SPEED);
    }
    else
    {
        cloud.setAlpha(CLOUDS_FADE_FULL);
    }
}

void cloudCustomContainer::imageClickHandler(const touchgfx::Image& cloud, const ClickEvent& e)
{
    if (e.getType() == touchgfx::ClickEvent::ClickEventType::PRESSED)
    {
        if (!fadeInRunning && !fadeOutRunning)
        {
            changeTransparency(const_cast<touchgfx::Image&>(cloud));
        }
    }
}
