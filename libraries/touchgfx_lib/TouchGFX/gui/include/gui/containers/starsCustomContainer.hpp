#ifndef STARSCUSTOMCONTAINER_HPP
#define STARSCUSTOMCONTAINER_HPP

#include <gui_generated/containers/starsCustomContainerBase.hpp>

class starsCustomContainer : public starsCustomContainerBase
{
public:
    starsCustomContainer();
    virtual ~starsCustomContainer() {}

    virtual void initialize();

    void showStars();
    void handleTick();
    void hideStars();
protected:
    uint32_t tickCount;
};

#endif // STARSCUSTOMCONTAINER_HPP
