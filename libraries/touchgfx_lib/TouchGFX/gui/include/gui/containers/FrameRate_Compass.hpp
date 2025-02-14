#ifndef FRAMERATE_COMPASS_HPP
#define FRAMERATE_COMPASS_HPP

#include <gui_generated/containers/FrameRate_CompassBase.hpp>
#include <gui/common/FrameRate.hpp>

class FrameRate_Compass : public FrameRate_CompassBase, public FrameRate
{
public:
    FrameRate_Compass();
    virtual ~FrameRate_Compass() {}

    virtual void initialize();
protected:
    virtual void updateShownFPS();
};

#endif // FRAMERATE_COMPASS_HPP
