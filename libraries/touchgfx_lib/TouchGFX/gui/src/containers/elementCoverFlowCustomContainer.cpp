#include <gui/containers/elementCoverFlowCustomContainer.hpp>
#include <BitmapDatabase.hpp>

elementCoverFlowCustomContainer::elementCoverFlowCustomContainer() :
    newYangle(0.0f),
    currentYangle(0.0f)
{

}

void elementCoverFlowCustomContainer::initialize()
{
    elementCoverFlowCustomContainerBase::initialize();
}

void elementCoverFlowCustomContainer::invalidateWheelContent()
{
    chipTextureMapper.invalidateContent();
}

void elementCoverFlowCustomContainer::getYangle()
{
    currentYangle = chipTextureMapper.getYAngle();
}

void elementCoverFlowCustomContainer::setChipBitmap(int16_t id)
{
    switch (id % NUMBER_OF_CHIPS)
    {
    case bitmap_H725_enum:
        chipTextureMapper.setBitmap(Bitmap(BITMAP_H725_SHADOW_ID));
        saveBitmap = Bitmap(BITMAP_H725_SHADOW_ID);
        break;
    case bitmap_H735_enum:
        chipTextureMapper.setBitmap(Bitmap(BITMAP_H735_SHADOW_ID));
        saveBitmap = Bitmap(BITMAP_H735_SHADOW_ID);
        break;
    case bitmap_H743_enum:
        chipTextureMapper.setBitmap(Bitmap(BITMAP_H743_SHADOW_ID));
        saveBitmap = Bitmap(BITMAP_H743_SHADOW_ID);
        break;
    case bitmap_H750_enum:
        chipTextureMapper.setBitmap(Bitmap(BITMAP_H750_SHADOW_ID));
        saveBitmap = Bitmap(BITMAP_H750_SHADOW_ID);
        break;
    case bitmap_H7A3_enum:
        chipTextureMapper.setBitmap(Bitmap(BITMAP_H7A3_SHADOW_ID));
        saveBitmap = Bitmap(BITMAP_H7A3_SHADOW_ID);
        break;
    case bitmap_H7B3_enum:
        chipTextureMapper.setBitmap(Bitmap(BITMAP_H7B3_SHADOW_ID));
        saveBitmap = Bitmap(BITMAP_H7B3_SHADOW_ID);
        break;
    case bitmap_H7S7_enum:
        chipTextureMapper.setBitmap(Bitmap(BITMAP_H7S7_SHADOW_ID));
        saveBitmap = Bitmap(BITMAP_H7S7_SHADOW_ID);
        break;
    }
}

//The new declaration and implementation of the setX() function used to modify the scaling value of the elements and the rotations
void elementCoverFlowCustomContainer::setX(int16_t x)
{
    // set X as normal
    elementCoverFlowCustomContainerBase::setX(x);

    // new Scale setting
    const int16_t deltaScaleCalculation = x - chipCenteredXvalue; // we set the value to the correct origin 0 where the chip is meant to be centered on the screen
    const float newScale = originChipScale - (deltaScaleCalculation) * (deltaScaleCalculation) * scalingModifier; // the forumal is scale = x^2*scalingModifier where the x=0 is not the real chipX=0 but chipX=chipCenteredXvalue
    chipTextureMapper.setScale(newScale);

    // rotational events
    if ((x >= (chipCenteredXvalue - distanceBetweenChips)) && (x <= chipCenteredXvalue))  // If the chip is within the 1st to the left chip position and the centered chip position
    {
        newYangle = (chipCenteredXvalue - x) * ySmallRotationAnglePerStep;
        chipTextureMapper.setBitmap(saveBitmap);
    }
    else if (x > chipCenteredXvalue) // If the chip is within the centered chip position and 1st to the right chip position
    {
        newYangle = (chipCenteredXvalue - x) * yBigRightRotationAnglePerStep;
        if (newYangle <= -PI_HALF)   // Change the bitmap of the chip  in certain angles to either show its true image or its back
        {
            chipTextureMapper.setBitmap(Bitmap(BITMAP_BACK_ID));
        }
        else
        {
            chipTextureMapper.setBitmap(saveBitmap);
        }

        if (newYangle <= -PI - PI_FIFTH)  // Force the 1/5th of a pi angle instead of a flat 0pi angle to have a nicer feel and look
        {
            newYangle = - PI - PI_FIFTH;
        }

    }
    else if (x < (chipCenteredXvalue - distanceBetweenChips))   // If the chip is left of the 1st to the left chip position
    {
        newYangle = PI_FIFTH + ((chipCenteredXvalue - distanceBetweenChips) - x) * yBigLeftRotationAnglePerStep;
        if (newYangle >= PI_HALF)
        {
            chipTextureMapper.setBitmap(Bitmap(BITMAP_BACK_ID));
        }
        else
        {
            chipTextureMapper.setBitmap(saveBitmap);
        }

        if (newYangle >= PI + PI_FIFTH)  // Force the 1/5th of a pi angle instead of a flat 0pi angle to have a nicer feel and look
        {
            newYangle = PI + PI_FIFTH;
        }
    }

    chipTextureMapper.setYAngle(newYangle);
    chipTextureMapper.invalidateContent();
}
