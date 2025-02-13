#include "gui/Transitions/RolloutTransition.hpp"
#include "gui/common/FloatEasingEquations.hpp"
#include <math.h>

RolloutTransition::RolloutTransition()
{
}

RolloutTransition::~RolloutTransition()
{
}

void RolloutTransition::setupTranstion(Bitmap start, Bitmap end)
{
    startImage.setBitmap(Bitmap(start));
    startImage.setXY(0, 0);
    add(startImage);

    for (int i = 0; i < NUMBER_OF_TEXTURE_MAPPERS; i++)
    {
        cutoutTextureMappers[i].setXY(i * 100, 0);
        cutoutTextureMappers[i].setBitmap(Bitmap(end));
        cutoutTextureMappers[i].setWidth(100);
        cutoutTextureMappers[i].setHeight(430);
        cutoutTextureMappers[i].setBitmapPosition(-i * 100.f, 0.000f);
        cutoutTextureMappers[i].setScale(1.000f);
        cutoutTextureMappers[i].setCameraDistance(1000.000f);
        cutoutTextureMappers[i].setOrigo(0.000f, 0.000f, 1000.000f);
        cutoutTextureMappers[i].setCamera(50.000f, 215.000f);
        cutoutTextureMappers[i].updateAngles(0.000f, 0.000f, 0.000f);
        cutoutTextureMappers[i].setRenderingAlgorithm(touchgfx::TextureMapper::NEAREST_NEIGHBOR);
        cutoutTextureMappers[i].setVisible(false);

        add(cutoutTextureMappers[i]);
    }
}

void RolloutTransition::animationTick(uint16_t step, uint16_t duration)
{
    for (int i = 0; i < NUMBER_OF_TEXTURE_MAPPERS; i++)
    {
        if ((i + 1) * (duration / NUMBER_OF_TEXTURE_MAPPERS) > step)
        {
            if (cutoutTextureMappers[i].isVisible() == false)
            {
                cutoutTextureMappers[i].setVisible(true);
            }

            float newAngle = FloatEasingEquations::floatCubicEaseOut(fmodf(step, (float)(duration / NUMBER_OF_TEXTURE_MAPPERS)), 0, PI / 2, (float)(duration / NUMBER_OF_TEXTURE_MAPPERS));
            cutoutTextureMappers[i].updateYAngle(PI / 2 - newAngle);
            cutoutTextureMappers[i].invalidate();
            break;
        }
    }
}

uint16_t RolloutTransition::endTransition()
{
    remove(startImage);

    for (int i = 0; i < NUMBER_OF_TEXTURE_MAPPERS; i++)
    {
        Bitmap::dynamicBitmapDelete(cutoutTextureMappers[i].getBitmap().getId());
        remove(cutoutTextureMappers[i]);
    }
    return cutoutTextureMappers[0].getBitmap();
}
