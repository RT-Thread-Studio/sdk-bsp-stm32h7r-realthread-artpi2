#include <gui/containers/scrollWheelCoverFlow.hpp>

scrollWheelCoverFlow::scrollWheelCoverFlow()
{

}

void scrollWheelCoverFlow::initialize()
{
    scrollWheelCoverFlowBase::initialize();
    scrollWheel.animateToItem(2, 0); // Force a redraw and calculation
    scrollWheel.animateToItem(3, 0);
}
