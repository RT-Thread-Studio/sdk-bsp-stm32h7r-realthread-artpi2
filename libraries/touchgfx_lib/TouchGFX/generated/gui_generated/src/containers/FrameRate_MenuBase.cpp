/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#include <gui_generated/containers/FrameRate_MenuBase.hpp>
#include <texts/TextKeysAndLanguages.hpp>
#include <touchgfx/Color.hpp>

FrameRate_MenuBase::FrameRate_MenuBase()
{
    setWidth(85);
    setHeight(30);
    fpsTite.setXY(0, 0);
    fpsTite.setColor(touchgfx::Color::getColorFromRGB(177, 170, 254));
    fpsTite.setLinespacing(0);
    fpsTite.setTypedText(touchgfx::TypedText(T___SINGLEUSE_PRW2));
    add(fpsTite);

    fpsValue.setPosition(42, 0, 43, 30);
    fpsValue.setColor(touchgfx::Color::getColorFromRGB(177, 170, 254));
    fpsValue.setLinespacing(0);
    Unicode::snprintf(fpsValueBuffer, FPSVALUE_SIZE, "%s", touchgfx::TypedText(T___SINGLEUSE_AXU9).getText());
    fpsValue.setWildcard(fpsValueBuffer);
    fpsValue.setTypedText(touchgfx::TypedText(T___SINGLEUSE_2U2X));
    add(fpsValue);
}

FrameRate_MenuBase::~FrameRate_MenuBase()
{

}

void FrameRate_MenuBase::initialize()
{

}
