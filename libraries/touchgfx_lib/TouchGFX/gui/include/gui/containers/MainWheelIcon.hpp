#ifndef MAINWHEELICON_HPP
#define MAINWHEELICON_HPP

#include <gui_generated/containers/MainWheelIconBase.hpp>

class MainWheelIcon : public MainWheelIconBase
{
public:
    MainWheelIcon();
    virtual ~MainWheelIcon() {}

    virtual void initialize();
    void invalidateWheelContent();

    void setIconScale(float size);
    float getIconScale();
    void setIcon(uint16_t icon);

    virtual void setAlpha(uint8_t alpha);
    virtual uint8_t getAlpha();

    void updateZAngle(float angle);

protected:
};

#endif // MAINWHEELICON_HPP
