#include <gui/dice_screen/DiceView.hpp>
#include <touchgfx/hal/HAL.hpp>

DiceView::DiceView() : currentState(animationsDepthButton::STATE_2D),
    waitingForDiceAnimation(false),
    frameSkippedCounter(0),
    frames(0),
    fps(0),
    previousMcuLoadPct(0)
{

}

void DiceView::setupScreen()
{
    //Enables framerate compensation, thereby skips frame if it can't render and transfer it in time for next vsync
    HAL::getInstance()->setFrameRateCompensation(true);

    //This function enables/disable Chrom-ART, the DMA2D hardware accelerator
    HAL::getInstance()->enableDMAAcceleration(true);

    DiceViewBase::setupScreen();
    cubeUpDownButtons.setTouchable(false);
}

void DiceView::tearDownScreen()
{
    HAL::getInstance()->setFrameRateCompensation(false);

    DiceViewBase::tearDownScreen();
}

void DiceView::handleTickEvent()
{
    // -- MCU LOAD -- //
    // This section uses the HAL layer to detect the MCU load and if it is changed the topBar is updated
    uint16_t mcuLoadPct = touchgfx::HAL::getInstance()->getMCULoadPct();
    if (mcuLoadPct != previousMcuLoadPct)
    {
        previousMcuLoadPct = mcuLoadPct;
        topBar.updateMCU(previousMcuLoadPct);
    }
    // -- MCU LOAD -- //

    // -- FPS -- //
    // This section uses the HAL layer to detect the LCD Refresh count and calculates the FPS if it is changed the topBar is updated
    if (HAL::getInstance()->getLCDRefreshCount() > 1)
    {
        frameSkippedCounter++;
    }
    else
    {
        frames++;
    }

    if (frames + frameSkippedCounter >= 60)
    {
        if (fps != frames)
        {
            fps = frames;
            topBar.updateFPS(fps);
        }
        frameSkippedCounter = 0;
        frames = 0;
    }
    // -- FPS -- //
}

void DiceView::scaleSliderChanged(int value)
{
    float newScale = value / 100.f;
    simpleDice.setScale(newScale);
    advanceDice.setScale(newScale);
}

void DiceView::animationComplexityChanged(bool value)
{
    currentState = (animationsDepthButton::BUTTON_STATE)value;
    if (!advanceDice.getIsAnimating() && !simpleDice.getIsAnimating())
    {
        changeCubeState();
    }
    else
    {
        waitingForDiceAnimation = true;
    }
}

void DiceView::upPressed()
{
    if (!advanceDice.getIsAnimating() && !simpleDice.getIsAnimating())
    {
        switch (currentState)
        {
        case animationsDepthButton::STATE_2D:
            break;
        case animationsDepthButton::STATE_3D:
            advanceDice.up();
            break;
        }
    }
}

void DiceView::downPressed()
{
    if (!advanceDice.getIsAnimating() && !simpleDice.getIsAnimating())
    {
        switch (currentState)
        {
        case animationsDepthButton::STATE_2D:
            break;
        case animationsDepthButton::STATE_3D:
            advanceDice.down();
            break;
        }
    }
}

void DiceView::leftPressed()
{
    if (!advanceDice.getIsAnimating() && !simpleDice.getIsAnimating())
    {
        switch (currentState)
        {
        case animationsDepthButton::STATE_2D:
            simpleDice.left();
            break;
        case animationsDepthButton::STATE_3D:
            advanceDice.left();
            break;
        }
    }
}

void DiceView::rightPressed()
{
    if (!advanceDice.getIsAnimating() && !simpleDice.getIsAnimating())
    {
        switch (currentState)
        {
        case animationsDepthButton::STATE_2D:
            simpleDice.right();
            break;
        case animationsDepthButton::STATE_3D:
            advanceDice.right();
            break;
        }
    }
}

void DiceView::exitCubeAnimationEnded()
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

void DiceView::enterCubeAnimationEnded()
{
    cubeUpDownButtons.setTouchable(true);
    cubeLeftRightButtons.setTouchable(true);
    animationDone();
}

void DiceView::animationDone()
{
    if (waitingForDiceAnimation)
    {
        changeCubeState();
        waitingForDiceAnimation = false;
    }
}

void DiceView::chromArtButtonPressed(bool value)
{
    HAL::getInstance()->enableDMAAcceleration(value);
}

void DiceView::changeCubeState()
{
    switch (currentState)
    {
    case animationsDepthButton::STATE_2D:
        advanceDice.exit3Dcube();
        cubeUpDownButtons.startFadeAnimation(0, 50, 0);
        cubeUpDownButtons.setTouchable(false);
        cubeLeftRightButtons.setTouchable(false);
        break;
    case animationsDepthButton::STATE_3D:
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
