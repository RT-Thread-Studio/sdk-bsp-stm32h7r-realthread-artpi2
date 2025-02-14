#include <gui/containers/FrameRate_SVG.hpp>

FrameRate_SVG::FrameRate_SVG()
{

}

void FrameRate_SVG::initialize()
{
    FrameRate_SVGBase::initialize();
}

void FrameRate_SVG::updateShownFPS()
{
    Unicode::snprintf(fpsValueBuffer, FPSVALUE_SIZE, "%d", fps);
    fpsValue.invalidate();
}
