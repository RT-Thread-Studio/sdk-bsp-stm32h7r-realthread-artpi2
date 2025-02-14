#ifndef FRAMERATE_HPP
#define FRAMERATE_HPP

#include<touchgfx/hal/HAL.hpp>

class FrameRate
{
public:
    FrameRate();
    virtual ~FrameRate() {}

    void countTheFrames();
protected:

    void checkIf_FPS_RequiresUpdate();
    virtual void updateShownFPS() = 0;

    uint16_t frameSkippedCounter;
    uint16_t frames;
    uint16_t fps;
};

#endif // FRAMERATE_HPP
