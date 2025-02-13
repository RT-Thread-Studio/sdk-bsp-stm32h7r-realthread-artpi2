#ifndef TRUCKCUSTOMCONTAINER_HPP
#define TRUCKCUSTOMCONTAINER_HPP

#include <gui_generated/containers/truckCustomContainerBase.hpp>


class TruckCustomContainer : public TruckCustomContainerBase
{
public:
    TruckCustomContainer();
    virtual ~TruckCustomContainer() {}

    virtual void initialize();
    virtual void setLights(const bool active);
    virtual void setNewColor(const bool nightMode);
    virtual void startAnimation();
    virtual void animationTick();
    virtual bool isAnimationFinished();

private:
    const uint16_t TRUCK_LIGHTS_FADE_DURATION = 40; // in ticks
    const uint16_t ANIMATION_DURATION = 10; // in ticks
    const uint16_t ANIMATION_PEAK_EXPANSION = 30; // in pixels

    bool animationStarted;
    bool lastDayColorWasBlue;
    int16_t tickCounter;
    int16_t animationStartX;
    int16_t animationStartWidth;
};

#endif // TRUCKCUSTOMCONTAINER_HPP
