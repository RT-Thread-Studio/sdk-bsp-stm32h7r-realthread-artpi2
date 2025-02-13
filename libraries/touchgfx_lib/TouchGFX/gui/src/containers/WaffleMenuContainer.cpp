#include <gui/containers/WaffleMenuContainer.hpp>
#include <images/BitmapDatabase.hpp>

WaffleMenuContainer::WaffleMenuContainer(): chosenWaffle(BIG)
{

}

void WaffleMenuContainer::initialize()
{
    WaffleMenuContainerBase::initialize();
}

void WaffleMenuContainer::setupTheMenu()
{
    bigWaffleMenuIcon.setupTheIcon(touchgfx::Bitmap(BITMAP_WAFFLE_MENU_CONE_LARGE_ID));
    mediumWaffleMenuIcon.setupTheIcon(touchgfx::Bitmap(BITMAP_WAFFLE_MENU_CONE_MEDIUM_ID));
    smallWaffleMenuIcon.setupTheIcon(touchgfx::Bitmap(BITMAP_WAFFLE_MENU_CONE_SMALL_ID));
}

void WaffleMenuContainer::changeStateOfMenuTo(SlideMenu::State state)
{
    slideMenu.animateToState(state);
    reactAccordinglyToTheNewStateOfMenu();
    emitSlideMenuChangedStateCallback();
}

void WaffleMenuContainer::reactAccordinglyToTheNewStateOfMenu()
{
    if (slideMenu.getState() == SlideMenu::COLLAPSED)
    {
        setWaffleLabelVisibilityTo(true);
        setMenuClickListenerActivityTo(true);
    }
    else
    {
        setWaffleLabelVisibilityTo(false);
        setMenuClickListenerActivityTo(false);
    }
}

void WaffleMenuContainer::setMenuClickListenerActivityTo(bool activity)
{
    menuClickListener.setVisible(activity);
    menuClickListener.invalidate();
}

void WaffleMenuContainer::setWaffleLabelVisibilityTo(bool visibility)
{
    int endAlpha, duration;
    if (visibility)
    {
        endAlpha = 255;
        duration = 20;
    }
    else
    {
        endAlpha = 0;
        duration = 6;
    }

    waffleLabel.clearFadeAnimationEndedAction();
    waffleLabel.startFadeAnimation(endAlpha, duration, touchgfx::EasingEquations::linearEaseIn);
}

void WaffleMenuContainer::bigWaffleIsChosen()
{
    chosenWaffle = BIG;

    changeStateOfMenuTo(SlideMenu::COLLAPSED);

    emitNewWaffleIsChosenCallback();
}

void WaffleMenuContainer::mediumWaffleIsChosen()
{
    chosenWaffle = MEDIUM;

    changeStateOfMenuTo(SlideMenu::COLLAPSED);

    emitNewWaffleIsChosenCallback();
}

void WaffleMenuContainer::smallWaffleIsChosen()
{
    chosenWaffle = SMALL;

    changeStateOfMenuTo(SlideMenu::COLLAPSED);

    emitNewWaffleIsChosenCallback();
}

WaffleMenuContainer::SizeOfWaffle WaffleMenuContainer::getSizeOfTheChosenWaffle()
{
    return chosenWaffle;
}

void WaffleMenuContainer::shakeTheBigWaffleIcon()
{
    if (slideMenu.getState() == SlideMenu::COLLAPSED)
    {
        bigWaffleMenuIcon.startTheShakeAnimation();
    }
}
