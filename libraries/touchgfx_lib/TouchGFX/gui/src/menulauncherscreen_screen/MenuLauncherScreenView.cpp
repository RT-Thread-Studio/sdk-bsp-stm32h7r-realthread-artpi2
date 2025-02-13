#include <gui/menulauncherscreen_screen/MenuLauncherScreenView.hpp>

MenuLauncherScreenView::MenuLauncherScreenView()
{

}

void MenuLauncherScreenView::setupScreen()
{
    MenuLauncherScreenViewBase::setupScreen();

    HAL::getInstance()->setFrameRateCompensation(true);

    selectedDemoName.setAlpha(0);

    sphere_wheel1.startWheel(presenter->getSelectedDemoNumber());
}

void MenuLauncherScreenView::tearDownScreen()
{
    MenuLauncherScreenViewBase::tearDownScreen();
}

void MenuLauncherScreenView::sphereWheelAnimationStarted()
{
    selectedDemoName.startFadeAnimation(0, 30);
    sphere_wheel1.stopIconAnimation();
}

void MenuLauncherScreenView::sphereWheelAnimationEndded(uint16_t value)
{
    selectedDemoName.setTypedText(touchgfx::TypedText(subDemoNames[value]));
    selectedDemoName.startFadeAnimation(255, 30);

    sphere_wheel1.startIconAnimation();
}

void MenuLauncherScreenView::sphereWheelItemSelected(uint16_t value)
{
    presenter->setSelectedDemoNumber(value);

    switch (value)
    {
    case DEMO_1 :
        goToDemoRousset();
        break;
    case DEMO_2:
        goToDemoTransition();
        break;
    case DEMO_3:
        goToDemoCompass();
        break;
    case DEMO_4:
        goToDemoEbike();
        break;
    case DEMO_5:
        goToInfoScreen();
        break;
    case DEMO_6:
        goToSVGScreen();
        break;
    default:
        break;
    }
}
