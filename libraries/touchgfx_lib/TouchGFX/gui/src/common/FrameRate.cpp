#include <gui/common/FrameRate.hpp>

FrameRate::FrameRate():
    frameSkippedCounter(0),
    frames(0),
    fps(0)
{

}

void FrameRate::countTheFrames()
{
    // This section uses the HAL layer to detect the LCD Refresh count and calculates the FPS if it is changed, the topBar is updated
    if (touchgfx::HAL::getInstance()->getLCDRefreshCount() > 1)
    {
        frameSkippedCounter++;
    }
    else
    {
        frames++;
    }

    checkIf_FPS_RequiresUpdate();
}

void FrameRate::checkIf_FPS_RequiresUpdate()
{
    if (frames + frameSkippedCounter >= 60)
    {
        if (fps != frames)
        {
            fps = frames;
            updateShownFPS();
        }
        frameSkippedCounter = 0;
        frames = 0;
    }
}
