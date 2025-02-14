#ifndef SELECTEDICONSCONTAINER_DICEDEMO_HPP
#define SELECTEDICONSCONTAINER_DICEDEMO_HPP

#include <gui_generated/containers/selectedIconsContainer_DiceDemoBase.hpp>

class selectedIconsContainer_DiceDemo : public selectedIconsContainer_DiceDemoBase
{
public:
    selectedIconsContainer_DiceDemo();
    virtual ~selectedIconsContainer_DiceDemo() {}

    virtual void initialize();

    void updateImage(bool is2d);
protected:

    static const uint16_t FADE_DURATION = 25;
};

#endif // SELECTEDICONSCONTAINER_DICEDEMO_HPP
