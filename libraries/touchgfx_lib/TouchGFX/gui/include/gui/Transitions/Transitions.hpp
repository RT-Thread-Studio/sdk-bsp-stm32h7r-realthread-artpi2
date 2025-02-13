#ifndef TRANSITIONS_HPP
#define TRANSITIONS_HPP

#include <touchgfx/widgets/Image.hpp>
#include <gui/common/FrontendApplication.hpp>
#include <touchgfx/containers/Container.hpp>

class Transitions : public touchgfx::Container
{
public:
    Transitions();
    ~Transitions();

    virtual void handleTickEvent();

    void startTransition(Bitmap start, Bitmap end, uint16_t duration);

    void setTransitionIconId(uint16_t iconId);
    uint16_t getTransitionIconId();

    void setTransitionTextId(uint16_t textId);
    uint16_t getTransitionTextId();

    void setTransitionTitleTextId(uint16_t textId);
    uint16_t getTransitionTitleTextId();

    void setTransitionEndedCallback(GenericCallback<uint16_t>& callback);

    virtual void setupTranstion(Bitmap start, Bitmap end) {}
    virtual void animationTick(uint16_t step, uint16_t duration) {}
    virtual uint16_t endTransition()
    {
        return 0;
    }

    enum Transition_Types
    {
        COVER,
        SLIDE,
        CURTAINS,
        ROLLOUT,
        CUBE,
        SPINOUT,
        FLIP,
        SPIN,
        NUMBER_OF_TRANSITIONS
    };

private:

    uint16_t transitionDuration;
    uint16_t stepCounter;

    uint16_t transitionIconId;
    uint16_t transitionTextId;
    uint16_t transitionTitleTextId;

    GenericCallback<uint16_t>* transitionEndedCallback;
};

#endif
