#ifndef WAFFLEMENUCONTAINER_HPP
#define WAFFLEMENUCONTAINER_HPP

#include <gui_generated/containers/WaffleMenuContainerBase.hpp>

class WaffleMenuContainer : public WaffleMenuContainerBase
{
public:
    WaffleMenuContainer();
    virtual ~WaffleMenuContainer() {}

    virtual void initialize();

    enum SizeOfWaffle
    {
        BIG, MEDIUM, SMALL
    };
    SizeOfWaffle getSizeOfTheChosenWaffle();

    void setupTheMenu();

    void shakeTheBigWaffleIcon();

protected:
    virtual void changeStateOfMenuTo(SlideMenu::State state);
    virtual void reactAccordinglyToTheNewStateOfMenu();

    virtual void bigWaffleIsChosen();
    virtual void mediumWaffleIsChosen();
    virtual void smallWaffleIsChosen();

private:
    void setWaffleLabelVisibilityTo(bool visibility);
    void setMenuClickListenerActivityTo(bool activity);

    SizeOfWaffle chosenWaffle;
};

#endif // WAFFLEMENUCONTAINER_HPP
