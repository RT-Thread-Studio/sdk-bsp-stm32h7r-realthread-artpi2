#include <gui/screentransitiondemo_screen/ScreenTransitionDemoView.hpp>
#include <images/BitmapDatabase.hpp>
#include "texts/TextKeysAndLanguages.hpp"
#include "gui/common/constans.hpp"
#include <touchgfx/hal/HAL.hpp>
#include <gui/common/Utils.hpp>

ScreenTransitionDemoView::ScreenTransitionDemoView():
    transitionEndedCallback(this, &ScreenTransitionDemoView::transitionEndedHandler),
    buttonFadeAnimationEndedCallback(this, &ScreenTransitionDemoView::buttonFadeAnimationEndedHandler),
    transitionInProgress(false),
    transitionSpeed(TRANSITION_SPEED_FAST),
    fadeDirection(-1),
    transitionMenuTimeoutCounter(0)
{

}

void ScreenTransitionDemoView::setupScreen()
{
    for (int i = 0; i < Transitions::NUMBER_OF_TRANSITIONS; i++)
    {
        switch (i)
        {
        case Transitions::FLIP :
            transitions[i] = &flip;
            transitions[i]->setTransitionIconId(BITMAP_TRANSITIONTYPE2_ID);
            transitions[i]->setTransitionTextId(T_FLIPTRANSITION);
            transitions[i]->setTransitionTitleTextId(T_LARGEFLIPTRANSITION);
            break;
        case Transitions::COVER:
            transitions[i] = &cover;
            transitions[i]->setTransitionIconId(BITMAP_TRANSITIONTYPE1_ID);
            transitions[i]->setTransitionTextId(T_COVERTRANSITION);
            transitions[i]->setTransitionTitleTextId(T_LARGECOVERTRANSITION);
            break;
        case Transitions::SLIDE:
            transitions[i] = &slide;
            transitions[i]->setTransitionIconId(BITMAP_TRANSITIONTYPE1_ID);
            transitions[i]->setTransitionTextId(T_SLIDETRANSITION);
            transitions[i]->setTransitionTitleTextId(T_LARGESLIDETRANSITION);
            break;
        case Transitions::CURTAINS:
            transitions[i] = &curtains;
            transitions[i]->setTransitionIconId(BITMAP_TRANSITIONTYPE3_ID);
            transitions[i]->setTransitionTextId(T_CURTAINSTRANSITION);
            transitions[i]->setTransitionTitleTextId(T_LARGECURTAINSTRANSITION);
            break;
        case Transitions::ROLLOUT:
            transitions[i] = &rollout;
            transitions[i]->setTransitionIconId(BITMAP_TRANSITIONTYPE2_ID);
            transitions[i]->setTransitionTextId(T_ROLLOUTTRANSITION);
            transitions[i]->setTransitionTitleTextId(T_LARGEROLLOUTTRANSITION);
            break;
        case Transitions::CUBE:
            transitions[i] = &cube;
            transitions[i]->setTransitionIconId(BITMAP_TRANSITIONTYPE4_ID);
            transitions[i]->setTransitionTextId(T_CUBETRANSITION);
            transitions[i]->setTransitionTitleTextId(T_LARGECUBETRANSITION);
            break;
        case Transitions::SPINOUT:
            transitions[i] = &spinout;
            transitions[i]->setTransitionIconId(BITMAP_TRANSITIONTYPE5_ID);
            transitions[i]->setTransitionTextId(T_SPINOUTTRANSITION);
            transitions[i]->setTransitionTitleTextId(T_LARGESPINOUTTRANSITION);
            break;
        case Transitions::SPIN:
            transitions[i] = &spin;
            transitions[i]->setTransitionIconId(BITMAP_TRANSITIONTYPE5_ID);
            transitions[i]->setTransitionTextId(T_SPINTRANSITION);
            transitions[i]->setTransitionTitleTextId(T_LARGESPINTRANSITION);
            break;
        default:
            break;
        }

        transitions[i]->setPosition(background.getX(), background.getY(), background.getWidth(), background.getHeight());
        transitions[i]->setTransitionEndedCallback(transitionEndedCallback);

        add(*transitions[i]);
    }

    bottombar.setNumberOfTransitionsItems(Transitions::NUMBER_OF_TRANSITIONS);

    ScreenTransitionDemoViewBase::setupScreen();

    startTransitionButton.setFadeAnimationEndedAction(buttonFadeAnimationEndedCallback);

}

void ScreenTransitionDemoView::tearDownScreen()
{
    HAL::getInstance()->setFrameRateCompensation(false);
    ScreenTransitionDemoViewBase::tearDownScreen();
}

void ScreenTransitionDemoView::handleTickEvent()
{
    if (transitionMenu.getState() == SlideMenu::EXPANDED && (transitionMenu.getExpandedStateTimer() + 10) > transitionMenu.getExpandedStateTimeout())
    {
        transitionMenu.resetExpandedStateTimer();
    }
    else if (transitionMenu.getState() == SlideMenu::COLLAPSED)
    {
        transitionMenuTimeoutCounter++;
        if (transitionMenuTimeoutCounter > TRANSITION_MENU_TIMEOUT)
        {
            transitionMenu.animateToState(SlideMenu::EXPANDED);
            bottombar.animateArrow(SlideMenu::EXPANDED);
            transitionMenuTimeoutCounter = 0;
        }
    }

    topbar.checkMCULoad();
    topbar.countTheFrames();

    if (!transitionInProgress)
    {
        uint8_t startTransitionButtonAlpha = startTransitionButton.getAlpha();
        if (startTransitionButtonAlpha == 255)
        {
            fadeDirection = -2;
        }
        else if (startTransitionButtonAlpha == 55)
        {
            fadeDirection = 2;
        }
        startTransitionButton.setAlpha(startTransitionButtonAlpha + fadeDirection);
        startTransitionButton.invalidate();
    }
}

void ScreenTransitionDemoView::startTransitionPressed()
{
    transitionInProgress = true;

    startTransitionButton.startFadeAnimation(0, FADE_DURATION_CONTROLS);
    selectedTransitionText.startFadeAnimation(0, FADE_DURATION_CONTROLS);
    bottombar.fade(0, FADE_DURATION_CONTROLS);

    startTransitionButton.setTouchable(false);
    bottomBarButton.setTouchable(false);

    if (transitionMenu.getState() == SlideMenu::EXPANDED)
    {
        transitionMenu.animateToState(SlideMenu::COLLAPSED);
        bottombar.animateArrow(SlideMenu::COLLAPSED);
        transitionMenuTimeoutCounter = 0;
    }
}

void ScreenTransitionDemoView::getTransitionInfo(TransitionInfo* value)
{
    value->iconId = transitions[value->transitionId]->getTransitionIconId();
    value->textId = transitions[value->transitionId]->getTransitionTextId();
}

void ScreenTransitionDemoView::scrollWheelPressed()
{
    selectedTransitionText.startFadeAnimation(0, FADE_DURATION_CONTROLS);
}

void ScreenTransitionDemoView::scrollWheelAnimationEnded()
{
    selectedTransitionText.setTypedText(TypedText(transitions[bottombar.getSelectedTransition()]->getTransitionTitleTextId()));
    selectedTransitionText.invalidate();
    selectedTransitionText.startFadeAnimation(255, FADE_DURATION_CONTROLS);
}

void ScreenTransitionDemoView::sliderMenuStateChanged(SlideMenu::State value)
{
    if (value == SlideMenu::COLLAPSED)
    {
        transitionMenuTimeoutCounter = 0;
    }
}

void ScreenTransitionDemoView::animationSpeedButtonPressed(bool value)
{
    transitionSpeed = value ? TRANSITION_SPEED_FAST : TRANSITION_SPEED_SLOW;
}

void ScreenTransitionDemoView::bottomBarButtonClicked()
{
    if (transitionMenu.getState() == SlideMenu::COLLAPSED)
    {
        transitionMenu.animateToState(SlideMenu::EXPANDED);
        bottombar.animateArrow(SlideMenu::EXPANDED);
    }
    else
    {
        transitionMenu.animateToState(SlideMenu::COLLAPSED);
        bottombar.animateArrow(SlideMenu::COLLAPSED);
    }
}

void ScreenTransitionDemoView::transitionEndedHandler(uint16_t newBackgroundId)
{
    if (newBackgroundId == BITMAP_BACKGROUND_DAY_FULL_ID)
    {
        currentBackground = DAY;
    }
    else
    {
        currentBackground = NIGHT;
    }

    background.setBitmap(Bitmap(newBackgroundId));
    background.setVisible(true);
    background.invalidate();

    startTransitionButton.startFadeAnimation(255, FADE_DURATION_CONTROLS);
    selectedTransitionText.startFadeAnimation(255, FADE_DURATION_CONTROLS);
    bottombar.fade(255, FADE_DURATION_CONTROLS);
}

void ScreenTransitionDemoView::buttonFadeAnimationEndedHandler(const FadeAnimator<Button>& src)
{
    if (&src == &startTransitionButton && startTransitionButton.getAlpha() == 0)
    {
        if (currentBackground == DAY)
        {
            transitions[bottombar.getSelectedTransition()]->startTransition(Bitmap(BITMAP_BACKGROUND_DAY_FULL_ID), Bitmap(BITMAP_BACKGROUND_NIGHT_FULL_ID), transitionSpeed);
        }
        else
        {
            transitions[bottombar.getSelectedTransition()]->startTransition(Bitmap(BITMAP_BACKGROUND_NIGHT_FULL_ID), Bitmap(BITMAP_BACKGROUND_DAY_FULL_ID), transitionSpeed);
        }
        background.setVisible(false);
        background.invalidate();

        remove(topbar);
        add(topbar);
    }
    else
    {
        transitionMenu.animateToState(SlideMenu::EXPANDED);
        bottombar.animateArrow(SlideMenu::EXPANDED);

        startTransitionButton.setTouchable(true);
        bottomBarButton.setTouchable(true);

        transitionInProgress = false;
    }
}
