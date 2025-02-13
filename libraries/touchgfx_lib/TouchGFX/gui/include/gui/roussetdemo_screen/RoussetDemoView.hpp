#ifndef ROUSSETDEMOVIEW_HPP
#define ROUSSETDEMOVIEW_HPP

#include <gui_generated/roussetdemo_screen/RoussetDemoViewBase.hpp>
#include <gui/roussetdemo_screen/RoussetDemoPresenter.hpp>

class RoussetDemoView : public RoussetDemoViewBase
{
public:
    RoussetDemoView();
    virtual ~RoussetDemoView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    //Declaration of needed Handlers
    virtual void videoClickHandler();
    virtual void coverflowClickHandler();
    virtual void pineconeClickHandler();
    virtual void carClickHandler();
    virtual void groundhogClickHandler();
    virtual void videoContainerMoveAnimationEndedHandler(const touchgfx::MoveAnimator<Container>& comp);

    //Declaration of the handleTickEvent
    virtual void handleTickEvent();

    //Declaration of functions used in Designer under "Interactions" section.
protected:


private:

    const uint16_t SUN_MOON_MOVE_DURATION = 35; //ticks
    const uint16_t DAY_NIGHT_FADE_DURATION = 100; //ticks

    enum statemachineStep_enum
    {
        startupAnimationInit_enum,
        startupAnimation_enum,
        startupAnimationEnd_enum,
        introScaleOutAnimationInit_enum,
        introScaleOutAnimation_enum,
        introScaleOutAnimationEnd_enum,
        truckBackwardAnimationInit_enum,
        truckBackwardAnimation_enum,
        truckBackwardAnimationEnd_enum,
        mcuAnimationInit_enum,
        mcuAnimation_enum,
        mcuAnimationEnd_enum,
        truckOnwardAnimationInit_enum,
        truckOnwardAnimation_enum,
        truckOnwardAnimationEnd_enum,
        nightAnimationInit_enum,
        nightAnimationFadeClouds_enum,
        nightAnimation_enum,
        nightAnimationEnd_enum,
        dayAnimationInit_enum,
        dayAnimation_enum,
        dayAnimationEnd_enum,

        waitAnimation_enum,

        statemachineStepEnd_enum
    };

    //Enums to keep tracking in which state the program is curently running.
    statemachineStep_enum animationStep;
    statemachineStep_enum nextAnimationStep;

    //Variable to count how many MCUs been moved inside the truck
    int mcuProductionCount;
    float mcuAnimationCounter;

    //Used for counting the total number of ticks
    uint32_t tickCount;

    //Variable to produce MCUs during night
    bool nightWork;

    Callback <RoussetDemoView, const touchgfx::MoveAnimator<Container>&> videoContainerMoveAnimationEndedCallback;

    //control the run of the application
    void stateMachine();

    /*
    All functions used by the state machine
    all states comes with 3 functions : initialisation, treatment and end function.
    Init function : used to make declaration, affectation, prepare the display and animations to be run
    Treatment function : used to run animation
    End function : destroy useless things, put to "normal" state everything that needs to.
    */
    void startupAnimationInit();
    void startupAnimation();
    void startupAnimationEnd();
    void introScaleOutAnimationInit();
    void introScaleOutAnimation();
    void introScaleOutAnimationEnd();
    void truckBackwardAnimationInit();
    void truckBackwardAnimation();
    void truckBackwardAnimationEnd();
    void truckOnwardAnimationInit();
    void truckOnwardAnimation();
    void truckOnwardAnimationEnd();
    void mcuAnimationInit();
    void mcuAnimation();
    void mcuAnimationEnd();
    void nightAnimationInit();
    void nightAnimationFadeClouds();
    void nightAnimation();
    void nightAnimationEnd();
    void dayAnimationInit();
    void dayAnimation();
    void dayAnimationEnd();
    void waitAnimation();

    void changeState(statemachineStep_enum nextState);

    void updateMcuLoad();
    void updateFps();
    void closeLargeVideo();
    bool isLargeVideoActive();
    bool isCoverFlowActive();
    void fadeInSmallVideo();

    uint16_t previousMcuLoadPct;
    uint16_t frameSkippedCounter;
    uint16_t frames;
    uint16_t fps;
};

#endif // ROUSSETDEMOVIEW_HPP
