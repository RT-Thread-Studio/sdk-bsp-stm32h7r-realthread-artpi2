#include <gui/containers/ScreenTranstionsTopbarContainer.hpp>
#include "gui/common/constans.hpp"

ScreenTranstionsTopbarContainer::ScreenTranstionsTopbarContainer()
{

}

void ScreenTranstionsTopbarContainer::initialize()
{
    ScreenTranstionsTopbarContainerBase::initialize();
}

void ScreenTranstionsTopbarContainer::updateMCU(uint16_t value)
{
    Unicode::snprintf(mcuValueBuffer, MCUVALUE_SIZE, "%d", value);
    mcuValue.invalidate();
}

void ScreenTranstionsTopbarContainer::updateFPS(uint16_t value)
{
    Unicode::snprintf(fpsValueBuffer, FPSVALUE_SIZE, "%d", value);
    fpsValue.invalidate();
}
