#include <gui/containers/animationsDepthButton.hpp>

animationsDepthButton::animationsDepthButton() :
    currentButtonState(STATE_2D)
{

}

void animationsDepthButton::initialize()
{
    animationsDepthButtonBase::initialize();
}

void animationsDepthButton::touchOverlayPressed()
{
    switch (currentButtonState)
    {
    case animationsDepthButton::STATE_2D:
        selectedIcons.startMoveAnimation(BUTTON_3D_X, BUTTON_3D_Y, BUTTON_ANIMATION_DURATION, EasingEquations::cubicEaseOut, EasingEquations::cubicEaseOut);
        currentButtonState = STATE_3D;
        break;
    case animationsDepthButton::STATE_3D:
        selectedIcons.startMoveAnimation(BUTTON_2D_X, BUTTON_2D_Y, BUTTON_ANIMATION_DURATION, EasingEquations::cubicEaseOut, EasingEquations::cubicEaseOut);
        currentButtonState = STATE_2D;
        break;
    default:
        break;
    }

    selectedIcons.updateImage(currentButtonState);
    emitAnimationButtonPressedCallback(currentButtonState);
}
