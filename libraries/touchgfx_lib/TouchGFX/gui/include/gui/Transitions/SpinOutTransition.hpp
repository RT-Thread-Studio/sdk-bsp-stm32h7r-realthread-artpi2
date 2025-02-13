#include "gui/Transitions/Transitions.hpp"
#include <touchgfx/widgets/Image.hpp>
#include <touchgfx/widgets/TextureMapper.hpp>

class SpinOutTransition : public Transitions
{
public:
    SpinOutTransition();
    ~SpinOutTransition();

    virtual void setupTranstion(Bitmap start, Bitmap end);
    virtual void animationTick(uint16_t step, uint16_t duration);
    virtual uint16_t endTransition();
private:

    Image startImage;
    TextureMapper endTextureMapper;
};
