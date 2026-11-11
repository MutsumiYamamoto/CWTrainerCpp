#include "IInputDevice.h"


#include <windows.h>

MorseElement UsbInput::Poll()
{
    bool ditNow =
        (GetAsyncKeyState(VK_LCONTROL)
            & 0x8000) != 0;

    bool dahNow =
        (GetAsyncKeyState('A')
            & 0x8000) != 0;

    if (ditNow &&
        !usbDitPressed)
    {
        usbDitPressed = ditNow;

        return MorseElement::Dit;
    }

    if (dahNow &&
        !usbDahPressed)
    {
        usbDahPressed = dahNow;

        return MorseElement::Dah;
    }

    usbDitPressed = ditNow;
    usbDahPressed = dahNow;

    return MorseElement::None;
}