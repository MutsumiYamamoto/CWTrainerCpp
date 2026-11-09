#pragma once

#include "MorseElement.h"
#include "IInputDevice.h"

class UsbInput : public IInputDevice
{
public:

	MorseElement Poll() override;

private:

    bool usbDitPressed = false;

    bool usbDahPressed = false;
};