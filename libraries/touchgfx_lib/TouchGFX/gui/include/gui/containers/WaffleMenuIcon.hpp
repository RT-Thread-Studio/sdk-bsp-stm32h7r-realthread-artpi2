#ifndef WAFFLEMENUICON_HPP
#define WAFFLEMENUICON_HPP

#include <gui_generated/containers/WaffleMenuIconBase.hpp>

class WaffleMenuIcon : public WaffleMenuIconBase
{
public:
    WaffleMenuIcon();
    virtual ~WaffleMenuIcon() {}

    virtual void initialize();

    void setupTheIcon(Bitmap);

    void startTheShakeAnimation();

protected:

private:
    touchgfx::Callback < WaffleMenuIcon, const touchgfx::AnimationTextureMapper& > step1ShakeAnimationEndedCallback;
    touchgfx::Callback < WaffleMenuIcon, const touchgfx::AnimationTextureMapper& > step2ShakeAnimationEndedCallback;
    touchgfx::Callback < WaffleMenuIcon, const touchgfx::AnimationTextureMapper& > step3ShakeAnimationEndedCallback;

    void step1ShakeAnimationEndedCallbackHandler(const touchgfx::AnimationTextureMapper& comp);
    void step2ShakeAnimationEndedCallbackHandler(const touchgfx::AnimationTextureMapper& comp);
    void step3ShakeAnimationEndedCallbackHandler(const touchgfx::AnimationTextureMapper& comp);
};

#endif // WAFFLEMENUICON_HPP
