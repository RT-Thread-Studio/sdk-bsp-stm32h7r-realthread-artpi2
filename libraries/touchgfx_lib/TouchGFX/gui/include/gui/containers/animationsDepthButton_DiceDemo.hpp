#ifndef ANIMATIONSDEPTHBUTTON_DICEDEMO_HPP
#define ANIMATIONSDEPTHBUTTON_DICEDEMO_HPP

#include <gui_generated/containers/animationsDepthButton_DiceDemoBase.hpp>

class animationsDepthButton_DiceDemo : public animationsDepthButton_DiceDemoBase
{
public:
    enum BUTTON_STATE
    {
        STATE_2D = false,
        STATE_3D = true
    };
    animationsDepthButton_DiceDemo();
    virtual ~animationsDepthButton_DiceDemo() {}

    virtual void initialize();

    virtual void touchOverlayPressed();
protected:
    static const int BUTTON_3D_X = 0;
    static const int BUTTON_3D_Y = 126;
    static const int BUTTON_2D_X = 0;
    static const int BUTTON_2D_Y = 0;

    static const int BUTTON_ANIMATION_DURATION = 50;

    BUTTON_STATE currentButtonState;
};

#endif // ANIMATIONSDEPTHBUTTON_DICEDEMO_HPP
