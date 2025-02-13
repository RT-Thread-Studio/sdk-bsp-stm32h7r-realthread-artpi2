#include <gui/containers/TopBarContainer.hpp>

TopBarContainer::TopBarContainer()
{

}

void TopBarContainer::initialize()
{
    TopBarContainerBase::initialize();
}

void TopBarContainer::updateFPS(int16_t fps)
{
    Unicode::snprintf(fpsValueBuffer, FPSVALUE_SIZE, "%d", fps);
    fpsValue.invalidate();
}

void TopBarContainer::updateMCU(uint16_t mcuLoad)
{
    Unicode::snprintf(mcuValueBuffer, MCUVALUE_SIZE, "%d", mcuLoad);
    mcuValue.invalidate();
}
