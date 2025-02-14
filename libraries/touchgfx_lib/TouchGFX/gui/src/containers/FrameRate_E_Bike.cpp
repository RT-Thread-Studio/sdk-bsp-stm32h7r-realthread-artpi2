#include <gui/containers/FrameRate_E_Bike.hpp>

FrameRate_E_Bike::FrameRate_E_Bike()
{

}

void FrameRate_E_Bike::initialize()
{
    FrameRate_E_BikeBase::initialize();
}

void FrameRate_E_Bike::updateShownFPS()
{
    Unicode::snprintf(fpsValueBuffer, FPSVALUE_SIZE, "%d", fps);
    fpsValue.invalidate();
}
