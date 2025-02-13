#include "gui/Transitions/CubeTransition.hpp"
#include <gui/common/FloatEasingEquations.hpp>

CubeTransition::CubeTransition()
{
}

CubeTransition::~CubeTransition()
{
}

void CubeTransition::setupTranstion(Bitmap start, Bitmap end)
{
    startTextureMapper.setXY(0, 0);
    startTextureMapper.setBitmap(touchgfx::Bitmap(start));
    startTextureMapper.setWidth(800);
    startTextureMapper.setHeight(430);
    startTextureMapper.setBitmapPosition(0.000f, 0.000f);
    startTextureMapper.setScale(1.000f);
    startTextureMapper.setCameraDistance(1000.000f);
    startTextureMapper.setOrigo(400.000f, 215.000f, 1399.000f);
    startTextureMapper.setCamera(400.000f, 215.000f);
    startTextureMapper.updateAngles(0.000f, 0.000f, 0.000f);
    startTextureMapper.setRenderingAlgorithm(touchgfx::TextureMapper::NEAREST_NEIGHBOR);

    endTextureMapper.setXY(0, 0);
    endTextureMapper.setBitmap(touchgfx::Bitmap(end));
    endTextureMapper.setWidth(800);
    endTextureMapper.setHeight(430);
    endTextureMapper.setBitmapPosition(0.000f, 0.000f);
    endTextureMapper.setScale(1.000f);
    endTextureMapper.setCameraDistance(1000.000f);
    endTextureMapper.setOrigo(400.000f, 215.000f, 1399.000f);
    endTextureMapper.setCamera(400.000f, 215.000f);
    endTextureMapper.updateAngles(0.000f, -(PI / 2), 0.000f);
    endTextureMapper.setRenderingAlgorithm(touchgfx::TextureMapper::NEAREST_NEIGHBOR);

    add(endTextureMapper);
    add(startTextureMapper);

    startTextureMapper.invalidate();
    endTextureMapper.invalidate();
}

void CubeTransition::animationTick(uint16_t step, uint16_t duration)
{
    startTextureMapper.invalidate();
    endTextureMapper.invalidate();

    float newAngle = FloatEasingEquations::floatCubicEaseInOut(step, 0, PI / 2, duration);
    startTextureMapper.updateYAngle(newAngle);
    endTextureMapper.updateYAngle(newAngle - PI / 2);

    float newScale;
    if (step > duration / 2)
    {
        newScale = FloatEasingEquations::floatCubicEaseOut(step - duration / 2.f, 0.6f, 0.4f, duration / 2.f);
    }
    else
    {
        newScale = FloatEasingEquations::floatCubicEaseIn(step, 1.f, -0.4f, duration / 2.f);
    }
    startTextureMapper.setScale(newScale);
    endTextureMapper.setScale(newScale);

    remove(startTextureMapper);
    remove(endTextureMapper);
    if (startTextureMapper.getYAngle() < PI / 4)
    {
        add(endTextureMapper);
        add(startTextureMapper);
    }
    else
    {
        add(startTextureMapper);
        add(endTextureMapper);
    }

    startTextureMapper.invalidate();
    endTextureMapper.invalidate();
}

uint16_t CubeTransition::endTransition()
{
    remove(startTextureMapper);
    remove(endTextureMapper);

    return endTextureMapper.getBitmap().getId();
}
