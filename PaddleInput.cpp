#include "PaddleInput.h"

#include <windows.h>

MorseElement PaddleInput::Poll()
{
    bool hNow =
        (GetAsyncKeyState('H')
            & 0x8000) != 0;

    bool tNow =
        (GetAsyncKeyState('T')
            & 0x8000) != 0;

    if (hNow && !hPressed)
    {
        hPressed = hNow;

        return MorseElement::Dit;
    }

    if (tNow && !tPressed)
    {
        tPressed = tNow;

        return MorseElement::Dah;
    }

    hPressed = hNow;
    tPressed = tNow;

    return MorseElement::None;
}