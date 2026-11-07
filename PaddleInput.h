#pragma once

#include "MorseElement.h"

class PaddleInput
{
public:

    MorseElement Poll();

private:

    bool hPressed = false;

    bool tPressed = false;
};
