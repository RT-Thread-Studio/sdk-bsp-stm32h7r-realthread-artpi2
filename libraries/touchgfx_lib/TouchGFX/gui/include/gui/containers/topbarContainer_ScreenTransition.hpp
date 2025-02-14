#ifndef TOPBARCONTAINER_SCREENTRANSITION_HPP
#define TOPBARCONTAINER_SCREENTRANSITION_HPP

#include <gui_generated/containers/topbarContainer_ScreenTransitionBase.hpp>

class topbarContainer_ScreenTransition : public topbarContainer_ScreenTransitionBase
{
public:
    topbarContainer_ScreenTransition();
    virtual ~topbarContainer_ScreenTransition() {}

    virtual void initialize();

    void checkMCULoad();
    void countTheFrames();
protected:
};

#endif // TOPBARCONTAINER_SCREENTRANSITION_HPP
