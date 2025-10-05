#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>

#include <stdint.h>
#include "custom.h"

extern "C"
{
extern Position joystickPosition;
}

Position Model::getJoystick()
{
#ifndef SIMULATOR
	return joystickPosition;
#else
	return 0;
#endif
}

Model::Model() : modelListener(0	)
{

}

void Model::tick()
{
	if(modelListener!=0)
	{
		modelListener->newJoystickValue(getJoystick());
	}
}
