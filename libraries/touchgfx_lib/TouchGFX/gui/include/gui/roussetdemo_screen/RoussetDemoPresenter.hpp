#ifndef ROUSSETDEMOPRESENTER_HPP
#define ROUSSETDEMOPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class RoussetDemoView;

class RoussetDemoPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    RoussetDemoPresenter(RoussetDemoView& v);

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

    virtual ~RoussetDemoPresenter() {};

private:
    RoussetDemoPresenter();

    RoussetDemoView& view;
};

#endif // ROUSSETDEMOPRESENTER_HPP
