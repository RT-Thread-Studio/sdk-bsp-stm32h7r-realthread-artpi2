#ifndef TRANSITIONITEM_HPP
#define TRANSITIONITEM_HPP

#include <gui_generated/containers/transitionItemBase.hpp>

class transitionItem : public transitionItemBase
{
public:
    transitionItem();
    virtual ~transitionItem() {}

    virtual void initialize();

    void setIcon(uint16_t iconId);
    void setName(uint16_t textId);
    void fade(uint8_t endAlpha, uint16_t duration, EasingEquation alphaProgressionEquation = &EasingEquations::linearEaseNone);

protected:
};

#endif // TRANSITIONITEM_HPP
