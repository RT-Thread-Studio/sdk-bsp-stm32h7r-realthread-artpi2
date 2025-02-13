#ifndef CHIMNEYCUSTOMCONTAINER_HPP
#define CHIMNEYCUSTOMCONTAINER_HPP

#include <gui_generated/containers/ChimneyCustomContainerBase.hpp>

class ChimneyCustomContainer : public ChimneyCustomContainerBase
{
public:
    ChimneyCustomContainer();
    virtual ~ChimneyCustomContainer() {}

    virtual void startAnimation(const bool nightMode);
    virtual void tick();

    virtual void initialize();

protected:
    const uint16_t ANIMATION_DURATION = 15; // in ticks
    const uint16_t ANIMATION_PEAK_EXPANSION = 7; // in pixels

    bool animationStarted;
    int16_t tickCounter;
    int16_t animationStartY;
    int16_t animationStartHeight;
};

#endif // CHIMNEYCUSTOMCONTAINER_HPP
