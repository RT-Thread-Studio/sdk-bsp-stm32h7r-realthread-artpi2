#include <gui/containers/animationsDepthButton_DiceDemo.hpp>

animationsDepthButton_DiceDemo::animationsDepthButton_DiceDemo():
    currentButtonState(STATE_2D)
{

}

void animationsDepthButton_DiceDemo::initialize()
{
    animationsDepthButton_DiceDemoBase::initialize();
}

void animationsDepthButton_DiceDemo::touchOverlayPressed()
{
    switch (currentButtonState)
    {
    case animationsDepthButton_DiceDemo::STATE_2D:
        selectedIcons.startMoveAnimation(BUTTON_3D_X, BUTTON_3D_Y, BUTTON_ANIMATION_DURATION, EasingEquations::cubicEaseOut, EasingEquations::cubicEaseOut);
        currentButtonState = STATE_3D;
        break;
    case animationsDepthButton_DiceDemo::STATE_3D:
        selectedIcons.startMoveAnimation(BUTTON_2D_X, BUTTON_2D_Y, BUTTON_ANIMATION_DURATION, EasingEquations::cubicEaseOut, EasingEquations::cubicEaseOut);
        currentButtonState = STATE_2D;
        break;
    default:
        break;
    }

    selectedIcons.updateImage(currentButtonState);
    emitAnimationButtonPressedCallback(currentButtonState);
}
