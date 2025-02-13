#include <gui/containers/SimpleDiceContainer.hpp>
#include <images/BitmapDatabase.hpp>

SimpleDiceContainer::SimpleDiceContainer() :
    animationTextureMapperCallback(this, &SimpleDiceContainer::animationTextureMapperHandler),
    currentSide(0),
    diceCounter(-1)
{
}

void SimpleDiceContainer::initialize()
{
    SimpleDiceContainerBase::initialize();
    cubeSidesBitmapID[0] = BITMAP_DICE_LARGE_1_ID;
    cubeSidesBitmapID[1] = BITMAP_DICE_LARGE_2_ID;
    cubeSidesBitmapID[2] = BITMAP_DICE_LARGE_3_ID;
    cubeSidesBitmapID[3] = BITMAP_DICE_LARGE_4_ID;
    cubeSidesBitmapID[4] = BITMAP_DICE_LARGE_5_ID;
    cubeSidesBitmapID[5] = BITMAP_DICE_LARGE_6_ID;

    singleSideDice.setTextureMapperAnimationEndedAction(animationTextureMapperCallback);
}

void SimpleDiceContainer::handleTickEvent()
{
    if (diceCounter > -1)
    {
        if (diceCounter == ANIMATION_DURATION_DICE_VALUE / 2)
        {
            singleSideDice.setBitmap(Bitmap(cubeSidesBitmapID[currentSide]));
            singleSideDice.setWidth(800);
            singleSideDice.setHeight(480);
        }
        diceCounter++;
    }
}

void SimpleDiceContainer::left()
{
    currentSide = positiveModulo(currentSide - 1, 6);

    singleSideDice.setupAnimation(AnimationTextureMapper::Z_ROTATION, -2 * PI, ANIMATION_DURATION, 0, EasingEquations::backEaseInOut);
    singleSideDice.startAnimation();

    diceCounter = 0;
    Application::getInstance()->registerTimerWidget(this);
}

void SimpleDiceContainer::right()
{
    currentSide = positiveModulo(currentSide + 1, 6);

    singleSideDice.setupAnimation(AnimationTextureMapper::Z_ROTATION, 2 * PI, ANIMATION_DURATION, 0, EasingEquations::backEaseInOut);
    singleSideDice.startAnimation();

    diceCounter = 0;
    Application::getInstance()->registerTimerWidget(this);
}

void SimpleDiceContainer::setScale(float value)
{
    singleSideDice.invalidateContent();
    singleSideDice.setScale(value);
    singleSideDice.invalidateContent();
}

int SimpleDiceContainer::positiveModulo(int i, int n)
{
    return (n + (i % n)) % n;
}

bool SimpleDiceContainer::getIsAnimating()
{
    return singleSideDice.isTextureMapperAnimationRunning();
}

void SimpleDiceContainer::setVisibleBitmap(uint16_t bitmapId)
{
    for (int i = 0; i < 6; i++)
    {
        if (cubeSidesBitmapID[i] == bitmapId)
        {
            currentSide = i;
        }
    }

    singleSideDice.setBitmap(Bitmap(cubeSidesBitmapID[currentSide]));
    singleSideDice.setWidth(800);
    singleSideDice.setHeight(480);

    singleSideDice.invalidateContent();
}

uint16_t SimpleDiceContainer::getVisibleBitmapId()
{
    return cubeSidesBitmapID[currentSide];
}

void SimpleDiceContainer::setZAngle(float zAngle)
{
    singleSideDice.updateZAngle(zAngle);
}

float SimpleDiceContainer::getZAngle()
{
    return singleSideDice.getZAngle();
}


void SimpleDiceContainer::animationTextureMapperHandler(const AnimationTextureMapper& src)
{
    singleSideDice.updateZAngle(0);

    diceCounter = -1;
    Application::getInstance()->unregisterTimerWidget(this);

    emitAnimationDoneCallback();
}

void SimpleDiceContainer::invalidateDice()
{
    singleSideDice.invalidateContent();
}
