#include "gui/Transitions/Transitions.hpp"
#include <touchgfx/widgets/Image.hpp>
#include <touchgfx/containers/Container.hpp>

class CurtainsTransition : public Transitions
{
public:
    CurtainsTransition();
    ~CurtainsTransition();

    void setupTranstion(Bitmap start, Bitmap end);

    void animationTick(uint16_t step, uint16_t duration);

    uint16_t endTransition();

private:

    Image startImage;
    Image endImageLeft;
    Image endImageRight;

    Container containerLeft;
    Container containerRight;
};
