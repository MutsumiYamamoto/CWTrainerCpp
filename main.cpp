#include <iostream>
#include <string>
#include <windows.h>
#include <chrono>

#include "SerialPort.h"
#include "MorseDecoder.h"

using namespace std::chrono;

steady_clock::time_point pressTime;
steady_clock::time_point lastInputTime;

bool sPressed = false;
bool charDecoded = true;

int main()
{
    MorseDecoder decoder;

    std::string currentCode;

    bool hPressed = false;
    bool tPressed = false;
    bool spacePressed = false;

    std::cout << "CW Trainer Test\n";
    std::cout << "H = DIT(.)\n";
    std::cout << "T = DAH(-)\n";
    std::cout << "SPACE = Decode\n";
    std::cout << "ESC = Exit\n\n";

    while (true)
    {
        bool hNow = (GetAsyncKeyState('H') & 0x8000) != 0;

        bool tNow = (GetAsyncKeyState('T') & 0x8000) != 0;

        bool spaceNow = (GetAsyncKeyState(VK_SPACE) & 0x8000) != 0;

        // H = DIT
        if (hNow && !hPressed)
        {
            currentCode += ".";
            Beep(700, 50);
            std::cout << currentCode << std::endl;
        }

        // T = DAH
        if (tNow && !tPressed)
        {
            currentCode += "-";
            Beep(700, 150);
            std::cout << currentCode << std::endl;
        }

        // SPACEで確定
        if (spaceNow && !spacePressed)
        {
            std::string result = decoder.Decode(currentCode);
            std::cout << currentCode << " = " << result << std::endl;
            currentCode.clear();
        }

        hPressed = hNow;
        tPressed = tNow;
        spacePressed = spaceNow;

        
        //ESCで修了
        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
        {
            break;
        }

        Sleep(20);
    }

    return 0;
}
