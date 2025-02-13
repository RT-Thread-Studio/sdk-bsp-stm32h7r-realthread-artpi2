#include "gui\Transitions\SlideTransition.hpp"
#include "touchgfx/EasingEquations.hpp"

SlideTransition::SlideTransition()
{
}

SlideTransition::~SlideTransition()
{
}

void SlideTransition::setupTranstion(Bitmap start, Bitmap end)
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

void SlideTransition::animationTick(uint16_t step, uint16_t duration)
{
    int newX = EasingEquations::cubicEaseOut(step, 0, 800, duration);

    startImage.setX(newX);
    startImage.invalidate();

    endImage.setX(newX - 800);
    endImage.invalidate();
}

uint16_t SlideTransition::endTransition()
{
    remove(endImage);
    remove(startImage);

    return endImage.getBitmap();
}
