#ifndef SIMPLEDICECONTAINER_HPP
#define SIMPLEDICECONTAINER_HPP

#include <gui_generated/containers/SimpleDiceContainerBase.hpp>

class SimpleDiceContainer : public SimpleDiceContainerBase
{
public:
    SimpleDiceContainer();
    virtual ~SimpleDiceContainer() {}

    virtual void initialize();

    virtual void handleTickEvent();

    virtual void left();
    virtual void right();
    virtual void setScale(float value);
    int positiveModulo(int i, int n);

    bool getIsAnimating();
    void setVisibleBitmap(uint16_t bitmapId);
    uint16_t getVisibleBitmapId();

    void setZAngle(float zAngle);
    float getZAngle();

    void invalidateDice();
protected:
    Callback<SimpleDiceContainer, const AnimationTextureMapper&> animationTextureMapperCallback;
    void animationTextureMapperHandler(const AnimationTextureMapper& src);

    static const int ANIMATION_DURATION = 100;
    static const int ANIMATION_DURATION_DICE_VALUE = ANIMATION_DURATION - (ANIMATION_DURATION / 8);

    uint16_t cubeSidesBitmapID[6];
    int currentSide;

    int16_t diceCounter;
};

#endif // SIMPLEDICECONTAINER_HPP
