#ifndef MODEL_HPP
#define MODEL_HPP

class ModelListener;

class Model
{
public:
    Model();

    void bind(ModelListener* listener)
    {
        modelListener = listener;
    }

    void setSelectedDemoNumber(int value);
    int getSelectedDemoNumber();

    void tick();
protected:
    ModelListener* modelListener;

    int selectedDemoNumber = 0;
};

#endif // MODEL_HPP
