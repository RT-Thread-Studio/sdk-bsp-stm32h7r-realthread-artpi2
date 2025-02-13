#ifndef COMPASSSCREENPRESENTER_HPP
#define COMPASSSCREENPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class CompassScreenView;

class CompassScreenPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    CompassScreenPresenter(CompassScreenView& v);

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

    virtual ~CompassScreenPresenter() {};

private:
    CompassScreenPresenter();

    CompassScreenView& view;
};

#endif // COMPASSSCREENPRESENTER_HPP
