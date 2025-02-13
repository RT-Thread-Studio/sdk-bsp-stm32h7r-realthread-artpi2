#ifndef TOPBARCONTAINER_HPP
#define TOPBARCONTAINER_HPP

#include <gui_generated/containers/TopBarContainerBase.hpp>

class TopBarContainer : public TopBarContainerBase
{
public:
    TopBarContainer();
    virtual ~TopBarContainer() {}

    virtual void initialize();

    void updateFPS(int16_t fps);
    void updateMCU(uint16_t mcuLoad);
protected:

};

#endif // TOPBARCONTAINER_HPP
