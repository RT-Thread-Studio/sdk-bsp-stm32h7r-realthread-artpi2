#ifndef VIDEOINTROVIEW_HPP
#define VIDEOINTROVIEW_HPP

#include <gui_generated/videointro_screen/VideoIntroViewBase.hpp>
#include <gui/videointro_screen/VideoIntroPresenter.hpp>

class VideoIntroView : public VideoIntroViewBase
{
public:
    VideoIntroView();
    virtual ~VideoIntroView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
};

#endif // VIDEOINTROVIEW_HPP
