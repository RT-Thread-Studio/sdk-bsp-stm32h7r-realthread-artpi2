#ifndef VIDEOINTROSCREENPRESENTER_HPP
#define VIDEOINTROSCREENPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class VideoIntroScreenView;

class VideoIntroScreenPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    VideoIntroScreenPresenter(VideoIntroScreenView& v);

    /**
     * The activate function is called automatically when this screen is "switched in"
     * (ie. made active). Initialization logic can be placed here.
     */
    virtual void activate();

    /**
     * The deactivate function is called automatically when this screen is "switched out"
     * (ie. made inactive). Teardown functionality can be placed here.
     */
    virtual void deactivate();

    virtual ~VideoIntroScreenPresenter() {};

private:
    VideoIntroScreenPresenter();

    VideoIntroScreenView& view;
};

#endif // VIDEOINTROSCREENPRESENTER_HPP
