#include "CommandInput.h"

#include <windows.h>

CommandAction CommandInput::Poll()
{
    //-----------------------------------
    // ESC
    //-----------------------------------

    if (GetAsyncKeyState(VK_ESCAPE)
        & 0x8000)
    {
        return CommandAction::Exit;
    }

    //-----------------------------------
    // F1
    //-----------------------------------
    bool helpNow =
        (GetAsyncKeyState(VK_F1)
            & 0x8000) != 0;

    if (helpNow &&
        !f1Pressed)
    {
        f1Pressed = true;

        return CommandAction::Help;
    }

    f1Pressed = helpNow;
   

    //-----------------------------------
    // DELETE
    //-----------------------------------

    bool deleteNow =
        (GetAsyncKeyState(VK_DELETE)
            & 0x8000) != 0;

    if (deleteNow &&
        !deletePressed)
    {
        deletePressed = true;

        return CommandAction::ClearBuffer;
    }

    deletePressed = deleteNow;

    //-----------------------------------
    // TAB
    //-----------------------------------

    bool tabNow =
        (GetAsyncKeyState(VK_TAB)
            & 0x8000) != 0;

    if (tabNow &&
        !tabPressed)
    {
        tabPressed = true;

        return CommandAction::DisplayBuffer;
    }

    tabPressed = tabNow;

    return CommandAction::None;
}