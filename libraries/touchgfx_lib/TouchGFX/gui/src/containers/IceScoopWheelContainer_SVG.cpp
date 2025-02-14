#include <gui/containers/IceScoopWheelContainer_SVG.hpp>
#include <touchgfx/Utils.hpp>

IceScoopWheelContainer_SVG::IceScoopWheelContainer_SVG()
{

}

void IceScoopWheelContainer_SVG::initialize()
{
    IceScoopWheelContainer_SVGBase::initialize();
}

void IceScoopWheelContainer_SVG::iceScoopScrollWheelUpdateItem(IceScoopItemContainer_SVG& item, int16_t itemIndex)
{
    item.setIceScoopSVG(itemIndex);
}

void IceScoopWheelContainer_SVG::moveContainerElementsToLeft()
{
    iceScoopScrollWheel.animateToItem(iceScoopScrollWheel.getSelectedItem() - 1);
}

void IceScoopWheelContainer_SVG::moveContainerElementsToRight()
{
    iceScoopScrollWheel.animateToItem(iceScoopScrollWheel.getSelectedItem() + 1);
}

bool IceScoopWheelContainer_SVG::isAnimating()
{
    return iceScoopScrollWheel.isAnimating();
}
