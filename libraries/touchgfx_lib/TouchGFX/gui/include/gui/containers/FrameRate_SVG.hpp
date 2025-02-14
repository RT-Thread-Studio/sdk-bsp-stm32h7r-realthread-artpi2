#ifndef FRAMERATE_SVG_HPP
#define FRAMERATE_SVG_HPP

#include <gui_generated/containers/FrameRate_SVGBase.hpp>
#include <gui/common/FrameRate.hpp>

class FrameRate_SVG : public FrameRate_SVGBase, public FrameRate
{
public:
    FrameRate_SVG();
    virtual ~FrameRate_SVG() {}

    virtual void initialize();
protected:
    virtual void updateShownFPS();
};

#endif // FRAMERATE_SVG_HPP
