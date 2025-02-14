#include <gui/containers/MCULoadPercentage_E_Bike.hpp>

MCULoadPercentage_E_Bike::MCULoadPercentage_E_Bike()
{

}

void MCULoadPercentage_E_Bike::initialize()
{
    MCULoadPercentage_E_BikeBase::initialize();
}

void MCULoadPercentage_E_Bike::updateShownMCULoadPercentage()
{
    Unicode::snprintf(mcuLoadPercentageBuffer, MCULOADPERCENTAGE_SIZE, "%d", currentMCULoadPercentage);
    mcuLoadPercentage.invalidate();
}
