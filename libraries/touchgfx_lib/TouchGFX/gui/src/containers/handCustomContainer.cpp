#include <gui/containers/handCustomContainer.hpp>
#include <math.h>

handCustomContainer::handCustomContainer():
    active(false),
    count(0)
{

}

void handCustomContainer::initialize()
{
    handCustomContainerBase::initialize();

    handTextureMapper.setVisible(false);
}

void handCustomContainer::activate()
{
    active = true;
    handTextureMapper.setVisible(true);
}

void handCustomContainer::tick()
{
    if (active)
    {
        count++;

        if (count < FADE_DURATION)
        {
            handTextureMapper.setAlpha(count * 255 / FADE_DURATION);
            handTextureMapper.setScale(1);
        }
        else if (count < FADE_DURATION + WAIT_DURATION)
        {
            handTextureMapper.setAlpha(255);
            handTextureMapper.setScale(1);
        }
        else if (count < FADE_DURATION + WAIT_DURATION + GROW_DURATION)
        {
            handTextureMapper.setAlpha(255);

            const float x = 3.14f * (count - FADE_DURATION - WAIT_DURATION) / GROW_DURATION;
            handTextureMapper.setScale(1.0f - 0.3f * float(sin(x)));
        }
        else if (count < FADE_DURATION + WAIT_DURATION + GROW_DURATION + WAIT_DURATION)
        {
            handTextureMapper.setAlpha(255);
            handTextureMapper.setScale(1);
        }
        else if (count < FADE_DURATION + WAIT_DURATION + GROW_DURATION + WAIT_DURATION + FADE_DURATION)
        {
            handTextureMapper.setAlpha((FADE_DURATION + WAIT_DURATION + GROW_DURATION + WAIT_DURATION + FADE_DURATION - count) * 255 / FADE_DURATION);
            handTextureMapper.setScale(1);
        }
        else
        {
            active = false;
            handTextureMapper.setVisible(false);
            count = 0;
        }
        invalidate();
    }
}
