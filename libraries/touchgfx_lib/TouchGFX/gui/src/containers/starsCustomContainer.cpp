#include <gui/containers/starsCustomContainer.hpp>

starsCustomContainer::starsCustomContainer() :
    tickCount(0)
{

}

void starsCustomContainer::initialize()
{
    starsCustomContainerBase::initialize();
    blinkingStarsContainer.setVisible(false);
}

void starsCustomContainer::showStars()
{
    // Make sure only the texture mapper is visible in the beginning
    starsImage.setVisible(false);
    blinkingStarsContainer.setVisible(false);
    star1Image.setAlpha(0);
    star2Image.setAlpha(0);
    star3Image.setAlpha(0);
    star4Image.setAlpha(0);
    star5Image.setAlpha(0);
    starShiny1Image.setAlpha(0);
    starShiny2Image.setAlpha(0);
    starShiny3Image.setAlpha(0);
    starShiny4Image.setAlpha(0);
    starShiny5Image.setAlpha(0);

    // Use the texturemapper to make the stars appear with a fade and scale animation
    starsTextureMapper.setVisible(true);
    starsTextureMapper.setAlpha(0);
    starsTextureMapper.startFadeAnimation(255, 50, EasingEquations::linearEaseOut);
    starsTextureMapper.setScale(0.7f);
    starsTextureMapper.setupAnimation(AnimationTextureMapper::SCALE, 1.0f, 100, 0, EasingEquations::circEaseOut);
    starsTextureMapper.startAnimation();

    tickCount = 0;
}

void starsCustomContainer::handleTick()
{
    // When the texturemapper animation is done, replace it with an image and start the blinking stars
    if (starsTextureMapper.isVisible() && !starsTextureMapper.isTextureMapperAnimationRunning())
    {
        starsTextureMapper.setVisible(false);
        starsImage.setVisible(true);
        starsImage.setAlpha(255);
        blinkingStarsContainer.setVisible(true);
        invalidate();
    }

    // Make some of the stars blink
    else if (blinkingStarsContainer.isVisible())
    {
        if (tickCount == 0)
        {
            star1Image.startFadeAnimation(255, 10, EasingEquations::linearEaseNone);
            star4Image.startFadeAnimation(255, 10, EasingEquations::linearEaseNone);
            starShiny3Image.startFadeAnimation(255, 10, EasingEquations::linearEaseNone);
            starShiny4Image.startFadeAnimation(255, 10, EasingEquations::linearEaseNone);
        }

        else if (tickCount == 20)
        {
            star2Image.startFadeAnimation(0, 10, EasingEquations::linearEaseNone);
            starShiny1Image.startFadeAnimation(0, 10, EasingEquations::linearEaseNone);
            starShiny5Image.startFadeAnimation(0, 10, EasingEquations::linearEaseNone);
        }

        else if (tickCount == 40)
        {
            star3Image.startFadeAnimation(255, 10, EasingEquations::linearEaseNone);
            star5Image.startFadeAnimation(255, 10, EasingEquations::linearEaseNone);
            starShiny2Image.startFadeAnimation(255, 10, EasingEquations::linearEaseNone);
        }

        else if (tickCount == 60)
        {
            star1Image.startFadeAnimation(0, 10, EasingEquations::linearEaseNone);
            star4Image.startFadeAnimation(0, 10, EasingEquations::linearEaseNone);
            starShiny3Image.startFadeAnimation(0, 10, EasingEquations::linearEaseNone);
            starShiny4Image.startFadeAnimation(0, 10, EasingEquations::linearEaseNone);
        }

        else if (tickCount == 80)
        {
            star2Image.startFadeAnimation(255, 10, EasingEquations::linearEaseNone);
            starShiny1Image.startFadeAnimation(255, 10, EasingEquations::linearEaseNone);
            starShiny5Image.startFadeAnimation(255, 10, EasingEquations::linearEaseNone);
        }

        else if (tickCount == 100)
        {
            star3Image.startFadeAnimation(0, 10, EasingEquations::linearEaseNone);
            star5Image.startFadeAnimation(0, 10, EasingEquations::linearEaseNone);
            starShiny2Image.startFadeAnimation(0, 10, EasingEquations::linearEaseNone);
        }

        // Make the blinking sequence loop
        tickCount++;
        if (tickCount >= 120)
        {
            tickCount = 0;
        }
    }
}

void starsCustomContainer::hideStars()
{
    blinkingStarsContainer.setVisible(false);
    starsImage.startFadeAnimation(0, 50, EasingEquations::linearEaseOut);
}
