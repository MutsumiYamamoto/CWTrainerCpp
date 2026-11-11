#include "IInputDevice.h"


#include <windows.h>

using namespace std::chrono;

SingleInput::SingleInput(
    int threshold)
{
    thresholdMs = threshold;

    spacePressed = false;
}

MorseElement SingleInput::Poll()
{
    bool spaceNow =
        (GetAsyncKeyState(VK_SPACE)
            & 0x8000) != 0;

    if (spaceNow &&
        !spacePressed)
    {
        keyDownTime =
            steady_clock::now();

        spacePressed = true;
    }

    if (!spaceNow &&
        spacePressed)
    {
        auto pressTime =
            duration_cast<milliseconds>(
                steady_clock::now()
                - keyDownTime)
            .count();

        spacePressed = false;

        if (pressTime < thresholdMs)
        {
            return MorseElement::Dit;
        }

        return MorseElement::Dah;
    }

    return MorseElement::None;
}