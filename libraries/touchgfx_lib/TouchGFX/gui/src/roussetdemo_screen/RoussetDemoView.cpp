#include <gui/roussetdemo_screen/RoussetDemoView.hpp>
#include <math.h>
#include <BitmapDatabase.hpp>
#include <videos/VideoDatabase.hpp>

// Initilisation of all variables in the class
RoussetDemoView::RoussetDemoView() :
    animationStep(startupAnimationInit_enum),
    nextAnimationStep(startupAnimationInit_enum),
    mcuProductionCount(0),
    mcuAnimationCounter(0),
    tickCount(0),
    nightWork(false),
    videoContainerMoveAnimationEndedCallback(this, &RoussetDemoView::videoContainerMoveAnimationEndedHandler),
    previousMcuLoadPct(0),
    frameSkippedCounter(0),
    frames(0),
    fps(0)
{

}

// Associating widgets with all needed callbacks
void RoussetDemoView::setupScreen()
{
    RoussetDemoViewBase::setupScreen();
    videoWindowContainer.setMoveAnimationEndedAction(videoContainerMoveAnimationEndedCallback);
    introAnimationContainer.setVisible(true);

    HAL::getInstance()->setFrameRateCompensation(true);
}

void RoussetDemoView::tearDownScreen()
{
    RoussetDemoViewBase::tearDownScreen();
}

/*
*******************STARTUP ANIMATION*******************
Prepare the first screen to be shown. Depends on the screen designed with TouchGFX Designer.
The main animation is the fade animation
*/
void RoussetDemoView::startupAnimationInit()
{
    introAnimationContainer.setVisible(true);
    contentContainer.setVisible(false);

    foregroundTreesTextureMapper.setScale(1.6f);
    grassTextureMapper.setScale(1.35f);
    stFactoryTextureMapper.setScale(1.3f);
    hillsTextureMapper.setScale(1.40f);

    invalidate();

    fadeBox.startFadeAnimation(0, 60, EasingEquations::linearEaseNone);

    changeState(startupAnimation_enum);
}

void RoussetDemoView::startupAnimation()
{
    if (!fadeBox.isFadeAnimationRunning())
    {
        changeState(startupAnimationEnd_enum);
    }
}

void RoussetDemoView::startupAnimationEnd()
{
    fadeBox.setVisible(false);
    changeState(introScaleOutAnimationInit_enum);
}
/**************************************/

/*
********************SCALE OUT********************
The idea with this step is to scale out multiple elements to make it look like moving the camera back.
Mostly using Texture Mappers and scale animation.
*/
void RoussetDemoView::introScaleOutAnimationInit()
{
    foregroundTreesTextureMapper.setupAnimation(AnimationTextureMapper::SCALE, 1.0f, 110, 0, EasingEquations::quadEaseOut);
    foregroundTreesTextureMapper.startAnimation();

    grassTextureMapper.setupAnimation(AnimationTextureMapper::SCALE, 1.0f, 110, 0, EasingEquations::quartEaseOut);
    grassTextureMapper.startAnimation();

    stFactoryTextureMapper.setupAnimation(AnimationTextureMapper::SCALE, 1.0f, 110, 0, EasingEquations::quartEaseOut);
    stFactoryTextureMapper.startAnimation();

    hillsTextureMapper.setupAnimation(AnimationTextureMapper::SCALE, 1.0f, 110, 0, EasingEquations::quartEaseOut);
    hillsTextureMapper.startAnimation();

    changeState(introScaleOutAnimation_enum);
}

void RoussetDemoView::introScaleOutAnimation()
{
    // wait for the animations to be finished, before changing to the next state
    if (!foregroundTreesTextureMapper.isTextureMapperAnimationRunning() &&
            !grassTextureMapper.isTextureMapperAnimationRunning() &&
            !stFactoryTextureMapper.isTextureMapperAnimationRunning() &&
            !hillsTextureMapper.isTextureMapperAnimationRunning())
    {
        changeState(introScaleOutAnimationEnd_enum);
    }
}

void RoussetDemoView::introScaleOutAnimationEnd()
{
    // Start showing the normal scenary instead of the widgets used for the intro and start the video
    introAnimationContainer.setVisible(false);
    contentContainer.setVisible(true);
    truckCustomContainer.setVisible(false);
    invalidate();

    cloudsCustomContainer.repositionClouds();

    fadeInSmallVideo();

    changeState(truckBackwardAnimationInit_enum);
}
/**************************************/

/*
********************TRUCK BACKWARD ANIMATION*********************
The main animation is a move animation
*/
void RoussetDemoView::truckBackwardAnimationInit()
{
    // Setup the truck to move from outside the screen to the factory
    truckCustomContainer.setX(-truckCustomContainer.getWidth());
    truckCustomContainer.setNewColor(nightWork);
    truckCustomContainer.setLights(nightWork);
    truckCustomContainer.setVisible(true);
    truckCustomContainer.startMoveAnimation(127, truckCustomContainer.getY(), 350, EasingEquations::backEaseOut);

    // Control clouds and which foreground tree overlay to use based on whether it is day or night
    if (!nightWork)
    {
        cloudsCustomContainer.setVisible(true);
        cloudsCustomContainer.fadeIn();
        cloudsCustomContainer.moveClouds();
        treeOverlayImage.setBitmap(BITMAP_FOREGROUND_SMALL_AREA_ID);
    }
    else
    {
        treeOverlayImage.setBitmap(BITMAP_FOREGROUND_SMALL_AREA_NIGHT3_ID);
    }
    treeOverlayImage.setVisible(true);

    changeState(truckBackwardAnimation_enum);
}

void RoussetDemoView::truckBackwardAnimation()
{
    // Wait for the truck move animation to complete
    if (!truckCustomContainer.isMoveAnimationRunning())
    {
        changeState(truckBackwardAnimationEnd_enum);
    }
}

void RoussetDemoView::truckBackwardAnimationEnd()
{
    truckCustomContainer.setLights(false);
    mcuProductionCount = 0;

    changeState(mcuAnimationInit_enum);
}
/**************************************/

/*
********************MCU ANIMATION*********************
This animation is split in 4 :
1- the mcu zooms in and have a first half circle movement
2- the mcu zooms out and have an other half circle movement
3- the mcu continues to zoom out and goes straight to the truck
4- the truck stretches for few frames
We pass by this function 3 times because we want to see 3 MCU produced and moved to the truck
Exit condition : truck stretech animation ends on the last (3rd) MCU.
*/
void RoussetDemoView::mcuAnimationInit()
{
    mcuAnimationCounter = 0;

    changeState(mcuAnimation_enum);
}

void RoussetDemoView::mcuAnimation()
{
    const uint32_t SECTION_DURATION = 50; //duration of each of the three MCU move sections in ticks
    const float MCU_SCALE_MAX = 0.9f;
    const uint32_t MCU_ANIMATION_CIRCLE_X = 280 - 42;
    const uint32_t MCU_ANIMATION_CIRCLE_Y = 185 - 42;
    const uint32_t MCU_ANIMATION_RADIUS = 68;

    mcuTextureMapper.invalidateContent(); //invalidate the MCU texture mapper before manipulating it
    mcuTextureMapper.setZAngle(mcuTextureMapper.getZAngle() + 0.04f); //make the MCU spin around its Z axis

    if (mcuAnimationCounter == 0)
    {
        // Start chimney animation and set the MCU texturemapper to its starting position (at the chimney)
        chimneyCustomContainer1.startAnimation(nightWork);
        mcuTextureMapper.setX(MCU_ANIMATION_CIRCLE_X - MCU_ANIMATION_RADIUS);
        mcuTextureMapper.setY(MCU_ANIMATION_CIRCLE_Y);
        mcuTextureMapper.setVisible(true);
        mcuTextureMapper.setScale(0.0f);
    }
    else if (mcuAnimationCounter < 1 * SECTION_DURATION)
    {
        // Move the MCU in the first quarter circle while scaling it up
        const float angle = (float)mcuAnimationCounter * 3.14f / 100;
        mcuTextureMapper.setX(MCU_ANIMATION_CIRCLE_X - (int16_t)(cos(angle) * MCU_ANIMATION_RADIUS));
        mcuTextureMapper.setY(MCU_ANIMATION_CIRCLE_Y - (int16_t)(sin(angle) * MCU_ANIMATION_RADIUS));
        mcuTextureMapper.setScale(mcuTextureMapper.getScale() + MCU_SCALE_MAX / SECTION_DURATION);
    }
    else if (mcuAnimationCounter < 2 * SECTION_DURATION)
    {
        // Move the MCU in the second quarter circle while scaling it halfway down
        const float angle = (float)mcuAnimationCounter * 3.14f / 100;
        mcuTextureMapper.setX(MCU_ANIMATION_CIRCLE_X - (int16_t)(cos(angle) * MCU_ANIMATION_RADIUS));
        mcuTextureMapper.setY(MCU_ANIMATION_CIRCLE_Y - (int16_t)(sin(angle) * MCU_ANIMATION_RADIUS));
        mcuTextureMapper.setScale(mcuTextureMapper.getScale() - MCU_SCALE_MAX / 2 / SECTION_DURATION);
    }
    else if (mcuAnimationCounter < 3 * SECTION_DURATION)
    {
        // Move the MCU vertically down to the while scaling it down
        mcuTextureMapper.setY(mcuTextureMapper.getY() + 2);
        mcuTextureMapper.setScale(mcuTextureMapper.getScale() - MCU_SCALE_MAX / 2 / SECTION_DURATION);
    }
    else if (mcuAnimationCounter == 3 * SECTION_DURATION)
    {
        // Hide the MCU texturemapper and start the truck stretch animation
        mcuTextureMapper.setVisible(false);
        truckCustomContainer.startAnimation();
    }
    else
    {
        // Wait for the truck stretch animation to finish
        if (truckCustomContainer.isAnimationFinished())
        {
            mcuProductionCount++;
            // Restart the MCU animation until it reaches the desired count, then end the MCU animation state
            if (mcuProductionCount < 3)
            {
                changeState(mcuAnimationInit_enum);
            }
            else
            {
                changeState(mcuAnimationEnd_enum);
            }
        }
    }
    mcuTextureMapper.invalidateContent(); //invalidate the MCU texture mapper after manipulating it
    mcuAnimationCounter++;
}

void RoussetDemoView::mcuAnimationEnd()
{
    changeState(truckOnwardAnimationInit_enum);
}
/**************************************/

/*
********************TRUCK ONWARD ANIMATION*********************
The main animation is a move animation
*/
void RoussetDemoView::truckOnwardAnimationInit()
{
    //turn on the truck lights, if it is night and make the truck drive out of the screen
    truckCustomContainer.setLights(nightWork);
    truckCustomContainer.startMoveAnimation(-truckCustomContainer.getWidth(), truckCustomContainer.getY(), 350, EasingEquations::backEaseIn);
    changeState(truckOnwardAnimation_enum);
}

void RoussetDemoView::truckOnwardAnimation()
{
    // Wait for the truck to stop moving
    if (!truckCustomContainer.isMoveAnimationRunning())
    {
        changeState(truckOnwardAnimationEnd_enum);
    }
}

void RoussetDemoView::truckOnwardAnimationEnd()
{
    // Hide the small image which was used to make the truck drive behind the foreground trees
    treeOverlayImage.setVisible(false);

    // Switch between day and night
    if (nightWork)
    {
        changeState(dayAnimationInit_enum);
        nightWork = false;
    }
    else
    {
        changeState(nightAnimationInit_enum);
        nightWork = true;
    }
}
/**************************************/

/*
********************NIGHT ANIMATION*********************
The step includes all the animation during night : hide sun & clouds, show moon, show stars etc..
*/
void RoussetDemoView::nightAnimationInit()
{
    // Start the day to night transistion by fading away the clouds
    cloudsCustomContainer.fadeOut();
    changeState(nightAnimationFadeClouds_enum);
}

void RoussetDemoView::nightAnimationFadeClouds()
{
    // Wait for the clouds fade to complete, then start the rest of the transition
    if (!cloudsCustomContainer.isFadeInProgress())
    {
        nightSkyContainer.setVisible(true);
        nightSkyContainer.invalidate();

        // Remove the clouds
        cloudsCustomContainer.setVisible(false);
        cloudsCustomContainer.stopClouds();

        // Fade the night landscape on top of the day landscape
        backgroundNightImage.setVisible(true);
        backgroundNightImage.setAlpha(0);
        backgroundNightImage.startFadeAnimation(255, DAY_NIGHT_FADE_DURATION, EasingEquations::linearEaseOut);

        // Make the sun set
        sunTextureMapper.setMoveAnimationDelay(0);
        sunTextureMapper.startMoveAnimation(sunTextureMapper.getX(), daySkyContainer.getHeight(), SUN_MOON_MOVE_DURATION, EasingEquations::backEaseIn, EasingEquations::backEaseIn);

        // Make the moon rise
        moonImage.setAlpha(255);
        moonImage.setY(nightSkyContainer.getHeight());
        moonImage.setMoveAnimationDelay(SUN_MOON_MOVE_DURATION);
        moonImage.startMoveAnimation(moonImage.getX(), 15, SUN_MOON_MOVE_DURATION, EasingEquations::backEaseOut, EasingEquations::backEaseOut);

        // Fade the night sky on top of the day sky
        nightSkyImage.setAlpha(0);
        nightSkyImage.startFadeAnimation(255, DAY_NIGHT_FADE_DURATION, EasingEquations::linearEaseOut);

        // Start the star animations
        starCustomContainer1.showStars();

        changeState(nightAnimation_enum);
    }
}

void RoussetDemoView::nightAnimation()
{
    // Wait for the transistion to finish
    if (!backgroundNightImage.isFadeAnimationRunning())
    {
        changeState(nightAnimationEnd_enum);
    }
}

void RoussetDemoView::nightAnimationEnd()
{
    // Hide day landscape and sky and turn on the building lights
    daySkyContainer.setVisible(false);
    backgroundDayImage.setVisible(false);
    clickableBuildingCustomContainer1.setAllLights(true);
    contentContainer.invalidate();

    changeState(truckBackwardAnimationInit_enum);
}
/**************************************/

/*
********************DAY ANIMATION*********************
In this state we put back the sun & clouds, remove stars, moon, and the night images
*/
void RoussetDemoView::dayAnimationInit()
{
    daySkyContainer.setVisible(true);

    cloudsCustomContainer.repositionClouds();
    cloudsCustomContainer.moveClouds();

    // Fade out the night sky, to make the day sky visible again
    nightSkyImage.startFadeAnimation(0, DAY_NIGHT_FADE_DURATION, EasingEquations::linearEaseOut);
    starCustomContainer1.hideStars();

    // Move down the moon and move up the sun
    moonImage.setMoveAnimationDelay(0);
    moonImage.startMoveAnimation(moonImage.getX(), nightSkyContainer.getHeight(), SUN_MOON_MOVE_DURATION, EasingEquations::backEaseIn, EasingEquations::backEaseIn);
    sunTextureMapper.setMoveAnimationDelay(SUN_MOON_MOVE_DURATION);
    sunTextureMapper.startMoveAnimation(sunTextureMapper.getX(), -2, SUN_MOON_MOVE_DURATION, EasingEquations::backEaseOut, EasingEquations::backEaseOut);

    // Fade out the night landscape, to make the day landscape visible again
    backgroundDayImage.setVisible(true);
    backgroundNightImage.startFadeAnimation(0, DAY_NIGHT_FADE_DURATION, EasingEquations::linearEaseNone);

    contentContainer.invalidate();

    changeState(dayAnimation_enum);
}

void RoussetDemoView::dayAnimation()
{
    // Wait for the transistion to finish
    if (!backgroundNightImage.isFadeAnimationRunning())
    {
        changeState(dayAnimationEnd_enum);
    }
}

void RoussetDemoView::dayAnimationEnd()
{
    clickableBuildingCustomContainer1.setAllLights(false);

    // Set the night images not visible
    backgroundNightImage.setVisible(false);
    nightSkyContainer.setVisible(false);
    contentContainer.invalidate();

    changeState(truckBackwardAnimationInit_enum);
}
/**************************************/

void RoussetDemoView::waitAnimation()
{
    // When a state change is done, the statemachine waits in this state if the large video or the coverflow is active
    // This will ensure that the "background" stays in the same mode when any of these windows are active
    if (!isLargeVideoActive() && !isCoverFlowActive())
    {
        animationStep = nextAnimationStep;
    }
}

void RoussetDemoView::updateMcuLoad()
{
    // This section uses the HAL layer to detect the MCU load and if it is changed, the topBar is updated
    const uint16_t mcuLoadPct = touchgfx::HAL::getInstance()->getMCULoadPct();

    // Only update the displayed number when it is different than the previous one
    if (mcuLoadPct != previousMcuLoadPct)
    {
        topBarContainer1.updateMCU(mcuLoadPct);
        previousMcuLoadPct = mcuLoadPct;
    }
}

void RoussetDemoView::updateFps()
{
    // This section uses the HAL layer to detect the LCD Refresh count and calculates the FPS if it is changed, the topBar is updated
    if (HAL::getInstance()->getLCDRefreshCount() > 1)
    {
        frameSkippedCounter++;
    }
    else
    {
        frames++;
    }

    if (frames + frameSkippedCounter >= 60)
    {
        if (fps != frames)
        {
            fps = frames;
            topBarContainer1.updateFPS(fps);
        }
        frameSkippedCounter = 0;
        frames = 0;
    }
}

void RoussetDemoView::closeLargeVideo()
{
    // Move the video window out of the visible screen area
    videoWindowContainer.startMoveAnimation(getScreenWidth(), videoWindowContainer.getY(), 30, EasingEquations::quadEaseOut);
}

bool RoussetDemoView::isLargeVideoActive()
{
    // return true if video contatiner is in the visible screen area
    return videoWindowContainer.getX() < getScreenWidth();
}

bool RoussetDemoView::isCoverFlowActive()
{
    return rotatingCoverFlowCustomContainer1.isVisible();
}

void RoussetDemoView::fadeInSmallVideo()
{
    // put a black box on top of the small video and gradually fade it away
    videoFadeBox.setAlpha(255);
    videoFadeBox.startFadeAnimation(0, 30);
}

/*
********************STATE MACHINE*********************
Control the execution of the state machine logic
All the states functions are called here. Do not call states functions inside themselves.
*/
void RoussetDemoView::stateMachine()
{
    //touchgfx_printf("stateMachine %i  \n",animationStep);

    switch (animationStep)
    {
    case startupAnimationInit_enum :
        startupAnimationInit();
        break;

    case startupAnimation_enum :
        startupAnimation();
        break;

    case startupAnimationEnd_enum :
        startupAnimationEnd();
        break;

    case introScaleOutAnimationInit_enum :
        introScaleOutAnimationInit();
        break;

    case introScaleOutAnimation_enum :
        introScaleOutAnimation();
        break;

    case introScaleOutAnimationEnd_enum :
        introScaleOutAnimationEnd();
        break;

    case truckBackwardAnimationInit_enum :
        truckBackwardAnimationInit();
        break;

    case truckBackwardAnimation_enum :
        truckBackwardAnimation();
        break;

    case truckBackwardAnimationEnd_enum :
        truckBackwardAnimationEnd();
        break;

    case mcuAnimationInit_enum :
        mcuAnimationInit();
        break;

    case mcuAnimation_enum :
        mcuAnimation();
        break;

    case mcuAnimationEnd_enum :
        mcuAnimationEnd();
        break;

    case truckOnwardAnimationInit_enum :
        truckOnwardAnimationInit();
        break;

    case truckOnwardAnimation_enum :
        truckOnwardAnimation();
        break;

    case truckOnwardAnimationEnd_enum :
        truckOnwardAnimationEnd();
        break;

    case nightAnimationInit_enum :
        nightAnimationInit();
        break;

    case nightAnimationFadeClouds_enum :
        nightAnimationFadeClouds();
        break;

    case nightAnimation_enum :
        nightAnimation();
        break;

    case nightAnimationEnd_enum :
        nightAnimationEnd();
        break;

    case dayAnimationInit_enum :
        dayAnimationInit();
        break;

    case dayAnimation_enum :
        dayAnimation();
        break;

    case dayAnimationEnd_enum :
        dayAnimationEnd();
        break;

    case waitAnimation_enum :
        waitAnimation();
        break;

    default:
        break;
    }
}

void RoussetDemoView::changeState(statemachineStep_enum nextState)
{
    // When a state change is requested, this function will store the requested state and put change the state to "wait",
    // where the actual state change will happen when the conditions are met
    nextAnimationStep = nextState;
    animationStep = waitAnimation_enum;
}
/**************************************/

/****************   HANDLERS    *************************/
void RoussetDemoView::videoClickHandler()
{
    // Activate the large video window, if it self or the coverflow are not already active
    if (!isLargeVideoActive() && !isCoverFlowActive())
    {
        // Store the current frame number of the video
        const uint32_t currentFrameNumber = factoryVideo.getCurrentFrameNumber();

        // this demo only uses one video widget, which is here moved from the small canvas to the large video window and changed to the larger video stream
        smallVideoContainer.remove(factoryVideo);
        largeVideoContainer.add(factoryVideo);
        factoryVideo.setVideoData(video_mediumRoussetFactoryDemo_bin_start, video_mediumRoussetFactoryDemo_bin_length);
        factoryVideo.expand();

        // Continue play the video from the frame it was at when clicked
        factoryVideo.seek(currentFrameNumber);
        factoryVideo.play();

        // Animate the large video to move to the middle of the screen
        videoWindowContainer.startMoveAnimation(63, videoWindowContainer.getY(), 30, EasingEquations::quadEaseOut);
    }
}

void RoussetDemoView::coverflowClickHandler()
{
    // Activate the coverflow window, if the large video or it self are not already active
    if (!isLargeVideoActive() && !isCoverFlowActive())
    {
        rotatingCoverFlowCustomContainer1.open();
    }
}

void RoussetDemoView::carClickHandler()
{
    // Toggle between show the car image with or without its lights on
    if (carImage.getBitmapId() == BITMAP_CAR_LIGHTSOFF_ID)
    {
        carImage.setBitmap(BITMAP_CAR_LIGHTSON_ID);
    }
    else
    {
        carImage.setBitmap(BITMAP_CAR_LIGHTSOFF_ID);
    }
    carImage.invalidate();
}

void RoussetDemoView::pineconeClickHandler()
{
    // Constants used for the animations
    const int X_ON_TREE = 66;
    const int Y_ON_TREE = 282;
    const float ANGLE_ON_TREE = -2.5f;
    const int X_ON_GROUND = 200;
    const int Y_ON_GROUND = 370;
    const float ANGLE_ON_GROUND = 0.0f;
    const int X_OUTSIDE_SCREEN = -pineconeTextureMapper.getWidth();
    const int Y_OUTSIDE_SCREEN = Y_ON_GROUND;
    const float ANGLE_OUTSIDE_SCREEN = -6.0f;
    const uint16_t DURATION_OF_FALL = 90;
    const uint16_t DELAY_BEFORE_ROTATION = 60;
    const uint16_t DURATION_OF_ROLL = 60;

    // Only accpet cliks when it is not already animating
    if (!pineconeTextureMapper.isMoveAnimationRunning())
    {
        if (pineconeTextureMapper.getX() < 0)
        {
            //Pine cone is outside screen, place it on the tree
            pineconeTextureMapper.setXY(X_ON_TREE, Y_ON_TREE);
            pineconeTextureMapper.setAngles(0, 0, ANGLE_ON_TREE);
            //... and drop it on the ground
            pineconeTextureMapper.startMoveAnimation(X_ON_GROUND, Y_ON_GROUND, DURATION_OF_FALL, EasingEquations::quadEaseOut, EasingEquations::bounceEaseOut);
            pineconeTextureMapper.setupAnimation(AnimationTextureMapper::Z_ROTATION, ANGLE_ON_GROUND, DELAY_BEFORE_ROTATION, DURATION_OF_FALL - DELAY_BEFORE_ROTATION, EasingEquations::sineEaseOut);
            pineconeTextureMapper.startAnimation();
        }
        else
        {
            //Pine cone is on the grass, roll it out of the screen
            pineconeTextureMapper.startMoveAnimation(X_OUTSIDE_SCREEN, Y_OUTSIDE_SCREEN, DURATION_OF_ROLL, EasingEquations::sineEaseOut);
            pineconeTextureMapper.setupAnimation(AnimationTextureMapper::Z_ROTATION, ANGLE_OUTSIDE_SCREEN, DURATION_OF_ROLL, 0, EasingEquations::sineEaseOut);
            pineconeTextureMapper.startAnimation();
        }
    }
}

void RoussetDemoView::groundhogClickHandler()
{
    // Activate the ground hog change it between up and down
    groundhogCustomContainer.toggle();
}

void RoussetDemoView::videoContainerMoveAnimationEndedHandler(const touchgfx::MoveAnimator<Container>& comp)
{
    // Check if the large video window just finished moving outside screen and in that case, reactivate the small video
    if (videoWindowContainer.getX() >= getScreenWidth())
    {
        // Store the current frame number of the video
        const uint32_t currentFrameNumber = factoryVideo.getCurrentFrameNumber();

        // this demo only uses one video widget, which is here moved from the small canvas to the large video window and changed to the larger video stream
        largeVideoContainer.remove(factoryVideo);
        smallVideoContainer.insert(0, factoryVideo); // insert the video widget behind the fade box
        factoryVideo.setVideoData(video_smallRoussetFactoryDemo_bin_start, video_smallRoussetFactoryDemo_bin_length);
        factoryVideo.expand();

        // Continue play the video from the frame it was at when clicked
        factoryVideo.seek(currentFrameNumber);
        factoryVideo.play();

        fadeInSmallVideo();
    }
}

/****************   Handle Tick    *************************/
void RoussetDemoView::handleTickEvent()
{
    // Call the state machine every tick
    stateMachine();

    tickCount++;

    // Show hand animation periodically
    if (tickCount % (10 * 60) == 0)
    {
        handCustomContainer1.activate();
    }

    // Rotate the sun
    sunTextureMapper.setZAngle(sunTextureMapper.getZAngle() + 0.002f);
    sunTextureMapper.invalidateContent();

    // Update the data in the top bar
    updateMcuLoad();
    updateFps();

    // Forward tick custom container which uses it to make their own animations
    cloudsCustomContainer.handleTick();
    handCustomContainer1.tick();
    starCustomContainer1.handleTick();
    truckCustomContainer.animationTick();
    chimneyCustomContainer1.tick();
}
