/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#ifndef MCULOADPERCENTAGE_MENUBASE_HPP
#define MCULOADPERCENTAGE_MENUBASE_HPP

#include <gui/common/FrontendApplication.hpp>
#include <touchgfx/containers/Container.hpp>
#include <touchgfx/widgets/TextAreaWithWildcard.hpp>
#include <touchgfx/widgets/TextArea.hpp>

class MCULoadPercentage_MenuBase : public touchgfx::Container
{
public:
    MCULoadPercentage_MenuBase();
    virtual ~MCULoadPercentage_MenuBase();
    virtual void initialize();

protected:
    FrontendApplication& application() {
        return *static_cast<FrontendApplication*>(touchgfx::Application::getInstance());
    }

    /*
     * Member Declarations
     */
    touchgfx::TextAreaWithOneWildcard mcuValue;
    touchgfx::TextArea mcuTitle;

    /*
     * Wildcard Buffers
     */
    static const uint16_t MCUVALUE_SIZE = 4;
    touchgfx::Unicode::UnicodeChar mcuValueBuffer[MCUVALUE_SIZE];

private:

};

#endif // MCULOADPERCENTAGE_MENUBASE_HPP
