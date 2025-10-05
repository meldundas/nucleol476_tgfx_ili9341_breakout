#ifndef MODEL_HPP
#define MODEL_HPP

#include "custom.h"

class ModelListener;

class Model
{
public:
    Model();

    void bind(ModelListener* listener)
    {
        modelListener = listener;
    }

    Position getJoystick();

    void tick();
protected:
    ModelListener* modelListener;
};

#endif // MODEL_HPP
