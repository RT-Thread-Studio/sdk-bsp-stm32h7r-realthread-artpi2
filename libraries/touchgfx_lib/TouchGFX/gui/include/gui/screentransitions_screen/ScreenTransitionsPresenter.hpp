#ifndef SCREENTRANSITIONSPRESENTER_HPP
#define SCREENTRANSITIONSPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class ScreenTransitionsView;

class ScreenTransitionsPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    ScreenTransitionsPresenter(ScreenTransitionsView& v);

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

    virtual ~ScreenTransitionsPresenter() {};

private:
    ScreenTransitionsPresenter();

    ScreenTransitionsView& view;
};

#endif // SCREENTRANSITIONSPRESENTER_HPP
