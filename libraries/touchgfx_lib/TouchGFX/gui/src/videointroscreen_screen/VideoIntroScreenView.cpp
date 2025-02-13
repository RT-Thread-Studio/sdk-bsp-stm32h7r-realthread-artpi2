#include <gui/videointroscreen_screen/VideoIntroScreenView.hpp>

VideoIntroScreenView::VideoIntroScreenView()
{

}

void VideoIntroScreenView::setupScreen()
{
    VideoIntroScreenViewBase::setupScreen();
    HAL::getInstance()->setFrameRateCompensation(true);
}

void VideoIntroScreenView::tearDownScreen()
{
    VideoIntroScreenViewBase::tearDownScreen();
}
