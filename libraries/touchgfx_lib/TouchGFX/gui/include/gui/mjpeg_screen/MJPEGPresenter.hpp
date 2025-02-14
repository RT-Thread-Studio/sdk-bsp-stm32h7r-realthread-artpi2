#ifndef MJPEGPRESENTER_HPP
#define MJPEGPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class MJPEGView;

class MJPEGPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    MJPEGPresenter(MJPEGView& v);

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

    virtual ~MJPEGPresenter() {};

private:
    MJPEGPresenter();

    MJPEGView& view;
};

#endif // MJPEGPRESENTER_HPP
