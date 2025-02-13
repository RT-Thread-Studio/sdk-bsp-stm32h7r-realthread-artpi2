#include "gui/common/FloatEasingEquations.hpp"

FloatEasingEquations::FloatEasingEquations()
{

}

float FloatEasingEquations::easeNone(float t, float b, float c, float d)
{
    return c * t / d + b;
}

float FloatEasingEquations::cubicEaseOut(float t, float b, float c, float d)
{
    t /= d;
    t--;
    return c * (t * t * t + 1) + b;
}

float FloatEasingEquations::backEaseInOut(float t, float b, float c, float d)
{
    float s = 1.70158f;

    if ((t /= d / 2) < 1)
    {
        s = s * (1.525f);
        return c / 2 * (t * t * ((s + 1) * t - s)) + b;
    }

    float postFix = t -= 2;

    s *= (1.525f);
    return c / 2 * ((postFix) * t * ((s + 1) * t + s) + 2) + b;
}

float FloatEasingEquations::backEaseOut(float t, float b, float c, float d)
{
    float s = 1.70158f;
    t = t / d - 1;
    return c * (t * t * ((s + 1) * t + s) + 1) + b;
}

float FloatEasingEquations::bounceEaseOut(float t, float b, float c, float d)
{
    if ((t /= d) < (1 / 2.75f))
    {
        return c * (7.5625f * t * t) + b;
    }
    else if (t < (2 / 2.75f))
    {
        float postFix = t -= (1.5f / 2.75f);
        return c * (7.5625f * (postFix) * t + .75f) + b;
    }
    else if (t < (2.5 / 2.75))
    {
        float postFix = t -= (2.25f / 2.75f);
        return c * (7.5625f * (postFix) * t + .9375f) + b;
    }
    else
    {
        float postFix = t -= (2.625f / 2.75f);
        return c * (7.5625f * (postFix) * t + .984375f) + b;
    }
}

float FloatEasingEquations::floatCubicEaseIn(float t, float b, float c, float d)
{
    t /= d;
    return c * t * t * t + b;
}

float FloatEasingEquations::floatCubicEaseOut(float t, float b, float c, float d)
{
    t /= d;
    t--;
    return c * (t * t * t + 1) + b;
}

float FloatEasingEquations::floatCubicEaseInOut(float t, float b, float c, float d)
{
    t /= d / 2;
    if (t < 1)
    {
        return c / 2 * t * t * t + b;
    }
    t -= 2;
    return c / 2 * (t * t * t + 2) + b;
}

float FloatEasingEquations::floatBackEaseOut(float t, float b, float c, float d)
{
    float s = 1.70158f;
    t = t / d - 1;
    return c * (t * t * ((s + 1) * t + s) + 1) + b;
}
