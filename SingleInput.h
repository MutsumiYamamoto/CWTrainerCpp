#pragma once

#include <chrono>

#include "MorseElement.h"
#include "IInputDevice.h"

class SingleInput : public IInputDevice
{
public:

    explicit SingleInput(
        int thresholdMs);

	MorseElement Poll() override;

private:

    int thresholdMs;

    bool spacePressed;

    std::chrono::steady_clock::
        time_point keyDownTime;
};