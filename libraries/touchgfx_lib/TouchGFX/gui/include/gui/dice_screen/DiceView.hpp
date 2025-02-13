#ifndef DICEVIEW_HPP
#define DICEVIEW_HPP

#include <gui_generated/dice_screen/DiceViewBase.hpp>
#include <gui/dice_screen/DicePresenter.hpp>

class DiceView : public DiceViewBase
{
public:
    DiceView();
    virtual ~DiceView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    virtual void handleTickEvent();

    virtual void scaleSliderChanged(int value);
    virtual void animationComplexityChanged(bool value);
    virtual void upPressed();
    virtual void downPressed();
    virtual void leftPressed();
    virtual void rightPressed();
    virtual void exitCubeAnimationEnded();
    virtual void enterCubeAnimationEnded();
    virtual void animationDone();
    virtual void chromArtButtonPressed(bool value);

    void changeCubeState();
protected:
    animationsDepthButton::BUTTON_STATE currentState;

    bool waitingForDiceAnimation;

    int16_t frameSkippedCounter;
    int16_t frames;
    int16_t fps;
    uint16_t previousMcuLoadPct;
};

#endif // DICEVIEW_HPP
