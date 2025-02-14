#include <gui/containers/MCULoadPercentage_Compass.hpp>

MCULoadPercentage_Compass::MCULoadPercentage_Compass()
{

}

void MCULoadPercentage_Compass::initialize()
{
    MCULoadPercentage_CompassBase::initialize();
}

void MCULoadPercentage_Compass::updateShownMCULoadPercentage()
{
    Unicode::snprintf(mcuLoadPercentageBuffer, MCULOADPERCENTAGE_SIZE, "%d", currentMCULoadPercentage);
    mcuLoadPercentage.invalidate();
}
