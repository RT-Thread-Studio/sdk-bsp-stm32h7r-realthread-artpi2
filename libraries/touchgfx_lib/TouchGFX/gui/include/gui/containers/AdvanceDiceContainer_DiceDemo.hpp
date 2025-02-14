#ifndef ADVANCEDICECONTAINER_DICEDEMO_HPP
#define ADVANCEDICECONTAINER_DICEDEMO_HPP

#include <gui_generated/containers/AdvanceDiceContainer_DiceDemoBase.hpp>

#include "touchgfx/widgets/TextureMapper.hpp"

class AdvanceDiceContainer_DiceDemo : public AdvanceDiceContainer_DiceDemoBase
{
public:
    AdvanceDiceContainer_DiceDemo();
    virtual ~AdvanceDiceContainer_DiceDemo() {}

    virtual void initialize();

    virtual void handleTickEvent();

    virtual void left();
    virtual void right();
    virtual void up();
    virtual void down();
    virtual void setScale(float value);

    void reorderCube();
    void updateRightToLeft(TextureMapper& tm);
    void updateLeftToRight(TextureMapper& tm);
    void updateUpToDown(TextureMapper& tm);
    void updateDownToUp(TextureMapper& tm);

    void resetCubeSidesAngles();
    void setDiceSides(uint16_t leftFrontBitmap);

    void enter3Dcube(float simpleDiceAngle);
    void exit3Dcube();
    bool getIsAnimating();
    uint16_t getLeftFrontBitmapId();
    float getFrontBitmapAngle();

    float floatBackEaseOut(float t, float b, float c, float d);
    float floatBackEaseIn(float t, float b, float c, float d);
    float floatBackEaseInOut(float t, float b, float c, float d);
    float easeNone(float t, float b, float c, float d);

    void invalidateDice();
protected:
    enum DICE_SIDES
    {
        TOP,
        LEFT_FRONT,
        RIGHT_FRONT,
        LEFT_BACK,
        RIGHT_BACK,
        BOTTOM
    };

    static const int NUMBER_OF_DICE_SIDES = 3;

    TextureMapper diceSides[NUMBER_OF_DICE_SIDES];
    uint16_t cubeMap[6];

    int stepCounterLeft;
    int stepCounterRight;
    int stepCounterUp;
    int stepCounterDown;
    int stepCounterEnter;
    int stepCounterExit;

    int steps;

    float previousX;
    float previousY;
    float previousZ;

    float startValueX;
    float startValueY;
    float startValueZ;

    float startCameraValueY;
    float startOrigoY;
    float startBitmapY;

    float finalValueX;
    float finalValueY;
    float finalValueZ;

    float distanceCameraValue;
    float distanceOrigoBitmapY;

    float newBitmapY;

    bool reverseSpin;

    bool animating;
};

#endif // ADVANCEDICECONTAINER_DICEDEMO_HPP
