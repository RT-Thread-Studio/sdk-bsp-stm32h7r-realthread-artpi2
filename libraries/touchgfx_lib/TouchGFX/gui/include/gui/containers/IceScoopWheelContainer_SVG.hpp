#ifndef IceScoopWheelContainer_SVG_HPP
#define IceScoopWheelContainer_SVG_HPP

#include <gui_generated/containers/IceScoopWheelContainer_SVGBase.hpp>

class IceScoopWheelContainer_SVG : public IceScoopWheelContainer_SVGBase
{
public:
    IceScoopWheelContainer_SVG();
    virtual ~IceScoopWheelContainer_SVG() {}

    virtual void initialize();

    void iceScoopScrollWheelUpdateItem(IceScoopItemContainer_SVG&, int16_t);

    virtual void moveContainerElementsToLeft();
    virtual void moveContainerElementsToRight();

    bool isAnimating();
};

#endif // IceScoopWheelContainer_SVG_HPP
