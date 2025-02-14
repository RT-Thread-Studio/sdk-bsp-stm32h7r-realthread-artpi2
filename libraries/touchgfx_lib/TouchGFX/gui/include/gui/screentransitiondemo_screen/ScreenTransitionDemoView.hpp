#ifndef SCREENTRANSITIONDEMOVIEW_HPP
#define SCREENTRANSITIONDEMOVIEW_HPP

#include <gui_generated/screentransitiondemo_screen/ScreenTransitionDemoViewBase.hpp>
#include <gui/screentransitiondemo_screen/ScreenTransitionDemoPresenter.hpp>

#include <gui/Transitions/FlipTransition.hpp>
#include <gui/Transitions/CoverTransition.hpp>
#include <gui/Transitions/SlideTransition.hpp>
#include <gui/Transitions/CurtainsTransition.hpp>
#include <gui/Transitions/RolloutTransition.hpp>
#include <gui/Transitions/CubeTransition.hpp>
#include <gui/Transitions/SpinOutTransition.hpp>
#include <gui/Transitions/SpinTransition.hpp>

class ScreenTransitionDemoView : public ScreenTransitionDemoViewBase
{
public:
    ScreenTransitionDemoView();
    virtual ~ScreenTransitionDemoView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    virtual void handleTickEvent();

    virtual void startTransitionPressed();
    virtual void getTransitionInfo(TransitionInfo* value);
    virtual void scrollWheelPressed();
    virtual void scrollWheelAnimationEnded();
    virtual void sliderMenuStateChanged(SlideMenu::State value);
    virtual void animationSpeedButtonPressed(bool value);
    virtual void bottomBarButtonClicked();

protected:
    enum BackgroundState
    {
        DAY,
        NIGHT
    } currentBackground;

    Callback<ScreenTransitionDemoView, uint16_t> transitionEndedCallback;
    void transitionEndedHandler(uint16_t newBackgroundId);

    Callback<ScreenTransitionDemoView, const FadeAnimator<Button>&> buttonFadeAnimationEndedCallback;
    void buttonFadeAnimationEndedHandler(const FadeAnimator<Button>& src);

    bool transitionInProgress;
    uint16_t transitionSpeed;

    Transitions* transitions[Transitions::NUMBER_OF_TRANSITIONS];
    FlipTransition flip;
    CoverTransition cover;
    SlideTransition slide;
    CurtainsTransition curtains;
    RolloutTransition rollout;
    CubeTransition cube;
    SpinOutTransition spinout;
    SpinTransition spin;

    int8_t fadeDirection;

    uint16_t transitionMenuTimeoutCounter;
};

#endif // SCREENTRANSITIONDEMOVIEW_HPP
