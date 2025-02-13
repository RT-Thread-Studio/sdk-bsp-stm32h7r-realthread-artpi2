#include <gui/containers/clickableBuildingCustomContainer.hpp>

clickableBuildingCustomContainer::clickableBuildingCustomContainer():
    BuildingContainerClickedCallback(this, &clickableBuildingCustomContainer::BuildingContainerClickHandler)
{

}

void clickableBuildingCustomContainer::initialize()
{
    clickableBuildingCustomContainerBase::initialize();
    entranceClickableContainer.setClickAction(BuildingContainerClickedCallback);
    leftBuildingClickableContainer.setClickAction(BuildingContainerClickedCallback);
    topBuildingClickableContainer.setClickAction(BuildingContainerClickedCallback);
    rightBuildingClickableContainer.setClickAction(BuildingContainerClickedCallback);
    rightBuildingImage.setAlpha(0);
    topBuildingImage.setAlpha(0);
    leftBuildingImage.setAlpha(0);
    entranceImage.setAlpha(0);
}

void clickableBuildingCustomContainer::setZoneLight(touchgfx::FadeAnimator< touchgfx::Image >& (i))
{
    int fadeValue = i.getAlpha() == 255 ? 0 : 255;
    i.startFadeAnimation(fadeValue, BUILDING_LIGHTS_CLICK_FADE_DURATION, EasingEquations::linearEaseOut);
}

void clickableBuildingCustomContainer::setAllLights(bool active)
{
    int fadeValue = active ? 255 : 0;
    entranceImage.startFadeAnimation(fadeValue, BUILDING_LIGHTS_AUTO_FADE_DURATION, EasingEquations::linearEaseOut);
    leftBuildingImage.startFadeAnimation(fadeValue, BUILDING_LIGHTS_AUTO_FADE_DURATION, EasingEquations::linearEaseOut);
    topBuildingImage.startFadeAnimation(fadeValue, BUILDING_LIGHTS_AUTO_FADE_DURATION, EasingEquations::linearEaseOut);
    rightBuildingImage.startFadeAnimation(fadeValue, BUILDING_LIGHTS_AUTO_FADE_DURATION, EasingEquations::linearEaseOut);
}

void clickableBuildingCustomContainer::BuildingContainerClickHandler(const touchgfx::Container& c, const ClickEvent& e)
{
    if (e.getType() == touchgfx::ClickEvent::ClickEventType::PRESSED)
    {
        if (&c == &entranceClickableContainer)
        {
            setZoneLight(entranceImage);
        }
        if (&c == &leftBuildingClickableContainer)
        {
            setZoneLight(leftBuildingImage);
        }
        if (&c == &topBuildingClickableContainer)
        {
            setZoneLight(topBuildingImage);
        }
        if (&c == &rightBuildingClickableContainer)
        {
            setZoneLight(rightBuildingImage);
        }
    }
}
