#include "gui/Transitions/FlipTransition.hpp"
#include "gui/common/FloatEasingEquations.hpp"
#include <images/BitmapDatabase.hpp>

FlipTransition::FlipTransition()
{
}

FlipTransition::~FlipTransition()
{
}

void FlipTransition::setupTranstion(Bitmap start, Bitmap end)
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

    add(endTextureMapper);
    add(startTextureMapper);

    startTextureMapper.invalidate();
    endTextureMapper.invalidate();
}

void FlipTransition::animationTick(uint16_t step, uint16_t duration)
{
    float newAngle = FloatEasingEquations::floatBackEaseOut(step, 0, PI, duration);

    startTextureMapper.updateYAngle(newAngle);
    endTextureMapper.updateYAngle(newAngle - PI);

    startTextureMapper.invalidate();
    endTextureMapper.invalidate();

    remove(endTextureMapper);
    remove(startTextureMapper);

    if (newAngle > (PI / 2))
    {

        add(startTextureMapper);
        add(endTextureMapper);

    }
    else
    {
        add(endTextureMapper);
        add(startTextureMapper);
    }
}

uint16_t FlipTransition::endTransition()
{
    remove(endTextureMapper);
    remove(startTextureMapper);

    return endTextureMapper.getBitmap().getId();
}
