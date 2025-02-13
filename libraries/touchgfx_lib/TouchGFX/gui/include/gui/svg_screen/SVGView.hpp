#ifndef SVGVIEW_HPP
#define SVGVIEW_HPP

#include <gui_generated/svg_screen/SVGViewBase.hpp>
#include <gui/svg_screen/SVGPresenter.hpp>
#include <gui/containers/IceScoopItemContainer_SVG.hpp>

class SVGView : public SVGViewBase
{
public:
    SVGView();
    virtual ~SVGView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    virtual void handleTickEvent();
    virtual void resetTickCounter();

protected:
    virtual void changeTheMainWaffle();

private:
    const float MAIN_WAFFLE_X = 49.0;
    float MAIN_WAFFLE_Y;
    void findOutTheChosenWaffleSize();
    void changeWaffleToBig();
    void changeWaffleToMedium();
    void changeWaffleToSmall();

    void moveTheWaffleBackFromButtom();
    void iceScoopScrollWheelUpdateItem(IceScoopItemContainer_SVG& item, int16_t itemIndex);

    void moveIceScoopsToLeft();
    void moveIceScoopsToRight();

    int tickCounter;
    void attractUserAttentionToWaffleMenu();
};

#endif // SVGVIEW_HPP
