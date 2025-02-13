#ifndef IceScoopItemContainer_SVG_HPP
#define IceScoopItemContainer_SVG_HPP

#include <gui_generated/containers/IceScoopItemContainer_SVGBase.hpp>

class IceScoopItemContainer_SVG : public IceScoopItemContainer_SVGBase
{
public:
    IceScoopItemContainer_SVG();
    virtual ~IceScoopItemContainer_SVG() {}

    virtual void initialize();
    virtual float getScaleSVG();
    virtual void setIceScoopSVG(int16_t itemIndex);

    void invalidateWheelContent();

    //The new declaration and implementation of the setX() function. The scalling animation will be added here
    virtual void setX(int16_t x);


protected:

    static constexpr int NUMBER_OF_ICE_SCOOPS = 10;
    static constexpr int iceScoopSVGWidth = 200;
    static constexpr int iceScoopOffsetX = 20;
    static constexpr int iceScoopOffsetY = 10;
    static constexpr int iceScoopScrollWheelOffset = 100;
    static constexpr int iceScoopSrollWheelContainerWidth = 800;
    static constexpr float IceScoopItemContainerRotationCenterX = 100.0;
    static constexpr float IceScoopItemContainerRotationCenterY = 100.0;
    static constexpr int iceScoopCenteredXvalue = (iceScoopSrollWheelContainerWidth - iceScoopSVGWidth) / 2;
    static constexpr float iceScoopDefaultScale = 1.000000f;
    static constexpr float scalingModifier = 0.0000040f; // the scale of an iceScoop when in the center of the screen

};

#endif // IceScoopItemContainer_SVG_HPP
