#ifndef E_BIKESCREENPRESENTER_HPP
#define E_BIKESCREENPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class E_BikeScreenView;

class E_BikeScreenPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    E_BikeScreenPresenter(E_BikeScreenView& v);

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

    virtual ~E_BikeScreenPresenter() {};

private:
    E_BikeScreenPresenter();

    E_BikeScreenView& view;
};

#endif // E_BIKESCREENPRESENTER_HPP
