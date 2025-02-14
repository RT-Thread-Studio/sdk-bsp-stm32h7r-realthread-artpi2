#ifndef DICEDEMOVIEW_HPP
#define DICEDEMOVIEW_HPP

#include <gui_generated/dicedemo_screen/DiceDemoViewBase.hpp>
#include <gui/dicedemo_screen/DiceDemoPresenter.hpp>

class DiceDemoView : public DiceDemoViewBase
{
public:
    DiceDemoView();
    virtual ~DiceDemoView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    virtual void handleTickEvent();

    virtual void scaleSliderChanged(float value);
    virtual void animationComplexTionChanged(bool value);
    virtual void upPressed();
    virtual void downPressed();
    virtual void leftPressed();
    virtual void rightPressed();
    virtual void exitCubeAnimationEnded();
    virtual void enterCubeAnimationEnded();
    virtual void animationDone();

    void changeCubeState();
protected:
    animationsDepthButton_DiceDemo::BUTTON_STATE currentState;

    bool waitingForDiceAnimation;
};

#endif // DICEDEMOVIEW_HPP
