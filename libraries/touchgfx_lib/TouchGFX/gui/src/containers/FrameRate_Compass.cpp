#include <gui/containers/FrameRate_Compass.hpp>

FrameRate_Compass::FrameRate_Compass()
{

}

void FrameRate_Compass::initialize()
{
    FrameRate_CompassBase::initialize();
}

void FrameRate_Compass::updateShownFPS()
{
    Unicode::snprintf(fpsValueBuffer, FPSVALUE_SIZE, "%d", fps);
    fpsValue.invalidate();
}
