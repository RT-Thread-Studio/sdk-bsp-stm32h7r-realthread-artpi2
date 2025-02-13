#include <gui/containers/IceScoopItemContainer_SVG.hpp>
#include <images/SVGDatabase.hpp>
#include <touchgfx/Utils.hpp>

IceScoopItemContainer_SVG::IceScoopItemContainer_SVG()
{

}

void IceScoopItemContainer_SVG::initialize()
{
    IceScoopItemContainer_SVGBase::initialize();
}

void IceScoopItemContainer_SVG::setIceScoopSVG(int16_t itemIndex)
{
    switch (itemIndex)
    {
    case 0:
        iceScoopSVG.setSVG(SVG_SCOOP_BALL_4_ID);
        break;
    case 1:
        iceScoopSVG.setSVG(SVG_SOFT_ICE_1_ID);
        break;
    case 2:
        iceScoopSVG.setSVG(SVG_SOFT_ICE_4_ID);
        break;
    case 3:
        iceScoopSVG.setSVG(SVG_SCOOP_BALL_6_ID);
        break;
    case 4:
        iceScoopSVG.setSVG(SVG_SCOOP_BALL_2_ID);
        break;
    case 5:
        iceScoopSVG.setSVG(SVG_SOFT_ICE_2_ID);
        break;
    case 6:
        iceScoopSVG.setSVG(SVG_SCOOP_BALL_3_ID);
        break;
    case 7:
        iceScoopSVG.setSVG(SVG_SCOOP_BALL_5_ID);
        break;
    case 8:
        iceScoopSVG.setSVG(SVG_SOFT_ICE_3_ID);
        break;
    case 9:
        iceScoopSVG.setSVG(SVG_SCOOP_BALL_1_ID);
        break;
    }

    IceScoopItemContainer_SVG::setX(IceScoopItemContainer_SVGBase::getX());
    iceScoopSVG.invalidate();
}

void IceScoopItemContainer_SVG::setX(int16_t x)
{
    // set X as normal
    IceScoopItemContainer_SVGBase::setX(x);

    const int16_t deltaScaleCalculation = x - iceScoopCenteredXvalue; // we set the value to the correct origin 0 where the chip is meant to be centered on the screen
    const float newScale = iceScoopDefaultScale - (deltaScaleCalculation) * (deltaScaleCalculation) * scalingModifier; //The formula is scale = (x^2) * scalingModifier
    iceScoopSVG.setScale(newScale);

    float mySVGWidth = newScale * iceScoopSVGWidth;
    float ImageNewPositionX = (float)((iceScoopSVGWidth - mySVGWidth) / 2.0) + iceScoopOffsetX;
    float ImageNewPositionY = (float)((iceScoopSVGWidth - mySVGWidth) / 2.0) + iceScoopOffsetY;


    int myPosition = IceScoopItemContainer_SVGBase::getX() - iceScoopScrollWheelOffset;
    float factor = (float)(myPosition % iceScoopSVGWidth) / iceScoopSVGWidth;
    float angle = factor * 360;

    iceScoopSVG.setRotationCenter(IceScoopItemContainerRotationCenterX, IceScoopItemContainerRotationCenterY);
    iceScoopSVG.setImagePosition(ImageNewPositionX, ImageNewPositionY);
    iceScoopSVG.setRotation(angle);
    iceScoopSVG.invalidateContent();
}

float IceScoopItemContainer_SVG::getScaleSVG()
{
    return iceScoopSVG.getScaleX();
}

void IceScoopItemContainer_SVG::invalidateWheelContent()
{
    iceScoopSVG.invalidate();
}
