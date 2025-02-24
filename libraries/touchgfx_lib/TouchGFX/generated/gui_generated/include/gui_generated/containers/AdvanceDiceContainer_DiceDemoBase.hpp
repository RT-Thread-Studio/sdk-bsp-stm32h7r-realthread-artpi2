/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#ifndef ADVANCEDICECONTAINER_DICEDEMOBASE_HPP
#define ADVANCEDICECONTAINER_DICEDEMOBASE_HPP

#include <gui/common/FrontendApplication.hpp>
#include <touchgfx/containers/Container.hpp>

class AdvanceDiceContainer_DiceDemoBase : public touchgfx::Container
{
public:
    AdvanceDiceContainer_DiceDemoBase();
    virtual ~AdvanceDiceContainer_DiceDemoBase();
    virtual void initialize();

    /*
     * Custom Actions
     */
    virtual void left()
    {
        // Override and implement this function in Screen1
    }

    virtual void right()
    {
        // Override and implement this function in Screen1
    }

    virtual void up()
    {
        // Override and implement this function in Screen1
    }

    virtual void down()
    {
        // Override and implement this function in Screen1
    }

    virtual void setScale(float value)
    {
        // Override and implement this function in Screen1
    }


    /*
     * Custom Trigger Callback Setters
     */
    void setAnimationDoneCallback(touchgfx::GenericCallback<>& callback)
    {
        this->animationDoneCallback = &callback;
    }
    void setExitAnimationEndedCallback(touchgfx::GenericCallback<>& callback)
    {
        this->exitAnimationEndedCallback = &callback;
    }
    void setEnterAnimationEndedCallback(touchgfx::GenericCallback<>& callback)
    {
        this->enterAnimationEndedCallback = &callback;
    }

protected:
    FrontendApplication& application() {
        return *static_cast<FrontendApplication*>(touchgfx::Application::getInstance());
    }

    /*
     * Custom Trigger Emitters
     */
    virtual void emitAnimationDoneCallback()
    {
        if (animationDoneCallback && animationDoneCallback->isValid())
        {
            this->animationDoneCallback->execute();
        }
    }
    virtual void emitExitAnimationEndedCallback()
    {
        if (exitAnimationEndedCallback && exitAnimationEndedCallback->isValid())
        {
            this->exitAnimationEndedCallback->execute();
        }
    }
    virtual void emitEnterAnimationEndedCallback()
    {
        if (enterAnimationEndedCallback && enterAnimationEndedCallback->isValid())
        {
            this->enterAnimationEndedCallback->execute();
        }
    }

private:

    /*
     * Custom Trigger Callback Declarations
     */
    touchgfx::GenericCallback<>* animationDoneCallback;
    touchgfx::GenericCallback<>* exitAnimationEndedCallback;
    touchgfx::GenericCallback<>* enterAnimationEndedCallback;

};

#endif // ADVANCEDICECONTAINER_DICEDEMOBASE_HPP
