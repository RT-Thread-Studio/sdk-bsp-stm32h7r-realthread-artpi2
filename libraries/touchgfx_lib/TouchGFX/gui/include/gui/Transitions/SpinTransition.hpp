#include "gui/Transitions/Transitions.hpp"
#include <touchgfx/widgets/TextureMapper.hpp>

class SpinTransition : public Transitions
{
public:
    SpinTransition();
    ~SpinTransition();

    virtual void setupTranstion(Bitmap start, Bitmap end);
    virtual void animationTick(uint16_t step, uint16_t duration);
    virtual uint16_t endTransition();
private:

    TextureMapper endTextureMapper;
    TextureMapper startTextureMapper;
};
