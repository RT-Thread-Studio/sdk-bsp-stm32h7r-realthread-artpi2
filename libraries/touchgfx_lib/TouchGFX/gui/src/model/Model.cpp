#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>

Model::Model() : modelListener(0)
{

}

void Model::tick()
{

}

void Model::setSelectedDemoNumber(int value)
{
    selectedDemoNumber = value;
}

int Model::getSelectedDemoNumber()
{
    return selectedDemoNumber;
}
