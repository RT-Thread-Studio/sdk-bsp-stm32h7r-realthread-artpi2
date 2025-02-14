#include <gui/dicedemo_screen/DiceDemoView.hpp>
#include <touchgfx/hal/HAL.hpp>
#include <gui/common/Utils.hpp>

DiceDemoView::DiceDemoView() : currentState(animationsDepthButton_DiceDemo::STATE_2D),
    waitingForDiceAnimation(false)
{

}

void DiceDemoView::setupScreen()
{
    DiceDemoViewBase::setupScreen();
    cubeUpDownButtons.setTouchable(false);
}

void DiceDemoView::tearDownScreen()
{
    HAL::getInstance()->setFrameRateCompensation(false);

    DiceDemoViewBase::tearDownScreen();
}

void DiceDemoView::handleTickEvent()
{
    topbar.checkMCULoad();
    topbar.countTheFrames();
}

void DiceDemoView::scaleSliderChanged(float value)
{
    float newScale = value / 100.f;
    simpleDice.setScale(newScale);
    advanceDice.setScale(newScale);
}

void DiceDemoView::animationComplexTionChanged(bool value)
{
    currentState = (animationsDepthButton_DiceDemo::BUTTON_STATE)value;
    if (!advanceDice.getIsAnimating() && !simpleDice.getIsAnimating())
    {
        changeCubeState();
    }
    else
    {
        waitingForDiceAnimation = true;
    }
}

void DiceDemoView::upPressed()
{
    if (!advanceDice.getIsAnimating() && !simpleDice.getIsAnimating())
    {
        switch (currentState)
        {
        case animationsDepthButton_DiceDemo::STATE_2D:
            break;
        case animationsDepthButton_DiceDemo::STATE_3D:
            advanceDice.up();
            break;
        }
    }
}

void DiceDemoView::downPressed()
{
    if (!advanceDice.getIsAnimating() && !simpleDice.getIsAnimating())
    {
        switch (currentState)
        {
        case animationsDepthButton_DiceDemo::STATE_2D:
            break;
        case animationsDepthButton_DiceDemo::STATE_3D:
            advanceDice.down();
            break;
        }
    }
}

void DiceDemoView::leftPressed()
{
    if (!advanceDice.getIsAnimating() && !simpleDice.getIsAnimating())
    {
        switch (currentState)
        {
        case animationsDepthButton_DiceDemo::STATE_2D:
            simpleDice.left();
            break;
        case animationsDepthButton_DiceDemo::STATE_3D:
            advanceDice.left();
            break;
        }
    }
}

void DiceDemoView::rightPressed()
{
    if (!advanceDice.getIsAnimating() && !simpleDice.getIsAnimating())
    {
        switch (currentState)
        {
        case animationsDepthButton_DiceDemo::STATE_2D:
            simpleDice.right();
            break;
        case animationsDepthButton_DiceDemo::STATE_3D:
            advanceDice.right();
            break;
        }
    }
}

void DiceDemoView::exitCubeAnimationEnded()
{
    cubeLeftRightButtons.setTouchable(true);

    simpleDice.setZAngle(advanceDice.getFrontBitmapAngle());
    simpleDice.setVisibleBitmap(advanceDice.getLeftFrontBitmapId());

    simpleDice.invalidateDice();
    advanceDice.invalidateDice();

    simpleDice.setVisible(true);
    advanceDice.setVisible(false);
    animationDone();
}

void DiceDemoView::enterCubeAnimationEnded()
{
    cubeUpDownButtons.setTouchable(true);
    cubeLeftRightButtons.setTouchable(true);
    animationDone();
}

void DiceDemoView::animationDone()
{
    if (waitingForDiceAnimation)
    {
        changeCubeState();
        waitingForDiceAnimation = false;
    }
}

void DiceDemoView::changeCubeState()
{
    switch (currentState)
    {
    case animationsDepthButton_DiceDemo::STATE_2D:
        advanceDice.exit3Dcube();
        cubeUpDownButtons.startFadeAnimation(0, 50, 0);
        cubeUpDownButtons.setTouchable(false);
        cubeLeftRightButtons.setTouchable(false);
        break;
    case animationsDepthButton_DiceDemo::STATE_3D:
        cubeUpDownButtons.startFadeAnimation(255, 50, 0);
        cubeLeftRightButtons.setTouchable(false);

        simpleDice.invalidateDice();
        advanceDice.invalidateDice();

        simpleDice.setVisible(false);
        advanceDice.setVisible(true);
        advanceDice.setDiceSides(simpleDice.getVisibleBitmapId());
        advanceDice.enter3Dcube(simpleDice.getZAngle());
        break;
    }
}
