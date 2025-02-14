#ifndef SCREENTRANSITIONDEMOPRESENTER_HPP
#define SCREENTRANSITIONDEMOPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class ScreenTransitionDemoView;

class ScreenTransitionDemoPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    ScreenTransitionDemoPresenter(ScreenTransitionDemoView& v);

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

    virtual ~ScreenTransitionDemoPresenter() {}

private:
    ScreenTransitionDemoPresenter();

    ScreenTransitionDemoView& view;
};

#endif // SCREENTRANSITIONDEMOPRESENTER_HPP
