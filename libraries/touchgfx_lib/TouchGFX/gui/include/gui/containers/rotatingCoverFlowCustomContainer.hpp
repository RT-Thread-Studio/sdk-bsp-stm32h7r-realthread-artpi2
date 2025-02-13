#ifndef ROTATINGCOVERFLOWCUSTOMCONTAINER_HPP
#define ROTATINGCOVERFLOWCUSTOMCONTAINER_HPP

#include <gui_generated/containers/rotatingCoverFlowCustomContainerBase.hpp>

class rotatingCoverFlowCustomContainer : public rotatingCoverFlowCustomContainerBase
{
public:
    rotatingCoverFlowCustomContainer();
    virtual ~rotatingCoverFlowCustomContainer() {}
    virtual void initialize();

    virtual void open();
    virtual void close();
    virtual void textureMapperAnimationEndedHandler(const AnimationTextureMapper& src);

protected:
    const uint16_t ROTATION_DURATION = 75;
    const int16_t BACKGROUND_START_X = 296 - 800 / 2; //To make the animation of the background animation start at the truck
    const int16_t BACKGROUND_END_X = 0;
    const int16_t BACKGROUND_START_Y = 357 - 800 / 2; //To make the animation of the background animation start at the truck
    const int16_t BACKGROUND_END_Y = -129;
    const float BACKGROUND_START_SCALE = 0.0f;
    const float BACKGROUND_END_SCALE = 1.0f;
    const float BACKGROUND_START_ANGLE = -6.28f;
    const float BACKGROUND_END_ANGLE = 0.0f;

    Callback<rotatingCoverFlowCustomContainer, const AnimationTextureMapper&> textureMapperAnimationEndedCallback;
};

#endif // ROTATINGCOVERFLOWCUSTOMCONTAINER_HPP
