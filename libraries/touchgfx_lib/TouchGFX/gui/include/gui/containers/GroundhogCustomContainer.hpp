#ifndef GROUNDHOGCUSTOMCONTAINER_HPP
#define GROUNDHOGCUSTOMCONTAINER_HPP

#include <gui_generated/containers/GroundhogCustomContainerBase.hpp>

class GroundhogCustomContainer : public GroundhogCustomContainerBase
{
public:
    GroundhogCustomContainer();
    virtual ~GroundhogCustomContainer() {}

    virtual void initialize();

    void toggle();

    virtual void textureMapperAnimationEndedHandler(const AnimationTextureMapper& src);
    virtual void imageMoveAnimationEndedHandler(const touchgfx::MoveAnimator<Image>& comp);

private:
    enum GroundhogState
    {
        DOWN,
        ASCENDING,
        UP,
        DESCENDING
    };

    const uint16_t DURATION_PART_ANIMATION = 30;
    const int16_t POSITION_GROUNDHOG_UP = 3; // this offset allows the groundhog to stay inside the container, despite the overshoot (backEaseOut)

    GroundhogState state;

    //Callback for Texture mapper : triggered when animation ends
    Callback<GroundhogCustomContainer, const AnimationTextureMapper&> textureMapperAnimationEndedCallback;

    //Callback for Image : triggered when move animatiom ends
    Callback <GroundhogCustomContainer, const touchgfx::MoveAnimator<Image>&> imageMoveAnimationEndedCallback;
};

#endif // GROUNDHOGCUSTOMCONTAINER_HPP
