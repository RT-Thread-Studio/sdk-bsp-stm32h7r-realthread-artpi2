#ifndef CLICKABLEBUILDINGCUSTOMCONTAINER_HPP
#define CLICKABLEBUILDINGCUSTOMCONTAINER_HPP

#include <gui_generated/containers/clickableBuildingCustomContainerBase.hpp>

class clickableBuildingCustomContainer : public clickableBuildingCustomContainerBase
{
public:
    clickableBuildingCustomContainer();
    virtual ~clickableBuildingCustomContainer() {}

    virtual void setZoneLight(touchgfx::FadeAnimator< touchgfx::Image >& (i));
    virtual void setAllLights(bool active);

    virtual void BuildingContainerClickHandler(const touchgfx::Container& c, const ClickEvent& e);


    virtual void initialize();
protected:

private:
    static constexpr int BUILDING_LIGHTS_CLICK_FADE_DURATION = 20; // in ticks
    static constexpr int BUILDING_LIGHTS_AUTO_FADE_DURATION = 20; // in ticks

    Callback <clickableBuildingCustomContainer, const touchgfx::Container&, const touchgfx::ClickEvent&> BuildingContainerClickedCallback;

};

#endif // CLICKABLEBUILDINGCUSTOMCONTAINER_HPP
