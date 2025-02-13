#include "gui/Transitions/CoverTransition.hpp"
#include "touchgfx/EasingEquations.hpp"

CoverTransition::CoverTransition()
{
}

CoverTransition::~CoverTransition()
{
}

void CoverTransition::setupTranstion(Bitmap start, Bitmap end)
{
    startImage.setXY(0, 0);
    endImage.setXY(-800, 0);

    startImage.setBitmap(touchgfx::Bitmap(start));
    endImage.setBitmap(touchgfx::Bitmap(end));

    add(startImage);
    add(endImage);

    startImage.invalidate();
    endImage.invalidate();
}

void CoverTransition::animationTick(uint16_t step, uint16_t duration)
{
    int newX = EasingEquations::cubicEaseOut(step, 0, 800, duration);
    endImage.setX(newX - 800);
    endImage.invalidate();
}

uint16_t CoverTransition::endTransition()
{
    remove(endImage);
    remove(startImage);

    return endImage.getBitmap();
}
