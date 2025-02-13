#include <gui/containers/sphere_wheel.hpp>
#include <gui/model/Model.hpp>
#include <math.h>

#include <touchgfx/Utils.hpp>

static const int radiusX = 270;
static const int radiusY = 40;
static const int originX = 235;
static const int originY = 32;

static const float PI_HALF = PI / 2;
static const float PI_QUARTER = PI / 4;
static const float angleFactor = 1.4f;

static const float sphereMinimumScale = 0.60f;
static const float spheereScaleFactor = 0.01f;

sphere_wheel::sphere_wheel() :
    lastAngle(0.0f),
    steps(0.0f),
    stepCounter(0.0f),
    moveDistance(0.0f),
    startValue(0.0f),
    animate(false),
    wasAnimating(false),
    dragged(false)
{
}

void sphere_wheel::initialize()
{
    sphere_wheelBase::initialize();

    Application::getInstance()->registerTimerWidget(this);

    menuWheelSpheres[0].element = &wheel_element_1;
    menuWheelSpheres[1].element = &wheel_element_2;
    menuWheelSpheres[2].element = &wheel_element_3;
    menuWheelSpheres[3].element = &wheel_element_4;
}

void sphere_wheel::startWheel(uint8_t selectedElement)
{
    for (int i = 0; i < NUMBER_OF_WHEEL_ELEMENTS; i++)
    {
        float newX, newY;
        const uint8_t subDemoFocusOffset = (i + selectedElement + (NUMBER_OF_SUB_DEMOS - 1)) % NUMBER_OF_SUB_DEMOS;

        menuWheelSpheres[i].element->setSphereImage(subDemoBitmapIds[subDemoFocusOffset]);

        menuWheelSpheres[i].angle = PI_HALF * i;
        newX = originX + cosf(menuWheelSpheres[i].angle) * radiusX;
        newY = originY + sinf(menuWheelSpheres[i].angle) * radiusY;

        menuWheelSpheres[i].element->setXY((int)newX, (int)newY);

        float deltaY = newY - originY;
        menuWheelSpheres[i].element->setSphereScale(sphereMinimumScale + deltaY * spheereScaleFactor);
        menuWheelSpheres[i].element->setAlpha(deltaY);

        menuWheelSpheres[i].demo = (SubDemo)subDemoFocusOffset;
    }

    const uint16_t INTRO_TIME = 20;
    const EasingEquation INTRO_EASING = EasingEquations::cubicEaseOut;

    const int16_t centerFinalY = wheel_element_2.getY();
    wheel_element_2.setY(480);
    wheel_element_2.startMoveAnimation(wheel_element_2.getX(), centerFinalY, INTRO_TIME, INTRO_EASING, INTRO_EASING);

    const int16_t rightFinalX = wheel_element_1.getX();
    wheel_element_1.setX(800);
    wheel_element_1.startMoveAnimation(rightFinalX, wheel_element_1.getY(), INTRO_TIME, INTRO_EASING, INTRO_EASING);

    const int16_t leftFinalX = wheel_element_3.getX();
    wheel_element_3.setX(-wheel_element_3.getWidth());
    wheel_element_3.startMoveAnimation(leftFinalX, wheel_element_3.getY(), INTRO_TIME, INTRO_EASING, INTRO_EASING);
}

void sphere_wheel::handleClickEvent(const ClickEvent& evt)
{
    if (evt.getType() == ClickEvent::PRESSED)
    {
        wasAnimating = animate;
        if (animate)
        {
            animate = false;
            Application::getInstance()->unregisterTimerWidget(this);
        }
        dragged = false;

        float b = evt.getX() - (float)originX;
        float a = evt.getY() - (float)originY;
        float c = sqrtf(a * a + b * b);

        if (evt.getX() >= originX)
        {
            lastAngle = asinf(a / c) * angleFactor;
        }
        else if (evt.getX() < originX)
        {
            lastAngle = (PI_HALF + (PI_HALF - asinf(a / c))) * angleFactor;
        }
    }
    else // ClickEvent::RELEASED
    {
        if (!animate)
        {
            if (dragged || wasAnimating)
            {
                animateToClosesItem();
            }
            else
            {
                if (evt.getX() > 235 && evt.getX() < getWidth() - 235) // center sphere is clicked
                {
                    emitItemSelectedCallback(menuWheelSpheres[getCenterSphere()].demo);
                }
                else
                {
                    startValue = 0;
                    moveDistance = evt.getX() > getWidth() / 2 ? PI_HALF : -PI_HALF; // rotate quater turn left or right, depending on which side was clicked
                    stepCounter = 0;
                    steps = 60;
                    lastAngle = 0;

                    emitAnimationStartedCallback();

                    animate = true;
                    Application::getInstance()->registerTimerWidget(this);
                }
            }
            dragged = false;
        }
    }
}

void sphere_wheel::handleDragEvent(const DragEvent& evt)
{
    if (!animate && !dragged)
    {
        emitAnimationStartedCallback();
    }

    dragged = true;

    float tmpAngle;

    float b = (float)(evt.getNewX() - originX);
    float a = (float)evt.getNewY() - originY;
    float c = sqrtf(a * a + b * b);

    if (evt.getNewX() >= originX)
    {
        tmpAngle = asinf(a / c) * angleFactor;
    }
    else if (evt.getNewX() < originX)
    {
        tmpAngle = (PI_HALF + (PI_HALF - asinf(a / c))) * angleFactor;
    }
    moveIcons(lastAngle - tmpAngle);
    lastAngle = tmpAngle;
}

void sphere_wheel::handleGestureEvent(const GestureEvent& evt)
{
    if (evt.getType() == GestureEvent::SWIPE_HORIZONTAL)
    {
        float velocityAbsolute = (float)abs(evt.getVelocity());

        steps = floorf(velocityAbsolute * 1.3f);
        stepCounter = 0;
        startValue = 0;
        lastAngle = 0;

        moveDistance = ((evt.getVelocity() > 0) ? -1 : 1) * (velocityAbsolute * 0.1f);

        float offValue = fmodf(menuWheelSpheres[0].angle + moveDistance, PI_HALF);

        if (offValue > PI_QUARTER && moveDistance > 0)
        {
            moveDistance += PI_HALF - offValue;
        }
        else if (offValue < -PI_QUARTER && moveDistance < 0)
        {
            moveDistance -= (PI_HALF + offValue);
        }
        else
        {
            moveDistance -= offValue;
        }

        if (!animate && !dragged)
        {
            emitAnimationStartedCallback();
        }

        animate = true;
        Application::getInstance()->registerTimerWidget(this);
    }
}

void sphere_wheel::handleTickEvent()
{
    if (!this->isTouchable())
    {
        if (!wheel_element_2.isMoveAnimationRunning())
        {
            this->setTouchable(true);
            Application::getInstance()->unregisterTimerWidget(this);
            emitAnimationEnddedCallback(menuWheelSpheres[getCenterSphere()].demo);
        }
    }

    if (animate)
    {
        float newAngle = floatCubicEaseOut(stepCounter, startValue, moveDistance, steps);
        moveIcons(lastAngle - newAngle);
        lastAngle = newAngle;
        stepCounter++;
        if (stepCounter > steps)
        {
            animate = false;
            Application::getInstance()->unregisterTimerWidget(this);
            emitAnimationEnddedCallback(menuWheelSpheres[getCenterSphere()].demo);
        }
    }
}

void sphere_wheel::animateToClosesItem()
{
    float offValue = fmodf(menuWheelSpheres[0].angle, PI_HALF);

    moveDistance = 0;

    if (offValue > PI_QUARTER && offValue > 0)
    {
        moveDistance = PI_HALF - offValue;
    }
    else if (offValue < -PI_QUARTER && offValue < 0)
    {
        moveDistance = -(PI_HALF + offValue);
    }
    else
    {
        moveDistance = -offValue;
    }

    steps = MAX(abs(floorf(moveDistance * 40)), 2);
    stepCounter = 0;
    startValue = 0;
    lastAngle = 0;

    if (!animate && !dragged)
    {
        emitAnimationStartedCallback();
    }

    animate = true;
    Application::getInstance()->registerTimerWidget(this);
}

void sphere_wheel::moveIcons(float deltaAngle)
{
    float newX = 0;
    float newY = 0;

    for (int i = 0; i < NUMBER_OF_WHEEL_ELEMENTS; i++)
    {
        bool prevVisibleState = menuWheelSpheres[i].element->isVisible();
        menuWheelSpheres[i].element->invalidateCustom();
        menuWheelSpheres[i].angle = fmodf(menuWheelSpheres[i].angle - deltaAngle, 2 * PI);

        newX = originX + cosf(menuWheelSpheres[i].angle) * radiusX;
        newY = originY + sinf(menuWheelSpheres[i].angle) * radiusY;

        menuWheelSpheres[i].element->setXY((int)newX, (int)newY);

        float deltaY = newY - originY;
        menuWheelSpheres[i].element->setSphereScale(sphereMinimumScale + deltaY * spheereScaleFactor);
        menuWheelSpheres[i].element->setAlpha(deltaY);


        if (menuWheelSpheres[i].element->isVisible() != prevVisibleState && menuWheelSpheres[i].element->isVisible())
        {
            if (NUMBER_OF_NON_VISIBLE_DEMOS >= 0)
            {
                uint16_t newDemo;
                uint16_t newCenterId;
                if (menuWheelSpheres[i].element->getX() > 200) // If X is larger than 200 the new demo button becomes visible in the right side of the screen and vice versa
                {
                    newCenterId = positiveModulo((i + 1), NUMBER_OF_WHEEL_ELEMENTS);
                    newDemo = positiveModulo((menuWheelSpheres[newCenterId].demo - 1), NUMBER_OF_SUB_DEMOS);
                }
                else
                {
                    newCenterId = positiveModulo((i - 1), NUMBER_OF_WHEEL_ELEMENTS);
                    newDemo = positiveModulo((menuWheelSpheres[newCenterId].demo + 1), NUMBER_OF_SUB_DEMOS);
                }
                menuWheelSpheres[i].element->setSphereImage(subDemoBitmapIds[newDemo]);
                menuWheelSpheres[i].demo = SubDemo(newDemo);
            }
        }
        menuWheelSpheres[i].element->invalidateCustom();
    }
    reorder();
}

void sphere_wheel::reorder()
{
    for (int i = 0; i < 4; i++)
    {
        remove(*menuWheelSpheres[i].element);
    }
    bool added[4] = { false };
    uint8_t nextItemToAdd = 0;

    for (int i = 0; i < 4; i++)
    {
        int16_t lowestY = 480;
        for (int j = 0; j < 4; j++)
        {
            if ((menuWheelSpheres[j].element->getY() < lowestY) && (added[j] == false))
            {
                nextItemToAdd = j;
                lowestY = menuWheelSpheres[j].element->getY();
            }
        }
        added[nextItemToAdd] = true;
        add(*menuWheelSpheres[nextItemToAdd].element);
    }
}

int16_t sphere_wheel::getCenterSphere()
{
    for (int i = 0; i < NUMBER_OF_WHEEL_ELEMENTS; i++)
    {
        if (menuWheelSpheres[i].angle > (PI_HALF - 0.1) && menuWheelSpheres[i].angle < (PI_HALF + 0.1))
        {
            return i;
        }
        else if (menuWheelSpheres[i].angle > (-PI - PI_HALF - 0.1) && menuWheelSpheres[i].angle < (-PI - PI_HALF + 0.1))
        {
            return i;
        }
    }
    return -1;
}

uint16_t sphere_wheel::positiveModulo(int16_t i, int16_t n)
{
    return (n + (i % n)) % n;
}

float sphere_wheel::floatCubicEaseOut(float t, float b, float c, float d)
{
    t /= d;
    t--;
    return c * (t * t * t + 1) + b;
}

void sphere_wheel::startIconAnimation()
{
    menuWheelSpheres[getCenterSphere()].element->startAnimation();
}

void sphere_wheel::stopIconAnimation()
{
    for (uint16_t i = 0; i < NUMBER_OF_WHEEL_ELEMENTS; i++)
    {
        menuWheelSpheres[i].element->stopAnimation();
    }
}
