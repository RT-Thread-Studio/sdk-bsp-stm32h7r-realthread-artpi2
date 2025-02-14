#include <gui/common/MCULoadPercentage.hpp>

MCULoadPercentage::MCULoadPercentage():
    previousMcuLoadPercentage(0),
    currentMCULoadPercentage(0)
{

}

void MCULoadPercentage::checkMCULoad()
{
    currentMCULoadPercentage = touchgfx::HAL::getInstance()->getMCULoadPct();

    // Only update the displayed number when it is different than the previous one
    if (currentMCULoadPercentage != previousMcuLoadPercentage)
    {
        updateShownMCULoadPercentage();
        previousMcuLoadPercentage = currentMCULoadPercentage;
    }
}
