#ifndef TOPBARCONTAINER_HPP
#define TOPBARCONTAINER_HPP

#include <gui_generated/containers/topbarContainerBase.hpp>
#include <gui/common/FrameRate.hpp>
#include <gui/common/MCULoadPercentage.hpp>

class topbarContainer : public topbarContainerBase, public FrameRate, public MCULoadPercentage
{
public:
    topbarContainer();
    virtual ~topbarContainer() {}

    virtual void initialize();
protected:

    virtual void updateShownFPS();
    virtual void updateShownMCULoadPercentage();
};

#endif // TOPBARCONTAINER_HPP
