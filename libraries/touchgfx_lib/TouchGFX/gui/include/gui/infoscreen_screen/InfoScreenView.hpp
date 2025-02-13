#ifndef INFOSCREENVIEW_HPP
#define INFOSCREENVIEW_HPP

#include <gui_generated/infoscreen_screen/InfoScreenViewBase.hpp>
#include <gui/infoscreen_screen/InfoScreenPresenter.hpp>

class InfoScreenView : public InfoScreenViewBase
{
public:
    InfoScreenView();
    virtual ~InfoScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
};

#endif // INFOSCREENVIEW_HPP
