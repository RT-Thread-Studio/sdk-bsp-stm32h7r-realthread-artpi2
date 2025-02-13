

#include "gui/Transitions/Transitions.hpp"
#include "touchgfx/widgets/TextureMapper.hpp"
#include "touchgfx/containers/CacheableContainer.hpp"

class RolloutTransition : public Transitions
{
public:
    RolloutTransition();
    ~RolloutTransition();

    virtual void setupTranstion(Bitmap start, Bitmap end);
    virtual void animationTick(uint16_t step, uint16_t duration);
    virtual uint16_t endTransition();
private:

    const static int NUMBER_OF_TEXTURE_MAPPERS = 8;

    TextureMapper cutoutTextureMappers[NUMBER_OF_TEXTURE_MAPPERS];

    Image startImage;
};
