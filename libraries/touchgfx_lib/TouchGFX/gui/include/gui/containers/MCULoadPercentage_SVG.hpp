#ifndef MCULOADPERCENTAGE_SVG_HPP
#define MCULOADPERCENTAGE_SVG_HPP

#include <gui_generated/containers/MCULoadPercentage_SVGBase.hpp>
#include <gui/common/MCULoadPercentage.hpp>

class MCULoadPercentage_SVG : public MCULoadPercentage_SVGBase, public MCULoadPercentage
{
public:
    MCULoadPercentage_SVG();
    virtual ~MCULoadPercentage_SVG() {}

    virtual void initialize();
protected:

    virtual void updateShownMCULoadPercentage();
};

#endif // MCULOADPERCENTAGE_SVG_HPP
