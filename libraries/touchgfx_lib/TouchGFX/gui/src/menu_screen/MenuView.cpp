#include <gui/menu_screen/MenuView.hpp>
#include <touchgfx/Utils.hpp>
#include <gui/common/Utils.hpp>
#include <gui/common/Constants.hpp>
#include <texts/TextKeysAndLanguages.hpp>
#include <images/BitmapDatabase.hpp>

MenuView::MenuView()
{

}

void MenuView::setupScreen()
{
    HAL::getInstance()->setFrameRateCompensation(true);

    MenuViewBase::setupScreen();
    useSMOCDrawing(false);
}

void MenuView::tearDownScreen()
{
    MenuViewBase::tearDownScreen();
}

void MenuView::startWheelAnimation()
{
    demosIconsWheel.startWheelAnimation();
}

void MenuView::changePreviewWheel()
{
    // convert the angles to be zero in the left side and in the range of -PI : +PI
    for (int i = 0; i < MainWheel::NUMBER_OF_WHEEL_ELEMENTS; i++)
    {
        wheelAngles[i] = demosIconsWheel.getElementAngle(i);
    }

    // Find the two elements closest to the center
    uint8_t indexImage1;
    uint8_t indexImage2;
    if (abs(wheelAngles[0]) < abs(wheelAngles[1]))
    {
        indexImage1 = 0;
        indexImage2 = 1;
    }
    else
    {
        indexImage1 = 1;
        indexImage2 = 0;
    }
    for (int i = 2; i < MainWheel::NUMBER_OF_WHEEL_ELEMENTS; i++)
    {
        if (abs(wheelAngles[i]) < abs(wheelAngles[indexImage1]))
        {
            indexImage2 = indexImage1;
            indexImage1 = i;
        }
        else if (abs(wheelAngles[i]) < abs(wheelAngles[indexImage2]))
        {
            indexImage2 = i;
        }
    }

    // Update the prieview images
    const float angletoPositionFactor = getScreenHeight() * MainWheel::NUMBER_OF_WHEEL_ELEMENTS / (2 * PI);
    const int16_t preview1Position = (int16_t)(angletoPositionFactor * wheelAngles[indexImage1]);
    const int16_t preview2Position = preview1Position + topPreviewImage.getHeight() * (wheelAngles[indexImage2] < wheelAngles[indexImage1] ? -1 : 1);
    topPreviewImage.moveTo(0, preview1Position);
    bottomPreviewImage.moveTo(0, preview2Position);
    topPreviewImage.setBitmap(convertTextIdToBitmapId(demosIconsWheel.getElementTextId(indexImage1)));
    bottomPreviewImage.setBitmap(convertTextIdToBitmapId(demosIconsWheel.getElementTextId(indexImage2)));
}

uint16_t MenuView::convertTextIdToBitmapId(const uint32_t textId)
{
    switch (textId)
    {
    case T_MAINSELECTEDSUBDEMOSVG:
        return BITMAP_PREVIEW_SVG_ID;
    case T_MAINSELECTEDSUBDEMODICE:
        return BITMAP_PREVIEW_DICE_ID;
    // case T_MAINSELECTEDSUBDEMOMJPEG:
    //     return BITMAP_PREVIEW_MJPEG_ID;
    case T_MAINSELECTEDSUBDEMOCOMPASS:
        return BITMAP_PREVIEW_COMPASS_ID;
    case T_MAINSELECTEDSUBDEMOTRANSITION:
        return BITMAP_PREVIEW_TRANSITIONS_ID;
    case T_MAINSELECTEDSUBDEMOEBIKE:
        return BITMAP_PREVIEW_EBIKE_ID;
    default:
        return 0;
    }
}

void MenuView::changeScreenTo(uint8_t selectedDemoIndex)
{
    switch (selectedDemoIndex)
    {
    case 0:
        goToSVG();
        break;
    case 1:
        goToDice();
        break;
    // case 2:
    //     goToMJPEG();
    //     break;
    case 2:
        goToCompass();
        break;
    case 3:
        goToTransition();
        break;
    case 4:
        goToEBike();
        break;
    }
}

void MenuView::handleTickEvent()
{
    mcuLoadPercentageContainer.checkMCULoad();
    frameRateContainer.countTheFrames();
}
