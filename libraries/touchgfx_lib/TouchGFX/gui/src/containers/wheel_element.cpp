#include <gui/containers/wheel_element.hpp>
#include <images/BitmapDatabase.hpp>

wheel_element::wheel_element()
{

}

void wheel_element::initialize()
{
    wheel_elementBase::initialize();
}

void wheel_element::setSphereImage(BitmapId bmpId)
{
    logoTextureMapper.setBitmap(Bitmap(bmpId));
}

void wheel_element::setSphereScale(float scale)
{
    logoTextureMapper.setScale(scale);
    ballTextureMapper.setScale(scale);
    reflectionTextureMapper.setScale(scale);
    shadowTextureMapper.setScale(scale);
}

void wheel_element::setAlpha(float deltaY)
{
    shadowTextureMapper.setAlpha(95 + (int)deltaY * 4 > 0 ? 95 + (int)deltaY * 4 : 0);

    int newAlpha = (int)deltaY * 7;
    if (newAlpha > 255)
    {
        newAlpha = 255;
    }
    else if (newAlpha < 0)
    {
        newAlpha = 0;
    }

    bool visible = newAlpha == 255 ? false : true;

    visible = newAlpha == 0 ? false : true;

    reflectionTextureMapper.setVisible(visible);
    reflectionTextureMapper.setAlpha(newAlpha);

    visible = deltaY > -30 ? true : false;
    setVisible(visible);
}

void wheel_element::startAnimation()
{
    logoAnimatedImage.setVisible(true);
    logoTextureMapper.setVisible(false);

    if (logoTextureMapper.getBitmapId() == BITMAP_ST_FACTORY_00000_ID)
    {
        logoAnimatedImage.setBitmap(BITMAP_ST_FACTORY_00000_ID);
        logoAnimatedImage.setBitmapEnd(BITMAP_ST_FACTORY_00015_ID);
        logoAnimatedImage.setUpdateTicksInterval(4);
        logoAnimatedImage.startAnimation(false, true, true);
    }

    else if (logoTextureMapper.getBitmapId() == BITMAP_TRANSTION_00000_ID)
    {
        logoAnimatedImage.setBitmap(BITMAP_TRANSTION_00000_ID);
        logoAnimatedImage.setBitmapEnd(BITMAP_TRANSTION_00015_ID);
        logoAnimatedImage.setUpdateTicksInterval(4);
        logoAnimatedImage.startAnimation(false, true, true);
    }

    else if (logoTextureMapper.getBitmapId() == BITMAP_COMPASS_00000_ID)
    {
        logoAnimatedImage.setBitmap(BITMAP_COMPASS_00000_ID);
        logoAnimatedImage.setBitmapEnd(BITMAP_COMPASS_00025_ID);
        logoAnimatedImage.setUpdateTicksInterval(4);
        logoAnimatedImage.startAnimation(false, true, true);
    }

    else if (logoTextureMapper.getBitmapId() == BITMAP_SPEEDOMETER_00000_ID)
    {
        logoAnimatedImage.setBitmap(BITMAP_SPEEDOMETER_00000_ID);
        logoAnimatedImage.setBitmapEnd(BITMAP_SPEEDOMETER_00025_ID);
        logoAnimatedImage.setUpdateTicksInterval(4);
        logoAnimatedImage.startAnimation(false, true, true);
    }

    else if (logoTextureMapper.getBitmapId() == BITMAP_INFO_00000_ID)
    {
        logoAnimatedImage.setBitmap(BITMAP_INFO_00000_ID);
        logoAnimatedImage.setBitmapEnd(BITMAP_INFO_00015_ID);
        logoAnimatedImage.setUpdateTicksInterval(4);
        logoAnimatedImage.startAnimation(false, true, true);
    }

    else if (logoTextureMapper.getBitmapId() == BITMAP_SVG_00000_ID)
    {
        logoAnimatedImage.setBitmap(BITMAP_SVG_00000_ID);
        logoAnimatedImage.setBitmapEnd(BITMAP_SVG_00018_ID);
        logoAnimatedImage.setUpdateTicksInterval(4);
        logoAnimatedImage.startAnimation(false, true, true);
    }

    else
    {
        stopAnimation();
    }
}

void wheel_element::stopAnimation()
{
    if (logoAnimatedImage.isAnimatedImageRunning())
    {
        logoAnimatedImage.stopAnimation();
    }
    logoAnimatedImage.setVisible(false);
    logoTextureMapper.setVisible(true);
}

void wheel_element::invalidateCustom()
{
    ballTextureMapper.invalidateContent();
    shadowTextureMapper.invalidateContent();
    reflectionTextureMapper.invalidateContent();
}
