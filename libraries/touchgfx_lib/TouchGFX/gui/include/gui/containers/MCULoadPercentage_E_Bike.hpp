#ifndef MCULOADPERCENTAGE_E_BIKE_HPP
#define MCULOADPERCENTAGE_E_BIKE_HPP

#include <gui_generated/containers/MCULoadPercentage_E_BikeBase.hpp>
#include <gui/common/MCULoadPercentage.hpp>

class MCULoadPercentage_E_Bike : public MCULoadPercentage_E_BikeBase, public MCULoadPercentage
{
public:
    MCULoadPercentage_E_Bike();
    virtual ~MCULoadPercentage_E_Bike() {}

    virtual void initialize();
protected:
    virtual void updateShownMCULoadPercentage();
};

#endif // MCULOADPERCENTAGE_E_BIKE_HPP
