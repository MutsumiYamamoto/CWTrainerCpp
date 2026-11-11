#pragma once

#include "CommandAction.h"

class CommandInput
{
public:

    CommandAction Poll();

private:

    bool deletePressed = false;

    bool tabPressed = false;

    bool f1Pressed = false;

};