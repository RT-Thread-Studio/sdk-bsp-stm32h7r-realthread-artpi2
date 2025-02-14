#ifndef SIMPLEDICECONTAINER_DICEDEMO_HPP
#define SIMPLEDICECONTAINER_DICEDEMO_HPP

#include <gui_generated/containers/SimpleDiceContainer_DiceDemoBase.hpp>

class SimpleDiceContainer_DiceDemo : public SimpleDiceContainer_DiceDemoBase
{
public:
    SimpleDiceContainer_DiceDemo();
    virtual ~SimpleDiceContainer_DiceDemo() {}

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
    Callback<SimpleDiceContainer_DiceDemo, const AnimationTextureMapper&> animationTextureMapperCallback;
    void animationTextureMapperHandler(const AnimationTextureMapper& src);

    static const int ANIMATION_DURATION = 100;
    static const int ANIMATION_DURATION_DICE_VALUE = ANIMATION_DURATION - (ANIMATION_DURATION / 8);

    uint16_t cubeSidesBitmapID[6];
    int currentSide;

    int16_t diceCounter;
};

#endif // SIMPLEDICECONTAINER_DICEDEMO_HPP
