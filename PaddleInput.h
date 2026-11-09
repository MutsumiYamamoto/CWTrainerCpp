#pragma once

#include "MorseElement.h"
#include "IInputDevice.h"

class PaddleInput : public IInputDevice
{
public:

	MorseElement Poll() override;

private:

    bool hPressed = false;

    bool tPressed = false;
};
