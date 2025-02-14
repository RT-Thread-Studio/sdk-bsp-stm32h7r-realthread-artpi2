#include "gui/Transitions/SpinOutTransition.hpp"
#include <images/BitmapDatabase.hpp>
#include "gui/common/FloatEasingEquations.hpp"

SpinOutTransition::SpinOutTransition()
{
}

SpinOutTransition::~SpinOutTransition()
{
}

void SpinOutTransition::setupTranstion(Bitmap start, Bitmap end)
{
    startImage.setXY(0, 0);
    startImage.setBitmap(touchgfx::Bitmap(start));

    endTextureMapper.setXY(0, 0);
    endTextureMapper.setBitmap(touchgfx::Bitmap(end));
    endTextureMapper.setWidth(800);
    endTextureMapper.setHeight(430);
    endTextureMapper.setBitmapPosition(0.000f, 0.000f);
    endTextureMapper.setScale(0.000f);
    endTextureMapper.setCameraDistance(1000.000f);
    endTextureMapper.setOrigo(400.000f, 215.000f, 1000.000f);
    endTextureMapper.setCamera(400.000f, 215.000f);
    endTextureMapper.updateAngles(0.000f, 0.000f, PI);
    endTextureMapper.setRenderingAlgorithm(touchgfx::TextureMapper::BILINEAR_INTERPOLATION);

    add(startImage);
    add(endTextureMapper);

    startImage.invalidate();
    endTextureMapper.invalidate();
}

void SpinOutTransition::animationTick(uint16_t step, uint16_t duration)
{
    float newAngle = FloatEasingEquations::floatCubicEaseOut((float)step, PI, 3 * PI, duration);
    float newScale = FloatEasingEquations::floatCubicEaseOut((float)step, 0, 1, duration);

    endTextureMapper.updateZAngle(newAngle);
    endTextureMapper.setScale(newScale);

    endTextureMapper.invalidate();
}

uint16_t SpinOutTransition::endTransition()
{
    remove(endTextureMapper);
    remove(startImage);

    return endTextureMapper.getBitmap().getId();
}
