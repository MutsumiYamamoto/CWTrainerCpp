#include <windows.h>

#include <chrono>
#include <cctype>
#include <iomanip>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "MorseDecoder.h"

using namespace std;
using namespace std::chrono;

enum class InputMode
{
    USB,
    Paddle,
    Single
};

struct HistoryItem
{
    string morse;
    char decoded;
};

int main()
{
    cout << "=========================\n";
    cout << "      CW Trainer\n";
    cout << "=========================\n\n";

    cout << "u : USB Paddle\n";
    cout << "p : PC Paddle (H/T)\n";
    cout << "s : Single Key (SPACE)\n\n";

    cout << "Select > ";

    char modeChar;
    cin >> modeChar;

    InputMode mode;

    switch (tolower(modeChar))
    {
    case 'u':
        mode = InputMode::USB;
        break;

    case 'p':
        mode = InputMode::Paddle;
        break;

    case 's':
        mode = InputMode::Single;
        break;

    default:
        return 0;
    }

    int wpm;

    cout << "WPM (5-40) > ";
    cin >> wpm;

    if (wpm < 5)
        wpm = 5;

    if (wpm > 40)
        wpm = 40;

    int ditMs = 1200 / wpm;
    int thresholdMs = ditMs * 2;
    int charGapMs = ditMs * 3;
    int wordGapMs = ditMs * 7;

    cout << "\n";
    cout << "DIT      : " << ditMs << " ms\n";
    cout << "THRESHOLD: " << thresholdMs << " ms\n";
    cout << "CHAR GAP : " << charGapMs << " ms\n";
    cout << "WORD GAP : " << wordGapMs << " ms\n\n";

    cout << "ESC = Exit\n\n";

    MorseDecoder decoder;

    string currentMorse;
    string translatedText;

    vector<HistoryItem> history;

    auto lastInputTime = steady_clock::now();

    auto keyDownTime = steady_clock::now();

    bool hPressed = false;
    bool tPressed = false;
    bool spacePressed = false;

    bool characterDecoded = true;

    auto AddDit = [&]()
        {
            currentMorse += '.';

            cout << '.';
            cout.flush();

            Beep(700, ditMs);

            lastInputTime =
                steady_clock::now();

            characterDecoded = false;
        };

    auto AddDah = [&]()
        {
            currentMorse += '-';

            cout << '-';
            cout.flush();

            Beep(700, ditMs * 3);

            lastInputTime =
                steady_clock::now();

            characterDecoded = false;
        };

    while (true)
    {
        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
        {
            break;
        }

        //-----------------------------------
        // Pモード
        //-----------------------------------

        if (mode == InputMode::Paddle)
        {
            bool hNow = (GetAsyncKeyState('H') & 0x8000) != 0;

            bool tNow = (GetAsyncKeyState('T') & 0x8000) != 0;

            if (hNow && !hPressed)
            {
                AddDit();
            }

            if (tNow && !tPressed)
            {
                AddDah();
            }

            hPressed = hNow;
            tPressed = tNow;
        }

        //-----------------------------------
        // Sモード
        //-----------------------------------

        if (mode == InputMode::Single)
        {
            bool spaceNow = (GetAsyncKeyState(VK_SPACE) & 0x8000) != 0;

            if (spaceNow && !spacePressed)
            {
                keyDownTime = steady_clock::now();

                spacePressed = true;
            }

            if (!spaceNow && spacePressed)
            {
                auto pressTime =
                    duration_cast<milliseconds>(steady_clock::now() - keyDownTime).count();

                if (pressTime < thresholdMs)
                {
                    AddDit();
                }
                else
                {
                    AddDah();
                }

                spacePressed = false;
            }
        }

        //-----------------------------------
        // USBモード
        //-----------------------------------

        if (mode == InputMode::USB)
        {
            // 将来Arduino接続時に実装
        }

        auto now =
            steady_clock::now();

        auto idleMs =
            duration_cast<milliseconds>(
                now - lastInputTime)
            .count();

        //-----------------------------------
        // 文字確定
        //-----------------------------------

        if (!characterDecoded &&
            !currentMorse.empty() &&
            idleMs >= charGapMs)
        {
            std::string tmp = decoder.Decode(currentMorse);
            char result = tmp.empty() ? '\0' : tmp[0];

            translatedText += result;

            history.push_back({ currentMorse, result });

            cout << " => " << result << "\n";

            cout << "TEXT : " << translatedText << "\n";

            currentMorse.clear();

            characterDecoded = true;
        }

        //-----------------------------------
        // 単語区切り
        //-----------------------------------

        if (characterDecoded &&
            idleMs >= wordGapMs)
        {
            if (!translatedText.empty() &&
                translatedText.back() != ' ')
            {
                translatedText += ' ';

                cout << "TEXT : " << translatedText << "\n";
            }

            lastInputTime = steady_clock::now();
        }

        Sleep(10);
    }

    cout << "\n\n========== RESULT ==========\n";
    cout << translatedText << "\n";

    cout << "\n========== HISTORY =========\n";

    for (size_t i = 0; i < history.size(); ++i)
    {
        cout << setw(3) << i + 1 << " : " << setw(6) << history[i].morse << " -> " << history[i].decoded << "\n";
    }

    return 0;
}
