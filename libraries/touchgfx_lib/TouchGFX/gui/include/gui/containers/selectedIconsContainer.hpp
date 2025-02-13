#ifndef SELECTEDICONSCONTAINER_HPP
#define SELECTEDICONSCONTAINER_HPP

#include <gui_generated/containers/selectedIconsContainerBase.hpp>

class selectedIconsContainer : public selectedIconsContainerBase
{
public:
    selectedIconsContainer();
    virtual ~selectedIconsContainer() {}

    virtual void initialize();

    void updateImage(bool is2d);
protected:

    static const uint16_t FADE_DURATION = 25;
};

#endif // SELECTEDICONSCONTAINER_HPP
