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