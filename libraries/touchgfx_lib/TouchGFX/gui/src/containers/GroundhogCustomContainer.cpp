#include <gui/containers/GroundhogCustomContainer.hpp>

GroundhogCustomContainer::GroundhogCustomContainer() :
    state(DOWN),
    textureMapperAnimationEndedCallback(this, &GroundhogCustomContainer::textureMapperAnimationEndedHandler),
    imageMoveAnimationEndedCallback(this, &GroundhogCustomContainer::imageMoveAnimationEndedHandler)
{

}

void GroundhogCustomContainer::initialize()
{
    GroundhogCustomContainerBase::initialize();

    holeTextureMapper.setTextureMapperAnimationEndedAction(textureMapperAnimationEndedCallback);
    groundhogImage.setMoveAnimationEndedAction(imageMoveAnimationEndedCallback);

    setVisible(false);
    groundhogImage.setY(getHeight());
    holeTextureMapper.setScale(0);
    holeOverlayImage.setVisible(false);
}

void GroundhogCustomContainer::toggle()
{
    if (state == DOWN)
    {
        state = ASCENDING;
        setVisible(true);
        holeTextureMapper.setupAnimation(AnimationTextureMapper::SCALE, 1.0f, DURATION_PART_ANIMATION, 0, EasingEquations::linearEaseIn);
        holeTextureMapper.startAnimation();
    }
    else if (state == UP)
    {
        state = DESCENDING;
        groundhogImage.startMoveAnimation(groundhogImage.getX(), groundhogContainer.getHeight(), DURATION_PART_ANIMATION, EasingEquations::quadEaseIn, EasingEquations::quadEaseIn);
    }
}

void GroundhogCustomContainer::textureMapperAnimationEndedHandler(const AnimationTextureMapper& src)
{
    if (&src == &holeTextureMapper)
    {
        if (state == ASCENDING)
        {
            holeOverlayImage.setVisible(true);
            groundhogImage.startMoveAnimation(groundhogImage.getX(), POSITION_GROUNDHOG_UP, DURATION_PART_ANIMATION, EasingEquations::linearEaseIn, EasingEquations::backEaseOut);
        }
        else if (state == DESCENDING)
        {
            state = DOWN;
            setVisible(false);
        }
    }
}

void GroundhogCustomContainer::imageMoveAnimationEndedHandler(const touchgfx::MoveAnimator<Image>& comp)
{
    if (&comp == &groundhogImage)
    {
        if (state == ASCENDING)
        {
            state = UP;
        }
        else if (state == DESCENDING)
        {
            holeOverlayImage.setVisible(false);
            holeTextureMapper.setupAnimation(AnimationTextureMapper::SCALE, 0.0f, DURATION_PART_ANIMATION, 0, EasingEquations::linearEaseIn);
            holeTextureMapper.startAnimation();
        }
    }
}
