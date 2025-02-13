#include "gui/Transitions/Transitions.hpp"
#include <touchgfx/widgets/Image.hpp>

class SlideTransition : public Transitions
{
public:
    SlideTransition();
    ~SlideTransition();

    void setupTranstion(Bitmap start, Bitmap end);

    void animationTick(uint16_t step, uint16_t duration);

    uint16_t endTransition();

private:

    Image startImage;
    Image endImage;
};
