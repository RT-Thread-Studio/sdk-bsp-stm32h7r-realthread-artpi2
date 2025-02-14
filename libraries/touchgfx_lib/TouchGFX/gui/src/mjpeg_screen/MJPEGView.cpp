#include <gui/mjpeg_screen/MJPEGView.hpp>

MJPEGView::MJPEGView()
{

}

void MJPEGView::setupScreen()
{
    MJPEGViewBase::setupScreen();
}

void MJPEGView::tearDownScreen()
{
    MJPEGViewBase::tearDownScreen();
}

void MJPEGView::handleTickEvent()
{
    topbar.countTheFrames();
    topbar.checkMCULoad();
}
