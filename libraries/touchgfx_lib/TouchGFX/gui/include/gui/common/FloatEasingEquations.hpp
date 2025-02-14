#pragma once
class FloatEasingEquations
{
public:
    FloatEasingEquations();
    ~FloatEasingEquations() {}

    static float cubicEaseOut(float t, float b, float c, float d);
    static float backEaseInOut(float t, float b, float c, float d);
    static float easeNone(float t, float b, float c, float d);
    static float backEaseOut(float t, float b, float c, float d);
    static float bounceEaseOut(float t, float b, float c, float d);

    static float floatCubicEaseIn(float t, float b, float c, float d);
    static float floatCubicEaseOut(float t, float b, float c, float d);
    static float floatCubicEaseInOut(float t, float b, float c, float d);
    static float floatBackEaseOut(float t, float b, float c, float d);

private:

};
