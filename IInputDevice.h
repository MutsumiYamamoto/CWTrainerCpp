#pragma once

#include "MorseElement.h"

class IInputDevice
{
public:

    virtual ~IInputDevice() = default;

    virtual MorseElement Poll() = 0;
};