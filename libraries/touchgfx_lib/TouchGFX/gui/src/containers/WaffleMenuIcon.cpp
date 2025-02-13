#include <gui/containers/WaffleMenuIcon.hpp>

WaffleMenuIcon::WaffleMenuIcon():
    step1ShakeAnimationEndedCallback(this, &WaffleMenuIcon::step1ShakeAnimationEndedCallbackHandler),
    step2ShakeAnimationEndedCallback(this, &WaffleMenuIcon::step2ShakeAnimationEndedCallbackHandler),
    step3ShakeAnimationEndedCallback(this, &WaffleMenuIcon::step3ShakeAnimationEndedCallbackHandler)
{

}

void WaffleMenuIcon::initialize()
{
    WaffleMenuIconBase::initialize();
}

void WaffleMenuIcon::setupTheIcon(Bitmap representedIcon)
{
    icon.setBitmap(representedIcon);
    icon.invalidate();
}

void WaffleMenuIcon::startTheShakeAnimation()
{
    //Step 1
    icon.cancelAnimationTextureMapperAnimation();
    icon.setupAnimation(touchgfx::AnimationTextureMapper::Z_ROTATION, 6.400f, 6, 3, touchgfx::EasingEquations::linearEaseIn);
    icon.startAnimation();
    icon.setTextureMapperAnimationEndedAction(step1ShakeAnimationEndedCallback);
}

void WaffleMenuIcon::step1ShakeAnimationEndedCallbackHandler(const touchgfx::AnimationTextureMapper& comp)
{
    //Step 2
    icon.cancelAnimationTextureMapperAnimation();
    icon.setupAnimation(touchgfx::AnimationTextureMapper::Z_ROTATION, 6.200f, 6, 3, touchgfx::EasingEquations::linearEaseIn);
    icon.startAnimation();
    icon.setTextureMapperAnimationEndedAction(step2ShakeAnimationEndedCallback);
}

void WaffleMenuIcon::step2ShakeAnimationEndedCallbackHandler(const touchgfx::AnimationTextureMapper& comp)
{
    //Step 3
    icon.cancelAnimationTextureMapperAnimation();
    icon.setupAnimation(touchgfx::AnimationTextureMapper::Z_ROTATION, 6.400f, 6, 3, touchgfx::EasingEquations::linearEaseIn);
    icon.startAnimation();
    icon.setTextureMapperAnimationEndedAction(step3ShakeAnimationEndedCallback);
}

void WaffleMenuIcon::step3ShakeAnimationEndedCallbackHandler(const touchgfx::AnimationTextureMapper& comp)
{
    //Step 4
    icon.cancelAnimationTextureMapperAnimation();
    icon.setupAnimation(touchgfx::AnimationTextureMapper::Z_ROTATION, 6.300f, 6, 3, touchgfx::EasingEquations::linearEaseIn);
    icon.startAnimation();
}
