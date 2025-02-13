#ifndef CLOUDCUSTOMCONTAINER_HPP
#define CLOUDCUSTOMCONTAINER_HPP

#include <gui_generated/containers/cloudCustomContainerBase.hpp>

class cloudCustomContainer : public cloudCustomContainerBase
{
public:
    cloudCustomContainer();
    virtual ~cloudCustomContainer() {}

    virtual void initialize();

    //functions for clouds behaviour
    virtual void fadeIn();
    virtual void fadeOut();
    virtual void moveClouds();
    virtual void repositionClouds();
    virtual void stopClouds();
    virtual void handleTick();
    virtual bool isFadeInProgress();
    virtual void imageClickHandler(const touchgfx::Image& cloud, const ClickEvent& e);
    virtual void changeTransparency(touchgfx::Image& cloud);

protected:

private :
    static constexpr int CLOUDS_FADE_NONE = 255;    //alpha value
    static constexpr int CLOUDS_FADE_LIGHT = 155;
    static constexpr int CLOUDS_FADE_STRONG = 55;
    static constexpr int CLOUDS_FADE_FULL = 0;
    static constexpr int CLOUDS_FADE_SPEED = 4;

    virtual void moveCloud(touchgfx::Image& cloud);
    virtual void fadeCloudIn(touchgfx::Image& cloud);
    virtual void fadeCloudOut(touchgfx::Image& cloud);

    //Callback for image : triggered when clicked
    Callback <cloudCustomContainer, const touchgfx::Image&, const ClickEvent&> imageClickedCallback;
    bool fadeInRunning;
    bool fadeOutRunning;
    bool cloudsAreMoving;
    uint32_t moveCounter;
};

#endif // CLOUDCUSTOMCONTAINER_HPP
