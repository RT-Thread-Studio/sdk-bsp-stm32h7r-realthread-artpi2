#ifndef VIDEOINTROSCREENVIEW_HPP
#define VIDEOINTROSCREENVIEW_HPP

#include <gui_generated/videointroscreen_screen/VideoIntroScreenViewBase.hpp>
#include <gui/videointroscreen_screen/VideoIntroScreenPresenter.hpp>

class VideoIntroScreenView : public VideoIntroScreenViewBase
{
public:
    VideoIntroScreenView();
    virtual ~VideoIntroScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
};

#endif // VIDEOINTROSCREENVIEW_HPP
