#include <gui/containers/topbarContainer_ScreenTransition.hpp>
#include "gui/common/constans.hpp"

topbarContainer_ScreenTransition::topbarContainer_ScreenTransition()
{

}

void topbarContainer_ScreenTransition::initialize()
{
    topbarContainer_ScreenTransitionBase::initialize();
}

void topbarContainer_ScreenTransition::checkMCULoad()
{
    topbar.checkMCULoad();
}

void topbarContainer_ScreenTransition::countTheFrames()
{
    topbar.countTheFrames();
}
