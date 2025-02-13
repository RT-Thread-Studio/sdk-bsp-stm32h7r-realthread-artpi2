#ifndef DICEPRESENTER_HPP
#define DICEPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class DiceView;

class DicePresenter : public touchgfx::Presenter, public ModelListener
{
public:
    DicePresenter(DiceView& v);

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

    virtual ~DicePresenter() {};

private:
    DicePresenter();

    DiceView& view;
};

#endif // DICEPRESENTER_HPP
