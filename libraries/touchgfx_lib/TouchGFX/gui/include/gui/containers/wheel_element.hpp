#ifndef WHEEL_ELEMENT_HPP
#define WHEEL_ELEMENT_HPP

#include <gui_generated/containers/wheel_elementBase.hpp>

class wheel_element : public wheel_elementBase
{
public:
    wheel_element();
    virtual ~wheel_element() {}

    virtual void initialize();

    void setSphereImage(BitmapId bmpId);
    void setSphereScale(float scale);
    void setAlpha(float deltaY);

    void startAnimation();
    void stopAnimation();
    void invalidateCustom();
protected:
};

#endif // WHEEL_ELEMENT_HPP
