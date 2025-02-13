#include <gui/containers/selectedIconsContainer.hpp>

selectedIconsContainer::selectedIconsContainer()
{

}

void selectedIconsContainer::initialize()
{
    selectedIconsContainerBase::initialize();
}

void selectedIconsContainer::updateImage(bool is2d)
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
