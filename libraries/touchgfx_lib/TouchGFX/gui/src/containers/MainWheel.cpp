#include <gui/containers/MainWheel.hpp>
#include <gui/common/Constants.hpp>
#include <gui/common/FloatEasingEquations.hpp>

#include <math.h>

#include <texts/TextKeysAndLanguages.hpp>
#include <images/BitmapDatabase.hpp>
#include <touchgfx/Utils.hpp>

static const float radius = 180; // Radius for the circle the menu items move in
static const float angle_between_elements = 2 * PI / MainWheel::NUMBER_OF_SUB_DEMOS;

MainWheel::MainWheel() : rotating(false), dragEventHappend(false), animateCircleBounce(false), animateCircleOut(false), activeIconsSelected(false)
{

}

void MainWheel::initialize()
{
    MainWheelBase::initialize();

    setTouchable(false);

    centerIcon = 1; // The index value of the icon that will be in the center/selcted position

    //Setup of the menu wheel placing the icons with a radian of a quarter pi apart
    for (int i = 0; i < NUMBER_OF_WHEEL_ELEMENTS; i++)
    {
        switch (i % NUMBER_OF_SUB_DEMOS)
        {
        case 0:
            wheelElements[i].textId = T_MAINSELECTEDSUBDEMOSVG;
            wheelElements[i].icon.setIcon(BITMAP_SVG_ICON_NORMAL_ID);
            break;
        case 1:
            wheelElements[i].textId = T_MAINSELECTEDSUBDEMODICE;
            wheelElements[i].icon.setIcon(BITMAP_DICE_ICON_NORMAL_ID);
            break;
        // case 2:
        //     wheelElements[i].textId = T_MAINSELECTEDSUBDEMOMJPEG;
        //     wheelElements[i].icon.setIcon(BITMAP_MJPEG_ICON_NORMAL_ID);
        //     break;
        case 2:
            wheelElements[i].textId = T_MAINSELECTEDSUBDEMOCOMPASS;
            wheelElements[i].icon.setIcon(BITMAP_COMPASS_ICON_NORMAL_ID);
            break;
        case 3:
            wheelElements[i].textId = T_MAINSELECTEDSUBDEMOTRANSITION;
            wheelElements[i].icon.setIcon(BITMAP_TRANSITIONS_ICON_NORMAL_ID);
            break;
        case 4:
            wheelElements[i].textId = T_MAINSELECTEDSUBDEMOEBIKE;
            wheelElements[i].icon.setIcon(BITMAP_EBIKE_ICON_NORMAL_ID);
            break;
        }

        wheelElements[i].icon.setVisible(false);
        wheelElements[i].angle = normalizeAngle(angle_between_elements * i);
        updateDrawingOfIcon(wheelElements[i]);

        insert(&selectedDemoName, wheelElements[i].icon);
    }
}

void MainWheel::handleClickEvent(const ClickEvent& evt)
{
    //Ignores click while intro is animating
    if (wheelElements[1].icon.isVisible() == false || animateCircleBounce || animateCircleOut)
    {
        return;
    }
    // Calculates the angle of the press (touch) acording to the wheel, used later to dertimine the wheels movement
    if (evt.getType() == ClickEvent::PRESSED)
    {
        dragEventHappend = false;
        rotating = false;

        selectedDemoName.startFadeAnimation(0, 10);

        lastAngle = calculateAngleFromCoordinate(evt.getX(), evt.getY());
    }
    //IF the wheel is not rotating and a drag have happen the wheel will move to have the icon that is closest to the "center" as center icon
    else if (rotating == false)
    {
        switchActiveIcons(false);
        if (dragEventHappend || rotatingStepCounter < rotatingSteps)
        {
            int closetsIcon = 1;
            float distance = 10.f; //Number large enough to never be the closest

            for (int i = 0; i < NUMBER_OF_WHEEL_ELEMENTS; i++)
            {
                if (abs(wheelElements[i].angle) < distance)
                {
                    closetsIcon = i;
                    distance = abs(wheelElements[i].angle);
                }
            }

            rotatingStepCounter = 0;
            startValue = wheelElements[0].angle;
            moveDistance = - wheelElements[closetsIcon].angle;
            rotatingSteps = MAX(abs(floorf(moveDistance * 40)), 2);
            rotating = true;
        }
        //If a drag is not performed between press and release touch, either move to the selected icon, or start animation to the selected screen
        else
        {
            if (selectedDemoName.getRect().intersect(evt.getX(), evt.getY()))
            {
                for (int j = 0; j < NUMBER_OF_WHEEL_ELEMENTS; j++)
                {
                    if (j != centerIcon)
                    {
                        wheelElements[j].icon.startFadeAnimation(0, 55);
                    }
                }

                animateCircleBounce = true;
                rotatingSteps = Constants::MAIN_EXIT_BOUNCE_ANIMATION_DURATION;
                rotatingStepCounter = 0;
                return;
            }
            else
            {
                bool wheellIconReleased = false;

                for (int i = 0; i < NUMBER_OF_WHEEL_ELEMENTS; i++)
                {
                    if (wheelElements[i].icon.getRect().intersect(evt.getX(), evt.getY()))
                    {
                        wheellIconReleased = true;
                        if (centerIcon == i)
                        {
                            for (int j = 0; j < NUMBER_OF_WHEEL_ELEMENTS; j++)
                            {
                                if (j != i)
                                {
                                    wheelElements[j].icon.startFadeAnimation(0, 55);
                                }
                            }

                            animateCircleBounce = true;
                            rotatingSteps = Constants::MAIN_EXIT_BOUNCE_ANIMATION_DURATION;
                            rotatingStepCounter = 0;

                            return;
                        }
                        else
                        {
                            moveDistance = -wheelElements[i].angle;

                            rotatingStepCounter = 0;
                            startValue = wheelElements[0].angle;
                            rotatingSteps = MAX(abs(floorf(moveDistance * 40)), 2);
                            rotating = true;
                            return;
                        }
                    }
                }

                if (wheellIconReleased == false)
                {
                    selectedDemoName.invalidate();
                    selectedDemoName.setX(Constants::SUBDEMO_NAME_TEXT_START_X);
                    selectedDemoName.setTypedText(TypedText(wheelElements[centerIcon].textId));
                    selectedDemoName.startMoveAnimation(Constants::SUBDEMO_NAME_TEXT_END_X, selectedDemoName.getY(), Constants::SUBDEMO_NAME_TEXT_MOVE_ANIMATION_DURATION, EasingEquations::bounceEaseOut);
                    selectedDemoName.startFadeAnimation(255, Constants::SUBDEMO_NAME_TEXT_MOVE_ANIMATION_DURATION);
                    switchActiveIcons(true);
                }
            }
        }
    }
}

//In handle drag the angle detected when we have a press event is used to calculate wheere to movw the wheel to
void MainWheel::handleDragEvent(const DragEvent& evt)
{
    if (!wheelElements[1].icon.isVisible() || animateCircleBounce || animateCircleOut)
    {
        return;
    }
    switchActiveIcons(false);

    dragEventHappend = true;

    const float changeInAngle = calculateAngleFromCoordinate(evt.getNewX(), evt.getNewY());

    for (int i = 0; i < NUMBER_OF_WHEEL_ELEMENTS; i++)
    {
        wheelElements[i].angle = normalizeAngle(wheelElements[i].angle - (lastAngle - changeInAngle));
        updateDrawingOfIcon(wheelElements[i]);
    }
    oldAngle = lastAngle;
    lastAngle = changeInAngle;
    if (selectedDemoName.getAlpha() != 0)
    {
        selectedDemoName.startFadeAnimation(0, 10);
    }


    signalWheelMovement();
}

void MainWheel::handleTickEvent()
{
    if (rotating)
    {
        signalWheelMovement();

        /*When wheel is set to rotate every tick the next angle is calculated until the desired angel is achived*/
        float newAngle = FloatEasingEquations::cubicEaseOut(rotatingStepCounter, startValue, moveDistance, rotatingSteps);

        for (int i = 0; i < NUMBER_OF_WHEEL_ELEMENTS; i++)
        {
            wheelElements[i].angle = normalizeAngle(newAngle + angle_between_elements * i);
            updateDrawingOfIcon(wheelElements[i]);

            if (!wheelElements[i].icon.isVisible())
            {
                if (abs(wheelElements[i].angle) > 3.0f) // set the icons visible when they pass behind the button
                {
                    wheelElements[i].icon.setVisible(true);
                }
            }
        }

        // The rotating step counter, counts up until it has hit the desired steps (rotatingSteps + 1) then it informs handleTick to stop rotating (rotating = false) before it looks for the icon that is placed in the selected icon posistion
        rotatingStepCounter++;
        if (rotatingStepCounter > rotatingSteps)
        {
            rotating = false;
            for (int i = 0; i < NUMBER_OF_WHEEL_ELEMENTS; i++)
            {
                if (abs(wheelElements[i].angle) < 0.1)
                {
                    centerIcon = i;
                }
            }
            setTouchable(true);
            selectedDemoName.invalidate();
            selectedDemoName.setX(Constants::SUBDEMO_NAME_TEXT_START_X);
            selectedDemoName.setTypedText(TypedText(wheelElements[centerIcon].textId));
            selectedDemoName.startMoveAnimation(Constants::SUBDEMO_NAME_TEXT_END_X, selectedDemoName.getY(), Constants::SUBDEMO_NAME_TEXT_MOVE_ANIMATION_DURATION, EasingEquations::bounceEaseOut);
            selectedDemoName.startFadeAnimation(255, Constants::SUBDEMO_NAME_TEXT_MOVE_ANIMATION_DURATION);
            switchActiveIcons(true);
        }
    }

    else if (animateCircleBounce)
    {
        wheelElements[centerIcon].icon.invalidateWheelContent();
        float scaleSbustration = 0.25f * rotatingStepCounter / rotatingSteps + 1;
        float newScale = FloatEasingEquations::backEaseInOut(rotatingStepCounter, 1, 0.25f, rotatingSteps) - scaleSbustration;
        wheelElements[centerIcon].icon.setIconScale(1 + newScale);
        wheelElements[centerIcon].icon.invalidateWheelContent();

        rotatingStepCounter++;
        if (rotatingStepCounter > rotatingSteps)
        {
            Application::getInstance()->unregisterTimerWidget(this);
            animateCircleBounce = false;
            subDemoSelected(centerIcon % NUMBER_OF_SUB_DEMOS);
        }
    }

    //scale up animation when active icon is shown
    if (wheelElements[centerIcon].icon.getIconScale() < 1.15f && activeIconsSelected)
    {
        wheelElements[centerIcon].icon.setIconScale(wheelElements[centerIcon].icon.getIconScale() + 0.01f);
        wheelElements[centerIcon].icon.invalidateWheelContent();
    }
}

void MainWheel::updateIconSize(menuElement& element)
{
    const float absoluteAngle = abs(element.angle);
    const float scale = 1.0f - 0.25f * absoluteAngle;
    element.icon.setIconScale(scale);
}

float MainWheel::getElementAngle(const uint8_t index) const
{
    float angle = 0.0f;
    if (index < NUMBER_OF_WHEEL_ELEMENTS)
    {
        angle = wheelElements[index].angle;
    }
    return angle;
}

uint32_t MainWheel::getElementTextId(const uint8_t index) const
{
    uint32_t id = 0;
    if (index < NUMBER_OF_WHEEL_ELEMENTS)
    {
        id = wheelElements[index].textId;
    }
    return id;
}

void MainWheel::updateDrawingOfIcon(menuElement& element)
{
    element.icon.invalidateWheelContent();

    const int16_t newX = ORIGIN_X - (int16_t)(cosf(element.angle) * radius) - element.icon.getWidth() / 2;
    const int16_t newY = ORIGIN_Y + (int16_t)(sinf(element.angle) * radius) - element.icon.getHeight() / 2;
    element.icon.setXY(newX, newY);

    element.icon.updateZAngle(element.angle * 3);

    updateIconSize(element);

    element.icon.invalidateWheelContent();
}

float MainWheel::calculateAngleFromCoordinate(int16_t x, int16_t y)
{
    float angle = atan2f(y - float(ORIGIN_Y), float(ORIGIN_X) - x);
    return angle;
}

float MainWheel::normalizeAngle(float angle)
{
    // Make sure that angle is in the range -PI : +PI
    while (angle > Constants::PI)
    {
        angle -= 2 * Constants::PI;
    }
    while (angle < -Constants::PI)
    {
        angle += 2 * Constants::PI;
    }
    return angle;
}

void MainWheel::startWheelAnimation()
{
    confirmButton.startMoveAnimation(380, 209, 30, EasingEquations::circEaseOut, EasingEquations::circEaseOut);
    Application::getInstance()->registerTimerWidget(this);
    confirmButton.setTouchable(false);

    // Start intro movement of icons
    rotatingSteps = 140;
    moveDistance = 2 * PI;
    rotatingStepCounter = 0;
    startValue = wheelElements[0].angle;
    rotating = true;
}

void MainWheel::switchActiveIcons(bool active)
{
    wheelElements[centerIcon].icon.setIcon(switchToSelectedIcon(active, getElementTextId(centerIcon)));
    wheelElements[centerIcon].icon.invalidateWheelContent();
    confirmButton.setBitmaps(active ? BITMAP_CONFIRM_BUTTON_ACTIVE_ID : BITMAP_CONFIRM_BUTTON_INACTIVE_ID, BITMAP_CONFIRM_BUTTON_PRESSED_ID);
    confirmButton.setTouchable(active);
    confirmButton.invalidate();
    activeIconsSelected = active;
}

uint16_t MainWheel::switchToSelectedIcon(bool active, uint32_t myTextEnum)
{

    switch (myTextEnum)
    {
    case T_MAINSELECTEDSUBDEMOSVG:
        return active ? BITMAP_SVG_ICON_SELECTED_ID : BITMAP_SVG_ICON_NORMAL_ID;
    case T_MAINSELECTEDSUBDEMODICE:
        return active ? BITMAP_DICE_ICON_SELECTED_ID : BITMAP_DICE_ICON_NORMAL_ID;
    // case T_MAINSELECTEDSUBDEMOMJPEG:
    //     return active ? BITMAP_MJPEG_ICON_SELECTED_ID : BITMAP_MJPEG_ICON_NORMAL_ID;
    case T_MAINSELECTEDSUBDEMOCOMPASS:
        return active ? BITMAP_COMPASS_ICON_SELECTED_ID : BITMAP_COMPASS_ICON_NORMAL_ID;
    case T_MAINSELECTEDSUBDEMOTRANSITION:
        return active ? BITMAP_TRANSITIONS_ICON_SELECTED_ID : BITMAP_TRANSITIONS_ICON_NORMAL_ID;
    case T_MAINSELECTEDSUBDEMOEBIKE:
        return active ? BITMAP_EBIKE_ICON_SELECTED_ID : BITMAP_EBIKE_ICON_NORMAL_ID;
    default:
        return 0;
    }
}

void MainWheel::confirmButtonClicked()
{
    animateCircleBounce = true;
    rotatingSteps = Constants::MAIN_EXIT_BOUNCE_ANIMATION_DURATION;
    rotatingStepCounter = 0;
    confirmButton.setTouchable(false);
}
