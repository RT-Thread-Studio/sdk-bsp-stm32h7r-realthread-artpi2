#include <gui/containers/AdvanceDiceContainer_DiceDemo.hpp>

#include <images/BitmapDatabase.hpp>
#include <math.h>

AdvanceDiceContainer_DiceDemo::AdvanceDiceContainer_DiceDemo() :
    stepCounterLeft(-1), stepCounterRight(-1),
    stepCounterUp(-1), stepCounterDown(-1),
    stepCounterEnter(-1), reverseSpin(false), animating(false)
{

}

static const float PI_HALF = PI / 2;
static const float PI_QUARTER = PI / 4;

void AdvanceDiceContainer_DiceDemo::initialize()
{
    AdvanceDiceContainer_DiceDemoBase::initialize();

    cubeMap[TOP] = BITMAP_DICE_LARGE_1_ID;
    cubeMap[LEFT_FRONT] = BITMAP_DICE_LARGE_5_ID;
    cubeMap[RIGHT_FRONT] = BITMAP_DICE_LARGE_4_ID;
    cubeMap[LEFT_BACK] = BITMAP_DICE_LARGE_2_ID;
    cubeMap[RIGHT_BACK] = BITMAP_DICE_LARGE_3_ID;
    cubeMap[BOTTOM] = BITMAP_DICE_LARGE_6_ID;

    for (int i = 0; i < NUMBER_OF_DICE_SIDES; i++)
    {
        diceSides[i].setXY(0, 0);
        diceSides[i].setScale(0.5f);
        diceSides[i].setCameraDistance(1000.000f);
        diceSides[i].setOrigo(400.000f, 265.000f, 1180.0f);
        diceSides[i].setCamera(400.000f, 240.000f);
        diceSides[i].setRenderingAlgorithm(touchgfx::TextureMapper::BILINEAR_INTERPOLATION);

        switch (i)
        {
        case 0:
            diceSides[i].updateAngles(0.000f, 0.000f, 0.000f);
            diceSides[i].setBitmap(touchgfx::Bitmap(cubeMap[LEFT_FRONT]));
            break;
        case 1:
            diceSides[i].updateAngles(0.000f, -PI_HALF, 0.000f);
            diceSides[i].setBitmap(touchgfx::Bitmap(cubeMap[RIGHT_FRONT]));
            break;
        case 2:
            diceSides[i].updateAngles(-PI_HALF, 0.000f, 0.000f);
            diceSides[i].setBitmap(touchgfx::Bitmap(cubeMap[TOP]));
            break;
        }

        diceSides[i].setWidth(800);
        diceSides[i].setHeight(480);
        diceSides[i].setBitmapPosition(220.000f, 60.000f);
        add(diceSides[i]);
    }

    reorderCube();
}

void AdvanceDiceContainer_DiceDemo::handleTickEvent()
{
    if (stepCounterRight > -1)
    {
        float newAngleY = floatBackEaseInOut((float)stepCounterRight, startValueY, finalValueY, (float)steps);
        for (int i = 0; i < NUMBER_OF_DICE_SIDES; i++)
        {
            diceSides[i].invalidateContent();

            switch (i)
            {
            case 0:
            case 1:
                if (reverseSpin)
                {
                    diceSides[i].updateAngles(diceSides[i].getXAngle(), diceSides[i].getYAngle() + (newAngleY - previousY), diceSides[i].getZAngle());
                }
                else
                {
                    diceSides[i].updateAngles(diceSides[i].getXAngle(), diceSides[i].getYAngle() - (newAngleY - previousY), diceSides[i].getZAngle());
                }

                if (diceSides[i].getYAngle() > PI_HALF)
                {
                    updateRightToLeft(diceSides[i]);
                }
                else if (diceSides[i].getYAngle() < -PI_HALF)
                {
                    updateLeftToRight(diceSides[i]);
                }

                break;
            default:
                diceSides[i].updateAngles(diceSides[i].getXAngle(), diceSides[i].getYAngle() - (newAngleY - previousY), diceSides[i].getZAngle());
                break;
            }
            diceSides[i].invalidateContent();
        }
        reorderCube();
        resetCubeSidesAngles();
        remove(diceSides[2]);
        add(diceSides[2]);
        previousY = newAngleY;
        stepCounterRight++;
        if (stepCounterRight > steps)
        {
            stepCounterRight = -1;
            Application::getInstance()->unregisterTimerWidget(this);
            resetCubeSidesAngles();
            animating = false;
            emitAnimationDoneCallback();
        }
    }
    else if (stepCounterLeft > -1)
    {
        float newAngleY = floatBackEaseInOut((float)stepCounterLeft, startValueY, finalValueY, (float)steps);
        for (int i = 0; i < NUMBER_OF_DICE_SIDES; i++)
        {
            diceSides[i].invalidateContent();
            switch (i)
            {
            case 0:
            case 1:
                if (reverseSpin)
                {
                    diceSides[i].updateAngles(diceSides[i].getXAngle(), diceSides[i].getYAngle() + (newAngleY - previousY), diceSides[i].getZAngle());
                }
                else
                {
                    diceSides[i].updateAngles(diceSides[i].getXAngle(), diceSides[i].getYAngle() - (newAngleY - previousY), diceSides[i].getZAngle());
                }

                if (diceSides[i].getYAngle() > PI_HALF)
                {
                    updateRightToLeft(diceSides[i]);
                }
                else if (diceSides[i].getYAngle() < -PI_HALF)
                {
                    updateLeftToRight(diceSides[i]);
                }
                break;
            default:
                diceSides[i].updateAngles(diceSides[i].getXAngle(), diceSides[i].getYAngle() - (newAngleY - previousY), diceSides[i].getZAngle());
                break;
            }
            diceSides[i].invalidateContent();
        }
        reorderCube();
        remove(diceSides[2]);
        add(diceSides[2]);

        previousY = newAngleY;
        stepCounterLeft++;
        if (stepCounterLeft > steps)
        {
            stepCounterLeft = -1;
            Application::getInstance()->unregisterTimerWidget(this);
            resetCubeSidesAngles();
            animating = false;
            emitAnimationDoneCallback();
        }
    }
    else if (stepCounterUp > -1)
    {
        float newAngleZ = floatBackEaseInOut((float)stepCounterUp, startValueZ, finalValueZ, (float)steps);
        for (int i = 0; i < NUMBER_OF_DICE_SIDES; i++)
        {
            diceSides[i].updateAngles(diceSides[i].getXAngle(), diceSides[i].getYAngle(), diceSides[i].getZAngle() + (newAngleZ - previousZ));

            if (diceSides[i].getZAngle() > PI_HALF && i == 2)
            {
                updateUpToDown(diceSides[i]);
            }
        }
        reorderCube();
        previousZ = newAngleZ;
        stepCounterUp++;
        if (stepCounterUp > steps)
        {
            reverseSpin = !reverseSpin;
            stepCounterUp = -1;
            Application::getInstance()->unregisterTimerWidget(this);
            resetCubeSidesAngles();
            animating = false;
            emitAnimationDoneCallback();
        }
    }

    else if (stepCounterDown > -1)
    {
        float newAngleZ = floatBackEaseInOut((float)stepCounterDown, startValueZ, finalValueZ, (float)steps);
        for (int i = 0; i < NUMBER_OF_DICE_SIDES; i++)
        {
            diceSides[i].updateAngles(diceSides[i].getXAngle(), diceSides[i].getYAngle(), diceSides[i].getZAngle() + (newAngleZ - previousZ));

            if (diceSides[i].getZAngle() < -PI_HALF && diceSides[i].getBitmap().getId() == cubeMap[TOP])
            {
                updateDownToUp(diceSides[i]);
            }
        }
        reorderCube();
        previousZ = newAngleZ;
        stepCounterDown++;
        if (stepCounterDown > steps)
        {
            reverseSpin = !reverseSpin;
            stepCounterDown = -1;
            Application::getInstance()->unregisterTimerWidget(this);
            resetCubeSidesAngles();
            animating = false;
            emitAnimationDoneCallback();
        }
    }
    else if (stepCounterEnter > -1)
    {
        float newAngleY = floatBackEaseInOut((float)stepCounterEnter, startValueY, finalValueY, (float)steps);
        float newCamera = floatBackEaseInOut((float)stepCounterEnter, startCameraValueY, distanceCameraValue, (float)steps);
        float newOrigoY = floatBackEaseInOut((float)stepCounterEnter, startOrigoY, distanceOrigoBitmapY, (float)steps);
        newBitmapY = floatBackEaseInOut((float)stepCounterEnter, startBitmapY, distanceOrigoBitmapY, (float)steps);

        for (int i = 0; i < NUMBER_OF_DICE_SIDES; i++)
        {
            if (reverseSpin && i == 2)
            {
                diceSides[i].updateAngles(diceSides[i].getXAngle(), diceSides[i].getYAngle() - (newAngleY - previousY), diceSides[i].getZAngle());
            }
            else
            {
                diceSides[i].updateAngles(diceSides[i].getXAngle(), diceSides[i].getYAngle() + (newAngleY - previousY), diceSides[i].getZAngle());
            }
            diceSides[i].invalidateContent();

            diceSides[i].setCamera(diceSides[i].getCameraX(), newCamera);
            diceSides[i].setOrigo(diceSides[i].getOrigoX(), newOrigoY);
            diceSides[i].setBitmapPosition(diceSides[i].getBitmapPositionX(), newBitmapY);
            diceSides[i].invalidateContent();
        }
        reorderCube();

        previousY = newAngleY;
        stepCounterEnter++;
        if (stepCounterEnter > steps)
        {
            stepCounterEnter = -1;
            Application::getInstance()->unregisterTimerWidget(this);
            resetCubeSidesAngles();
            animating = false;
            emitEnterAnimationEndedCallback();
        }
    }
    else if (stepCounterExit > -1)
    {
        float newAngleY = floatBackEaseInOut((float)stepCounterExit, startValueY, finalValueY, (float)steps);
        float newCamera = floatBackEaseInOut((float)stepCounterExit, startCameraValueY, distanceCameraValue, (float)steps);
        float newOrigoY = floatBackEaseInOut((float)stepCounterExit, startOrigoY, distanceOrigoBitmapY, (float)steps);
        newBitmapY = floatBackEaseInOut((float)stepCounterExit, startBitmapY, distanceOrigoBitmapY, (float)steps);
        for (int i = 0; i < NUMBER_OF_DICE_SIDES; i++)
        {
            if (reverseSpin && i == 2)
            {
                diceSides[i].updateAngles(diceSides[i].getXAngle(), diceSides[i].getYAngle() - (newAngleY - previousY), diceSides[i].getZAngle());
            }
            else
            {
                diceSides[i].updateAngles(diceSides[i].getXAngle(), diceSides[i].getYAngle() + (newAngleY - previousY), diceSides[i].getZAngle());
            }
            diceSides[i].invalidateContent();
            diceSides[i].setCamera(diceSides[i].getCameraX(), newCamera);
            diceSides[i].setOrigo(diceSides[i].getOrigoX(), newOrigoY);
            diceSides[i].setBitmapPosition(diceSides[i].getBitmapPositionX(), newBitmapY);
            diceSides[i].invalidateContent();
        }
        reorderCube();

        previousY = newAngleY;
        stepCounterExit++;
        if (stepCounterExit > steps)
        {
            stepCounterExit = -1;
            Application::getInstance()->unregisterTimerWidget(this);
            resetCubeSidesAngles();
            animating = false;
            emitExitAnimationEndedCallback();
        }
    }
}


void AdvanceDiceContainer_DiceDemo::left()
{
    animating = true;
    steps = 100;
    previousY = 0;
    startValueY = 0;
    finalValueY = -2 * PI - PI_HALF;
    stepCounterLeft = 0;
    Application::getInstance()->registerTimerWidget(this);
}

void AdvanceDiceContainer_DiceDemo::right()
{
    animating = true;
    steps = 100;
    previousY = 0;
    startValueY = 0;
    finalValueY = 2 * PI - PI_HALF;
    stepCounterRight = 0;
    Application::getInstance()->registerTimerWidget(this);
}

void AdvanceDiceContainer_DiceDemo::up()
{
    animating = true;
    steps = 100;
    previousZ = 0;
    startValueZ = 0;
    finalValueZ = 3 * PI;
    stepCounterUp = 0;
    Application::getInstance()->registerTimerWidget(this);
}

void AdvanceDiceContainer_DiceDemo::down()
{
    animating = true;
    steps = 100;
    previousZ = 0;
    startValueZ = 0;
    finalValueZ = -3 * PI;
    stepCounterDown = 0;
    Application::getInstance()->registerTimerWidget(this);
}

void AdvanceDiceContainer_DiceDemo::setScale(float value)
{
    for (int i = 0; i < NUMBER_OF_DICE_SIDES; i++)
    {
        diceSides[i].invalidateContent();
        diceSides[i].setScale(value);
        diceSides[i].invalidateContent();
    }
}

void AdvanceDiceContainer_DiceDemo::reorderCube()
{
    for (int i = 0; i < NUMBER_OF_DICE_SIDES; i++)
    {
        remove(diceSides[i]);
    }

    bool inserted[NUMBER_OF_DICE_SIDES - 1];
    for (int i = 0; i < NUMBER_OF_DICE_SIDES - 1; i++)
    {
        inserted[i] = false;
    }

    int index = -1;

    add(diceSides[NUMBER_OF_DICE_SIDES - 1]);

    for (int i = 0; i < NUMBER_OF_DICE_SIDES - 1; i++)
    {
        float biggest = -10000.0f;
        for (int j = 0; j < NUMBER_OF_DICE_SIDES - 1; j++)
        {
            float zvalue = MAX(MAX(diceSides[j].getZ0(), diceSides[j].getZ1()), MAX(diceSides[j].getZ2(), diceSides[j].getZ3()));
            if (!inserted[j] && zvalue > biggest)
            {
                biggest = zvalue;
                index = j;
            }
        }
        add(diceSides[index]);
        inserted[index] = true;
    }
}

void AdvanceDiceContainer_DiceDemo::updateRightToLeft(TextureMapper& tm)
{
    float tmpNewAngle = tm.getYAngle() - PI;
    if (tmpNewAngle > -PI_HALF && tmpNewAngle < PI_HALF)
    {
        tm.updateYAngle(tmpNewAngle);
    }
    cubeMap[LEFT_FRONT] = cubeMap[RIGHT_FRONT];
    cubeMap[RIGHT_FRONT] = cubeMap[LEFT_BACK];
    cubeMap[LEFT_BACK] = cubeMap[RIGHT_BACK];
    cubeMap[RIGHT_BACK] = tm.getBitmap().getId();
    tm.setBitmap(Bitmap(cubeMap[RIGHT_FRONT]));
    tm.setWidth(800);
    tm.setHeight(480);
}

void AdvanceDiceContainer_DiceDemo::updateLeftToRight(TextureMapper& tm)
{
    tm.updateYAngle(tm.getYAngle() + PI);
    cubeMap[RIGHT_FRONT] = cubeMap[LEFT_FRONT];
    cubeMap[LEFT_FRONT] = cubeMap[RIGHT_BACK];
    cubeMap[RIGHT_BACK] = cubeMap[LEFT_BACK];
    cubeMap[LEFT_BACK] = tm.getBitmap().getId();
    tm.setBitmap(Bitmap(cubeMap[LEFT_FRONT]));
    tm.setWidth(800);
    tm.setHeight(480);
}

void AdvanceDiceContainer_DiceDemo::updateUpToDown(TextureMapper& tm)
{
    tm.updateZAngle(tm.getZAngle() - PI);
    cubeMap[TOP] = cubeMap[BOTTOM];

    cubeMap[BOTTOM] = tm.getBitmap().getId();

    tm.setBitmap(Bitmap(cubeMap[TOP]));
    tm.setWidth(800);
    tm.setHeight(480);
}

void AdvanceDiceContainer_DiceDemo::updateDownToUp(TextureMapper& tm)
{
    tm.updateZAngle(tm.getZAngle() + PI);

    cubeMap[TOP] = cubeMap[BOTTOM];
    cubeMap[BOTTOM] = tm.getBitmap().getId();

    tm.setBitmap(Bitmap(cubeMap[TOP]));
    tm.setWidth(800);
    tm.setHeight(480);
}

void AdvanceDiceContainer_DiceDemo::resetCubeSidesAngles()
{
    for (int i = 0; i < NUMBER_OF_DICE_SIDES; i++)
    {
        diceSides[i].updateXAngle(fmodf(diceSides[i].getXAngle(), 2 * PI));
        diceSides[i].updateYAngle(fmodf(diceSides[i].getYAngle(), 2 * PI));
        diceSides[i].updateZAngle(fmodf(diceSides[i].getZAngle(), 2 * PI));
    }
}

void AdvanceDiceContainer_DiceDemo::setDiceSides(uint16_t leftFrontBitmap)
{
    switch (leftFrontBitmap)
    {
    case BITMAP_DICE_LARGE_5_ID:
        cubeMap[LEFT_FRONT] = BITMAP_DICE_LARGE_5_ID;
        cubeMap[RIGHT_FRONT] = BITMAP_DICE_LARGE_4_ID;
        cubeMap[LEFT_BACK] = BITMAP_DICE_LARGE_2_ID;
        cubeMap[RIGHT_BACK] = BITMAP_DICE_LARGE_3_ID;
        break;
    case BITMAP_DICE_LARGE_4_ID:
        cubeMap[LEFT_FRONT] = BITMAP_DICE_LARGE_4_ID;
        cubeMap[RIGHT_FRONT] = BITMAP_DICE_LARGE_2_ID;
        cubeMap[LEFT_BACK] = BITMAP_DICE_LARGE_3_ID;
        cubeMap[RIGHT_BACK] = BITMAP_DICE_LARGE_5_ID;
        break;
    case BITMAP_DICE_LARGE_2_ID:
        cubeMap[LEFT_FRONT] = BITMAP_DICE_LARGE_2_ID;
        cubeMap[RIGHT_FRONT] = BITMAP_DICE_LARGE_3_ID;
        cubeMap[LEFT_BACK] = BITMAP_DICE_LARGE_5_ID;
        cubeMap[RIGHT_BACK] = BITMAP_DICE_LARGE_4_ID;
        break;
    case BITMAP_DICE_LARGE_3_ID:
        cubeMap[LEFT_FRONT] = BITMAP_DICE_LARGE_3_ID;
        cubeMap[RIGHT_FRONT] = BITMAP_DICE_LARGE_5_ID;
        cubeMap[LEFT_BACK] = BITMAP_DICE_LARGE_4_ID;
        cubeMap[RIGHT_BACK] = BITMAP_DICE_LARGE_2_ID;
        break;
    case BITMAP_DICE_LARGE_1_ID:
        cubeMap[LEFT_FRONT] = BITMAP_DICE_LARGE_1_ID;
        cubeMap[RIGHT_FRONT] = BITMAP_DICE_LARGE_5_ID;
        cubeMap[LEFT_BACK] = BITMAP_DICE_LARGE_6_ID;
        cubeMap[RIGHT_BACK] = BITMAP_DICE_LARGE_2_ID;
        break;
    case BITMAP_DICE_LARGE_6_ID:
        cubeMap[LEFT_FRONT] = BITMAP_DICE_LARGE_6_ID;
        cubeMap[RIGHT_FRONT] = BITMAP_DICE_LARGE_4_ID;
        cubeMap[LEFT_BACK] = BITMAP_DICE_LARGE_1_ID;
        cubeMap[RIGHT_BACK] = BITMAP_DICE_LARGE_3_ID;
        break;
    }

    switch (leftFrontBitmap)
    {
    case BITMAP_DICE_LARGE_5_ID:
    case BITMAP_DICE_LARGE_4_ID:
        cubeMap[TOP] = BITMAP_DICE_LARGE_6_ID;
        cubeMap[BOTTOM] = BITMAP_DICE_LARGE_1_ID;
        break;
    case BITMAP_DICE_LARGE_2_ID:
    case BITMAP_DICE_LARGE_3_ID:
        cubeMap[TOP] = BITMAP_DICE_LARGE_1_ID;
        cubeMap[BOTTOM] = BITMAP_DICE_LARGE_6_ID;
        break;
    case BITMAP_DICE_LARGE_1_ID:
        cubeMap[TOP] = BITMAP_DICE_LARGE_4_ID;
        cubeMap[BOTTOM] = BITMAP_DICE_LARGE_3_ID;
        break;
    case BITMAP_DICE_LARGE_6_ID:
        cubeMap[TOP] = BITMAP_DICE_LARGE_5_ID;
        cubeMap[BOTTOM] = BITMAP_DICE_LARGE_2_ID;
        break;
    }

}

void AdvanceDiceContainer_DiceDemo::enter3Dcube(float simpleDiceAngle)
{
    for (int i = 0; i < NUMBER_OF_DICE_SIDES; i++)
    {
        switch (i)
        {
        case 0:
            diceSides[i].updateAngles(0.f, 0.0f, simpleDiceAngle);
            diceSides[i].setBitmap(touchgfx::Bitmap(cubeMap[LEFT_FRONT]));
            break;
        case 1:
            diceSides[i].updateAngles(0.f, -PI_HALF, simpleDiceAngle);
            diceSides[i].setBitmap(touchgfx::Bitmap(cubeMap[RIGHT_FRONT]));
            break;
        case 2:
            diceSides[i].updateAngles(-PI_HALF, 0.000f, 0.000f);
            diceSides[i].setBitmap(touchgfx::Bitmap(cubeMap[TOP]));
            break;
        }

        diceSides[i].setWidth(800);
        diceSides[i].setHeight(480);
        diceSides[i].setBitmapPosition(220.000f, 85.000f);
    }
    reorderCube();

    reverseSpin = simpleDiceAngle != 0.f ? true : false;

    steps = 100;
    previousY = 0;
    startValueY = 0;
    finalValueY = PI_QUARTER;
    stepCounterEnter = 0;

    startCameraValueY = 240.000f;
    distanceCameraValue = -400.f - 240.f;

    startOrigoY = 265.000f;
    startBitmapY = 85.000f;
    distanceOrigoBitmapY = 100.000f;

    animating = true;
    Application::getInstance()->registerTimerWidget(this);
}

void AdvanceDiceContainer_DiceDemo::exit3Dcube()
{
    steps = 100;
    previousY = 0;
    startValueY = 0;
    finalValueY = -PI_QUARTER;
    stepCounterExit = 0;

    startCameraValueY = -400.000f;
    distanceCameraValue = 400.000f + 240.000f;

    startOrigoY = 365.000f;
    startBitmapY = 185.000f;
    distanceOrigoBitmapY = -100.000f;

    animating = true;
    Application::getInstance()->registerTimerWidget(this);
}

bool AdvanceDiceContainer_DiceDemo::getIsAnimating()
{
    return animating;
}

uint16_t AdvanceDiceContainer_DiceDemo::getLeftFrontBitmapId()
{
    return cubeMap[LEFT_FRONT];
}

float AdvanceDiceContainer_DiceDemo::getFrontBitmapAngle()
{
    return diceSides[0].getZAngle();
}

float AdvanceDiceContainer_DiceDemo::floatBackEaseOut(float t, float b, float c, float d)
{
    float s = 1.70158f;
    t = t / d - 1;
    return c * (t * t * ((s + 1) * t + s) + 1) + b;
}

float AdvanceDiceContainer_DiceDemo::floatBackEaseIn(float t, float b, float c, float d)
{
    float s = 1.70158f;
    float postFix = t /= d;
    return c * (postFix) * t * ((s + 1) * t - s) + b;
}

float AdvanceDiceContainer_DiceDemo::floatBackEaseInOut(float t, float b, float c, float d)
{
    float s = 1.70158f;
    if ((t /= d / 2) < 1)
    {
        s *= (1.525f);
        return c / 2 * (t * t * ((s + 1) * t - s)) + b;
    }
    t -= 2;
    float postFix = t;
    s *= (1.525f);
    return c / 2 * ((postFix) * t * ((s + 1) * t + s) + 2) + b;
}

float AdvanceDiceContainer_DiceDemo::easeNone(float t, float b, float c, float d)
{
    return c * t / d + b;
}

void AdvanceDiceContainer_DiceDemo::invalidateDice()
{
    for (int i = 0; i < NUMBER_OF_DICE_SIDES; i++)
    {
        diceSides[i].invalidateContent();
    }
}
