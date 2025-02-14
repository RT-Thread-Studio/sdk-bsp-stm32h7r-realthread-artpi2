#ifndef MCULOADPERCENTAGE_COMPASS_HPP
#define MCULOADPERCENTAGE_COMPASS_HPP

#include <gui_generated/containers/MCULoadPercentage_CompassBase.hpp>
#include <gui/common/MCULoadPercentage.hpp>

class MCULoadPercentage_Compass : public MCULoadPercentage_CompassBase, public MCULoadPercentage
{
public:
    MCULoadPercentage_Compass();
    virtual ~MCULoadPercentage_Compass() {}

    virtual void initialize();
protected:
    virtual void updateShownMCULoadPercentage();
};

#endif // MCULOADPERCENTAGE_COMPASS_HPP
