#include <gui/containers/rotatingCoverFlowCustomContainer.hpp>

rotatingCoverFlowCustomContainer::rotatingCoverFlowCustomContainer() :
    textureMapperAnimationEndedCallback(this, &rotatingCoverFlowCustomContainer::textureMapperAnimationEndedHandler)
{

}

void rotatingCoverFlowCustomContainer::initialize()
{
    rotatingCoverFlowCustomContainerBase::initialize();
    backgroundTextureMapper.setTextureMapperAnimationEndedAction(textureMapperAnimationEndedCallback);

    // Set the animation texturemapper to its starting position
    backgroundTextureMapper.setXY(BACKGROUND_START_X, BACKGROUND_START_Y);
    backgroundTextureMapper.setScale(BACKGROUND_START_SCALE);
    backgroundTextureMapper.setAngles(0, 0, BACKGROUND_START_ANGLE);

    // Hide all the contents
    closeCoverFlowButton.setVisible(false);
    scrollWheelCoverFlow1.setVisible(false);
    backgroundImage.setVisible(false);
}

void rotatingCoverFlowCustomContainer::open()
{
    // Make this coverflow custom container visible
    setVisible(true);

    // Make the coverflow background scale up, rotate and move to the center of the screen
    backgroundTextureMapper.setupAnimation(AnimationTextureMapper::SCALE, BACKGROUND_END_SCALE, ROTATION_DURATION, 0, EasingEquations::cubicEaseIn);
    backgroundTextureMapper.setupAnimation(AnimationTextureMapper::Z_ROTATION, BACKGROUND_END_ANGLE, ROTATION_DURATION, 0, EasingEquations::linearEaseIn);
    backgroundTextureMapper.startAnimation();
    backgroundTextureMapper.startMoveAnimation(BACKGROUND_END_X, BACKGROUND_END_Y, ROTATION_DURATION);
}

void rotatingCoverFlowCustomContainer::close()
{
    // Hide the contents of the coverflow window
    closeCoverFlowButton.setVisible(false);
    scrollWheelCoverFlow1.setVisible(false);
    backgroundImage.setVisible(false);
    backgroundImage.invalidate();

    // Make the coverflow background scale down, rotate and move to back to its starting position
    backgroundTextureMapper.setVisible(true);
    backgroundTextureMapper.setupAnimation(AnimationTextureMapper::SCALE, BACKGROUND_START_SCALE, ROTATION_DURATION, 0, EasingEquations::cubicEaseOut);
    backgroundTextureMapper.setupAnimation(AnimationTextureMapper::Z_ROTATION, BACKGROUND_START_ANGLE, ROTATION_DURATION, 0, EasingEquations::linearEaseIn);
    backgroundTextureMapper.startAnimation();
    backgroundTextureMapper.startMoveAnimation(BACKGROUND_START_X, BACKGROUND_START_Y, ROTATION_DURATION);
}

void rotatingCoverFlowCustomContainer::textureMapperAnimationEndedHandler(const AnimationTextureMapper& src)
{
    // This is executed when the coverflow background open/close trasition is done
    if (backgroundTextureMapper.getScale() > BACKGROUND_START_SCALE)
    {
        // If the open transition just completed, hide the texture mapper and show the content
        backgroundTextureMapper.setVisible(false);
        closeCoverFlowButton.setVisible(true);
        scrollWheelCoverFlow1.setVisible(true);
        backgroundImage.setVisible(true);
        backgroundImage.invalidate();
    }
    else
    {
        // If the close transition just completed, this entire coverflow custom container
        setVisible(false);
    }
}
