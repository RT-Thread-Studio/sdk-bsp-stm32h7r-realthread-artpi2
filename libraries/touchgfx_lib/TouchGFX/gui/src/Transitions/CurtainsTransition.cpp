#include "gui/Transitions/CurtainsTransition.hpp"
#include "touchgfx/EasingEquations.hpp"

CurtainsTransition::CurtainsTransition()
{
}

CurtainsTransition::~CurtainsTransition()
{
}

void CurtainsTransition::setupTranstion(Bitmap start, Bitmap end)
{
    startImage.setXY(0, 0);
    startImage.setBitmap(Bitmap(start));
    add(startImage);

    endImageLeft.setXY(0, 0);
    endImageLeft.setBitmap(Bitmap(end));
    containerLeft.setPosition(0, 0, 0, 430);
    containerLeft.add(endImageLeft);

    endImageRight.setXY(-800, 0);
    endImageRight.setBitmap(Bitmap(end));
    containerRight.setPosition(800, 0, 0, 430);
    containerRight.add(endImageRight);

    add(containerLeft);
    add(containerRight);
}

void CurtainsTransition::animationTick(uint16_t step, uint16_t duration)
{
    int newWidth = EasingEquations::bounceEaseOut(step, 0, 400, duration);

    containerLeft.invalidate();

    containerLeft.setWidth(newWidth);

    containerLeft.invalidate();

    containerRight.invalidate();

    containerRight.setWidth(newWidth);
    containerRight.setX(800 - newWidth);
    endImageRight.setX(newWidth - 800);

    containerRight.invalidate();
}

uint16_t CurtainsTransition::endTransition()
{
    containerLeft.remove(endImageLeft);
    containerRight.remove(endImageRight);

    remove(startImage);
    remove(containerLeft);
    remove(containerRight);
    return endImageLeft.getBitmap();
}
