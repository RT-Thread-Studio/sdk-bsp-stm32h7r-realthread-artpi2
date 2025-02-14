#ifndef MJPEGVIEW_HPP
#define MJPEGVIEW_HPP

#include <gui_generated/mjpeg_screen/MJPEGViewBase.hpp>
#include <gui/mjpeg_screen/MJPEGPresenter.hpp>

class MJPEGView : public MJPEGViewBase
{
public:
    MJPEGView();
    virtual ~MJPEGView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
    virtual void handleTickEvent();
protected:
};

#endif // MJPEGVIEW_HPP
