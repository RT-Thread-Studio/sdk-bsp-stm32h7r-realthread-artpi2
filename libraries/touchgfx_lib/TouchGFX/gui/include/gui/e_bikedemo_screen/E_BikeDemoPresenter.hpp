#ifndef E_BIKEDEMOPRESENTER_HPP
#define E_BIKEDEMOPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class E_BikeDemoView;

class E_BikeDemoPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    E_BikeDemoPresenter(E_BikeDemoView& v);

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

    virtual ~E_BikeDemoPresenter() {}

private:
    E_BikeDemoPresenter();

    E_BikeDemoView& view;
};

#endif // E_BIKEDEMOPRESENTER_HPP
