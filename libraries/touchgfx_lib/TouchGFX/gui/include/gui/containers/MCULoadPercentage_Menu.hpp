#ifndef MCULOADPERCENTAGE_MENU_HPP
#define MCULOADPERCENTAGE_MENU_HPP

#include <gui_generated/containers/MCULoadPercentage_MenuBase.hpp>
#include <gui/common/MCULoadPercentage.hpp>

class MCULoadPercentage_Menu : public MCULoadPercentage_MenuBase, public MCULoadPercentage
{
public:
    MCULoadPercentage_Menu();
    virtual ~MCULoadPercentage_Menu() {}

    virtual void initialize();
protected:

    virtual void updateShownMCULoadPercentage();
};

#endif // MCULOADPERCENTAGE_MENU_HPP
