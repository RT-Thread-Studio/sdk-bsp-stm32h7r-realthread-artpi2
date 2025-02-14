#include <gui/containers/selectedIconsContainer_DiceDemo.hpp>

selectedIconsContainer_DiceDemo::selectedIconsContainer_DiceDemo()
{

}

void selectedIconsContainer_DiceDemo::initialize()
{
    selectedIconsContainer_DiceDemoBase::initialize();
}

void selectedIconsContainer_DiceDemo::updateImage(bool is2d)
{
    if (is2d)
    {
        dice2DPressed.setFadeAnimationDelay(0);
        dice2DPressed.startFadeAnimation(0, FADE_DURATION);
        dice3DPressed.setFadeAnimationDelay(FADE_DURATION);
        dice3DPressed.startFadeAnimation(255, FADE_DURATION);
    }
    else
    {
        dice3DPressed.setFadeAnimationDelay(0);
        dice3DPressed.startFadeAnimation(0, FADE_DURATION);
        dice2DPressed.setFadeAnimationDelay(FADE_DURATION);
        dice2DPressed.startFadeAnimation(255, FADE_DURATION);
    }
}
