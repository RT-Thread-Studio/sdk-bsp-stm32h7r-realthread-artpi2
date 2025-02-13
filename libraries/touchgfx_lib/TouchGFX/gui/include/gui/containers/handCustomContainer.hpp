#ifndef HANDCUSTOMCONTAINER_HPP
#define HANDCUSTOMCONTAINER_HPP

#include <gui_generated/containers/handCustomContainerBase.hpp>

class handCustomContainer : public handCustomContainerBase
{
public:
    handCustomContainer();
    virtual ~handCustomContainer() {}

    virtual void initialize();

    void activate();
    void tick();

protected:

    static const uint16_t FADE_DURATION = 20;
    static const uint16_t WAIT_DURATION = 30;
    static const uint16_t GROW_DURATION = 20;

    bool active;
    uint16_t count;
};

#endif // HANDCUSTOMCONTAINER_HPP
