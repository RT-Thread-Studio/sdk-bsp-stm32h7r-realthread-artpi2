#include "gui/Transitions/SpinTransition.hpp"
#include <images/BitmapDatabase.hpp>
#include <gui/common/FloatEasingEquations.hpp>
#include "touchgfx/EasingEquations.hpp"

SpinTransition::SpinTransition()
{
}

SpinTransition::~SpinTransition()
{
}

void SpinTransition::setupTranstion(Bitmap start, Bitmap end)
{
    endTextureMapper.setXY(0, 0);
    endTextureMapper.setBitmap(touchgfx::Bitmap(end));
    endTextureMapper.setWidth(800);
    endTextureMapper.setHeight(430);
    endTextureMapper.setBitmapPosition(0.000f, 0.000f);
    endTextureMapper.setScale(1.000f);
    endTextureMapper.setCameraDistance(1000.000f);
    endTextureMapper.setOrigo(400.000f, 215.000f, 1000.000f);
    endTextureMapper.setCamera(400.000f, 215.000f);
    endTextureMapper.updateAngles(0.000f, 0.000f, 0.000f);
    endTextureMapper.setRenderingAlgorithm(touchgfx::TextureMapper::BILINEAR_INTERPOLATION);
    endTextureMapper.setAlpha(255);

    startTextureMapper.setXY(0, 0);
    startTextureMapper.setBitmap(touchgfx::Bitmap(start));
    startTextureMapper.setWidth(800);
    startTextureMapper.setHeight(430);
    startTextureMapper.setBitmapPosition(0.000f, 0.000f);
    startTextureMapper.setScale(1.000f);
    startTextureMapper.setCameraDistance(1000.000f);
    startTextureMapper.setOrigo(400.000f, 215.000f, 1000.000f);
    startTextureMapper.setCamera(400.000f, 215.000f);
    startTextureMapper.updateAngles(0.000f, 0.000f, 0.000f);
    startTextureMapper.setRenderingAlgorithm(touchgfx::TextureMapper::BILINEAR_INTERPOLATION);
    startTextureMapper.setAlpha(255);

    add(endTextureMapper);
    add(startTextureMapper);

    startTextureMapper.invalidate();
    endTextureMapper.invalidate();
}

void SpinTransition::animationTick(uint16_t step, uint16_t duration)
{
    if (endTextureMapper.isVisible() == false)
    {
        endTextureMapper.setVisible(true);
    }
    float newAngle = FloatEasingEquations::floatCubicEaseInOut((float)step, 0, 2 * PI, (float)duration);
    startTextureMapper.updateZAngle(newAngle);
    endTextureMapper.updateZAngle(newAngle);

    uint16_t newAlpha = EasingEquations::cubicEaseInOut(step, 255, -255, duration);
    startTextureMapper.setAlpha((uint8_t)newAlpha);

    float newScale;
    if (step > duration / 2)
    {
        newScale = FloatEasingEquations::floatCubicEaseOut(step - duration / 2.f, 3.f, -2.f, duration / 2.f);
    }
    else
    {
        newScale = FloatEasingEquations::floatCubicEaseIn((float)step, 1.f, 2.f, duration / 2.f);
    }
    startTextureMapper.setScale(newScale);
    endTextureMapper.setScale(newScale);

    startTextureMapper.invalidate();
    endTextureMapper.invalidate();
}

uint16_t SpinTransition::endTransition()
{
    remove(endTextureMapper);
    remove(startTextureMapper);

    return endTextureMapper.getBitmap().getId();
}
