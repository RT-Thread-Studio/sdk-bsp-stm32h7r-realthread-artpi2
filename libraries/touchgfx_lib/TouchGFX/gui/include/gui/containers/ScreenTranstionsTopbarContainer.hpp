#ifndef SCREENTRANSTIONSScreenTranstionsTopbarContainer_HPP
#define SCREENTRANSTIONSScreenTranstionsTopbarContainer_HPP


#include <gui_generated/containers/ScreenTranstionsTopbarContainerBase.hpp>

class ScreenTranstionsTopbarContainer : public ScreenTranstionsTopbarContainerBase
{
public:
    ScreenTranstionsTopbarContainer();
    virtual ~ScreenTranstionsTopbarContainer() {}

    virtual void initialize();

    void updateMCU(uint16_t value);
    void updateFPS(uint16_t value);
protected:

};

#endif // SCREENTRANSTIONSTopbarContainer_HPP
