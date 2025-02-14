#include <gui/svg_screen/SVGView.hpp>
#include <touchgfx/Utils.hpp>
#include <gui/common/Utils.hpp>
#include <images/SVGDatabase.hpp>

SVGView::SVGView(): MAIN_WAFFLE_Y(15.0), tickCounter(0)
{

}

void SVGView::setupScreen()
{
    SVGViewBase::setupScreen();

    waffleMenu.setupTheMenu();
}

void SVGView::tearDownScreen()
{
    SVGViewBase::tearDownScreen();
}

void SVGView::changeTheMainWaffle()
{
    findOutTheChosenWaffleSize();

    moveTheWaffleBackFromButtom();
}

void SVGView::findOutTheChosenWaffleSize()
{
    WaffleMenuContainer::SizeOfWaffle sizeOfTheChosenWaffle = waffleMenu.getSizeOfTheChosenWaffle();

    switch (sizeOfTheChosenWaffle)
    {
    case WaffleMenuContainer::BIG:
        changeWaffleToBig();
        break;
    case WaffleMenuContainer::MEDIUM:
        changeWaffleToMedium();
        break;
    case WaffleMenuContainer::SMALL:
        changeWaffleToSmall();
        break;
    default:
        break;
    }
}

void SVGView::changeWaffleToBig()
{
    MAIN_WAFFLE_Y = 15.0;

    mainWaffle.setSVG(SVG_WAFFLE_LARGE_ID);
    mainWaffle.setImagePosition(MAIN_WAFFLE_X, MAIN_WAFFLE_Y);
    mainWaffle.invalidate();
}

void SVGView::changeWaffleToMedium()
{
    MAIN_WAFFLE_Y = 19.0;

    mainWaffle.setSVG(SVG_WAFFLE_MEDIUM_ID);
    mainWaffle.setImagePosition(MAIN_WAFFLE_X, MAIN_WAFFLE_Y);
    mainWaffle.invalidate();

}

void SVGView::changeWaffleToSmall()
{
    MAIN_WAFFLE_Y = 20.0;

    mainWaffle.setSVG(SVG_WAFFLE_SMALL_ID);
    mainWaffle.setImagePosition(MAIN_WAFFLE_X, MAIN_WAFFLE_Y);
    mainWaffle.invalidate();

}

void SVGView::moveTheWaffleBackFromButtom()
{
    //The waffle is set to move out of the screen through the designer
    //when new waffle is selected

    mainWaffle.clearMoveAnimationEndedAction();
    mainWaffle.startMoveAnimation(290, 230, 50, touchgfx::EasingEquations::bounceEaseOut, touchgfx::EasingEquations::bounceEaseOut);
}

void SVGView::handleTickEvent()
{
    mcuLoadPercentageContainer.checkMCULoad();
    frameRateContainer.countTheFrames();
    iceWheelContainer.handleTickEvent();

    leftButton.setTouchable(!iceWheelContainer.isAnimating());
    rightButton.setTouchable(!iceWheelContainer.isAnimating());

    tickCounter++;
    attractUserAttentionToWaffleMenu();
}

void SVGView::attractUserAttentionToWaffleMenu()
{
    if (tickCounter == 200)
    {
        waffleMenu.shakeTheBigWaffleIcon();
        resetTickCounter();
    }
}

void SVGView::resetTickCounter()
{
    //This function is also called from the Interactions
    tickCounter = 0;
}

void SVGView::moveIceScoopsToLeft()
{
    iceWheelContainer.moveContainerElementsToLeft();
}

void SVGView::moveIceScoopsToRight()
{
    iceWheelContainer.moveContainerElementsToRight();
}
