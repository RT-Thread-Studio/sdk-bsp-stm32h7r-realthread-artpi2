#include <gui/containers/MainWheelIcon.hpp>
#include <BitmapDatabase.hpp>

MainWheelIcon::MainWheelIcon()
{

}

void MainWheelIcon::initialize()
{
    MainWheelIconBase::initialize();
}

void MainWheelIcon::invalidateWheelContent()
{
    subDemoIcon.invalidateContent();
}

void MainWheelIcon::setIconScale(float size)
{
    subDemoIcon.setScale(size);
}

float MainWheelIcon::getIconScale()
{
    return subDemoIcon.getScale();
}

void MainWheelIcon::setIcon(uint16_t iconId)
{
    subDemoIcon.setBitmap(Bitmap(iconId));
    subDemoIcon.expand();
}

void MainWheelIcon::setAlpha(uint8_t alpha)
{
    subDemoIcon.setAlpha(alpha);
}

uint8_t MainWheelIcon::getAlpha()
{
    return subDemoIcon.getAlpha();
}

void MainWheelIcon::updateZAngle(float angle)
{
    subDemoIcon.updateZAngle(angle);
}
