#pragma once

#include "MorseElement.h"

class UsbInput
{
public:

    MorseElement Poll();

private:

    bool usbDitPressed = false;

    bool usbDahPressed = false;
};