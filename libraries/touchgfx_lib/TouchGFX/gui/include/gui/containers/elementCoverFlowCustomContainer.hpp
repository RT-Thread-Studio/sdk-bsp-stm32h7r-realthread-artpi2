#ifndef ELEMENTCOVERFLOWCUSTOMCONTAINER_HPP
#define ELEMENTCOVERFLOWCUSTOMCONTAINER_HPP

#include <gui_generated/containers/elementCoverFlowCustomContainerBase.hpp>

class elementCoverFlowCustomContainer : public elementCoverFlowCustomContainerBase
{
public:
    elementCoverFlowCustomContainer();
    virtual ~elementCoverFlowCustomContainer() {}

    virtual void initialize();
    void invalidateWheelContent();

    void getYangle();

    void setChipBitmap(int16_t id);

    //The new declaration and implementation of the setX() function. The scalling animation will be added here
    virtual void setX(int16_t X);

protected:
    static constexpr float PI = 3.14159265358979323846f;
    static constexpr float PI_HALF = PI / 2;
    static constexpr float PI_QUARTER = PI / 4;
    static constexpr float PI_THREE_QUARTER = PI / 4 * 3;
    static constexpr float PI_FIFTH = PI / 5;
    static constexpr float PI_SIXTH = PI / 6;
    static constexpr float PI_EIGHTH = PI / 8;
    static constexpr float PI_SCALE = 1 / PI;

    //Cover Flow Custom Container and Element
    static constexpr int NUMBER_OF_WHEEL_ELEMENTS = 7;
    static constexpr int NUMBER_OF_CHIPS = 7;
    static constexpr int chipBitmapWidth = 196;
    static constexpr int maximumPositonXFirstChip = chipBitmapWidth + 5;
    static constexpr int minimumPositonXLastChip = 200;
    static constexpr int originChipX = -chipBitmapWidth * 2; // center coordinates for the chip wheel
    static constexpr int originChipY = 75;
    static constexpr int distanceChips = 75; // distance between the chips in pixel
    static constexpr int scrollWheelItemMargin = 14;    // Update this value based on the "Item Margin" value in the ScrollWheel settings in the cover flow custom container
    static constexpr int chipCenteredXvalue = (630 / 2) - chipBitmapWidth / 2; // The x coordinate of a chip currently in the center of the screen is of 217px (size of container /2  -  width of chip / 2 = 630/2 - 196/2)
    static constexpr int distanceBetweenChips = chipBitmapWidth + scrollWheelItemMargin * 2; // The distance on the x axis between 2 chips is equal to the width of an element + 2*scroll wheel margin
    static constexpr float originChipScale = 1.0f; // the scale of a chip when in the center of the screen
    static constexpr float scalingModifier = 0.0000025f; // the scale of a chip when in the center of the screen
    static constexpr float ySmallRotationAnglePerStep = PI_FIFTH / distanceBetweenChips; // 1/5 pi rotation in a x movement equal to the distance between 2 chips
    static constexpr float yBigRightRotationAnglePerStep = PI_HALF / (distanceBetweenChips / 2.4f); // 1/2 pi rotation in a x movement equal to the distance between 2 chips divided by a modifier to make the rotation smoother
    static constexpr float yBigLeftRotationAnglePerStep = PI_HALF / (chipBitmapWidth / 4); // half pi rotation in a x movement equal to half of a chip's width

    // Enum used to know which bitmap to use for each element of the cover flow
    enum chipBitmap_enum
    {
        bitmap_H725_enum,  // 0
        bitmap_H735_enum,  // 1
        bitmap_H743_enum,  // 2
        bitmap_H750_enum,  // 3
        bitmap_H7A3_enum,  // 4
        bitmap_H7B3_enum,  // 5
        bitmap_H7S7_enum   // 6
    };

    float newYangle;
    float currentYangle;

    Bitmap saveBitmap;
};

#endif // ELEMENTCOVERFLOWCUSTOMCONTAINER_HPP
