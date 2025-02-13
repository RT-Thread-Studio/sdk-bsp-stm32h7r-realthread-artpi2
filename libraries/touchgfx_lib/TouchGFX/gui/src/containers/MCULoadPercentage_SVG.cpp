#include <gui/containers/MCULoadPercentage_SVG.hpp>

MCULoadPercentage_SVG::MCULoadPercentage_SVG()
{

}

void MCULoadPercentage_SVG::initialize()
{
    MCULoadPercentage_SVGBase::initialize();
}

void MCULoadPercentage_SVG::updateShownInformation()
{
    Unicode::snprintf(mcuValueBuffer, MCUVALUE_SIZE, "%d", currentMCULoadPercentage);
    mcuValue.invalidate();
}
