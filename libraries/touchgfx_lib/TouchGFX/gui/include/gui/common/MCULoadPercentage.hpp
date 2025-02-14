#ifndef MCULOADPERCENTAGE_HPP
#define MCULOADPERCENTAGE_HPP

#include<touchgfx/hal/HAL.hpp>

class MCULoadPercentage
{
public:
    MCULoadPercentage();
    virtual ~MCULoadPercentage() {}

    void checkMCULoad();
protected:
    virtual void updateShownMCULoadPercentage() = 0;

    uint16_t previousMcuLoadPercentage;
    uint16_t currentMCULoadPercentage;
};

#endif // MCULOADPERCENTAGE_HPP
