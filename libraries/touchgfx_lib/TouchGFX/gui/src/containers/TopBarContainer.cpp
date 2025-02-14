#include <gui/containers/topbarContainer.hpp>
#include "gui/common/constans.hpp"
#include <touchgfx/Color.hpp>
#include <images/BitmapDatabase.hpp>

topbarContainer::topbarContainer()
{
}

void topbarContainer::initialize()
{
    topbarContainerBase::initialize();
}

void topbarContainer::updateShownFPS()
{
    Unicode::snprintf(fpsValueBuffer, FPSVALUE_SIZE, "%d", fps);
    fpsValue.invalidate();
}

void topbarContainer::updateShownMCULoadPercentage()
{
    Unicode::snprintf(mcuValueBuffer, MCUVALUE_SIZE, "%d", currentMCULoadPercentage);
    mcuValue.invalidate();
}
