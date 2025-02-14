#ifndef FRAMERATE_E_BIKE_HPP
#define FRAMERATE_E_BIKE_HPP

#include <gui_generated/containers/FrameRate_E_BikeBase.hpp>
#include<gui/common/FrameRate.hpp>

class FrameRate_E_Bike : public FrameRate_E_BikeBase, public FrameRate
{
public:
    FrameRate_E_Bike();
    virtual ~FrameRate_E_Bike() {}

    virtual void initialize();
protected:
    virtual void updateShownFPS();
};

#endif // FRAMERATE_E_BIKE_HPP
