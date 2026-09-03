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
    SerialPort serial("\\\\.\\COM3");

    MorseDecoder decoder;

    if (!serial.IsConnected())
    {
        std::cout << "COM connct failer!! key mode\n";

        std::string currentCode;
                
        bool hPressed = false;
        bool tPressed = false;
        bool spacePressed = false;

        std::cout << "CW Trainer Test\n";
		std::cout << "Paddle mode\n";
        std::cout << "H = DIT(.)\n";    
        std::cout << "T = DAH(-)\n";
        std::cout << "SPACE = Decode\n";
        std::cout << "ESC = end\n\n";

        while (true)
        {

            bool hNow = false;
            bool tNow = false;

            hNow = (GetAsyncKeyState('H') & 0x8000) != 0;
            tNow = (GetAsyncKeyState('T') & 0x8000) != 0;
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
                std::cout << currentCode << " = " << decoder.Decode(currentCode) << std::endl;
                currentCode.clear();
            }

            hPressed = hNow;
            tPressed = tNow;
            spacePressed = spaceNow;

            // ESCで修了
            if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
            {
                break;
            }

            Sleep(20);
        }
        
    }
    else {
        std::string currentCode;

        char buffer[128];

        std::cout << "SPACE = Decode\n";

        while (true)
        {
            int n = serial.ReadData(buffer,sizeof(buffer) - 1);

            if (n > 0)
            {
                buffer[n] = '\0';

                std::string msg(buffer);

                if (msg.find("DIT") != std::string::npos)
                {
                    currentCode += ".";
                    Beep(700, 50);
                    std::cout << currentCode << std::endl;
                }

                if (msg.find("DAH") != std::string::npos)
                {
                    currentCode += "-";
                    Beep(700, 150);
                    std::cout << currentCode << std::endl;
                }
            }

            if (GetAsyncKeyState(VK_SPACE) & 0x8000)
            {
                std::cout << " = " << decoder.Decode(currentCode) << std::endl;
                currentCode.clear();
                Sleep(300);
            }

            Sleep(10);
        }
    }


    return 0;
}

