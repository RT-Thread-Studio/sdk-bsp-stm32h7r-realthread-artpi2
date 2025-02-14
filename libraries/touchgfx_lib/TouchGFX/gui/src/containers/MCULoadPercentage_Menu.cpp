#include <gui/containers/MCULoadPercentage_Menu.hpp>

MCULoadPercentage_Menu::MCULoadPercentage_Menu()
{

}

void MCULoadPercentage_Menu::initialize()
{
    MCULoadPercentage_MenuBase::initialize();
}

void MCULoadPercentage_Menu::updateShownMCULoadPercentage()
{
    Unicode::snprintf(mcuValueBuffer, MCUVALUE_SIZE, "%d", currentMCULoadPercentage);
    mcuValue.invalidate();
}
