#ifndef SCROLLWHEELCOVERFLOW_HPP
#define SCROLLWHEELCOVERFLOW_HPP

#include <gui_generated/containers/scrollWheelCoverFlowBase.hpp>

class scrollWheelCoverFlow : public scrollWheelCoverFlowBase
{
public:
    scrollWheelCoverFlow();
    virtual ~scrollWheelCoverFlow() {}

    virtual void initialize();

    virtual void scrollWheelUpdateItem(elementCoverFlowCustomContainer& item, int16_t itemIndex)
    {
        item.setChipBitmap(itemIndex);
    }
protected:
};

#endif // SCROLLWHEELCOVERFLOW_HPP
