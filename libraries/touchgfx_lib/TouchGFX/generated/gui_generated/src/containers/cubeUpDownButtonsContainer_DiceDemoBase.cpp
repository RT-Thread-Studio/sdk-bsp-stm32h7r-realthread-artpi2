/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#include <gui_generated/containers/cubeUpDownButtonsContainer_DiceDemoBase.hpp>
#include <images/BitmapDatabase.hpp>

cubeUpDownButtonsContainer_DiceDemoBase::cubeUpDownButtonsContainer_DiceDemoBase() :
    upPressedCallback(0),
    downPressedCallback(0),
    fadeAnimationEnddedCallback(0),
    flexButtonCallback(this, &cubeUpDownButtonsContainer_DiceDemoBase::flexButtonCallbackHandler)
{
    setWidth(156);
    setHeight(430);
    background.setXY(38, 20);
    background.setBitmap(touchgfx::Bitmap(BITMAP_DICE_TOP_BOTTOM_BAR_ID));
    background.setAlpha(0);
    add(background);

    buttonDown.setIconBitmaps(Bitmap(BITMAP_ICON_DICE_DOWN_NORMAL_ID), Bitmap(BITMAP_ICON_DICE_DOWN_PRESSED_ID));
    buttonDown.setIconXY(60, 35);
    buttonDown.setAlpha(0);
    buttonDown.setAction(flexButtonCallback);
    buttonDown.setPosition(0, 329, 156, 101);
    add(buttonDown);

    buttonUp.setIconBitmaps(Bitmap(BITMAP_ICON_DICE_UP_NORMAL_ID), Bitmap(BITMAP_ICON_DICE_UP_PRESSED_ID));
    buttonUp.setIconXY(60, 32);
    buttonUp.setAlpha(0);
    buttonUp.setAction(flexButtonCallback);
    buttonUp.setPosition(0, 0, 156, 101);
    add(buttonUp);
}

cubeUpDownButtonsContainer_DiceDemoBase::~cubeUpDownButtonsContainer_DiceDemoBase()
{

}

void cubeUpDownButtonsContainer_DiceDemoBase::initialize()
{

}

void cubeUpDownButtonsContainer_DiceDemoBase::flexButtonCallbackHandler(const touchgfx::AbstractButtonContainer& src)
{
    if (&src == &buttonUp)
    {
        //UpPressed
        //When buttonUp clicked emit upPressed callback
        //Emit callback
        emitUpPressedCallback();
    }
    if (&src == &buttonDown)
    {
        //DownPressed
        //When buttonDown clicked emit downPressed callback
        //Emit callback
        emitDownPressedCallback();
    }
}
