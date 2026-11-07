#pragma once

#include <chrono>

#include "MorseElement.h"

class SingleInput
{
public:

    explicit SingleInput(
        int thresholdMs);

    MorseElement Poll();

private:

    int thresholdMs;

    bool spacePressed;

    std::chrono::steady_clock::
        time_point keyDownTime;
};