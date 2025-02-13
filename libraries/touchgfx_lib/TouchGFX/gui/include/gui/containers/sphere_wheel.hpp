#ifndef SPHERE_WHEEL_HPP
#define SPHERE_WHEEL_HPP

#include <gui_generated/containers/sphere_wheelBase.hpp>
#include <gui/common/subeDemoInfo.hpp>

class sphere_wheel : public sphere_wheelBase
{
public:
    sphere_wheel();
    virtual ~sphere_wheel() {}

    virtual void initialize();
    virtual void startWheel(uint8_t selectedElement);


    virtual void handleClickEvent(const ClickEvent& evt);
    virtual void handleDragEvent(const DragEvent& evt);
    virtual void handleGestureEvent(const GestureEvent& evt);

    virtual void handleTickEvent();

    void animateToClosesItem();
    void moveIcons(float deltaAngle);
    void reorder();

    int16_t getCenterSphere();
    uint16_t positiveModulo(int16_t i, int16_t n);
    float floatCubicEaseOut(float t, float b, float c, float d);
    void startIconAnimation();
    void stopIconAnimation();

protected:
    static const int16_t NUMBER_OF_WHEEL_ELEMENTS = 4;
    static const int16_t NUMBER_OF_NON_VISIBLE_DEMOS = NUMBER_OF_SUB_DEMOS - NUMBER_OF_WHEEL_ELEMENTS;

    struct WheelSpheres
    {
        wheel_element* element;
        float angle;
        SubDemo demo;
    } menuWheelSpheres[NUMBER_OF_WHEEL_ELEMENTS];

    float lastAngle;

    float steps;
    float stepCounter;
    float moveDistance;
    float startValue;

    bool animate;
    bool wasAnimating;
    bool dragged;
};

#endif // SPHERE_WHEEL_HPP
