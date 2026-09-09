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

struct HistoryItem {
    std::string code;
    std::string text;
    HistoryItem(const std::string& c, const std::string& t) : code(c), text(t) {}
};

int main()
{
    cout << "=========================\n";
    cout << "      CW Trainer\n";
    cout << "=========================\n\n";

 //   cout << "u : USB Paddle\n";
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

	int wpm;    // Words Per Minute

    cout << "WPM (5-40) > ";
    cin >> wpm; 

    if (wpm < 5)
        wpm = 5;

    if (wpm > 40)
        wpm = 40;

	int ditMs = 1200 / wpm; // DITの長さ（ミリ秒）
	int thresholdMs = ditMs * 2;    // DITとDAHの判定閾値（ミリ秒）
    int charGapMs = ditMs * 3;    // 文字間隔（ミリ秒）
    int wordGapMs = ditMs * 9;    // 単語間隔（ミリ秒）

    cout << "\n";
    cout << "DIT      : " << ditMs << " ms\n";
    cout << "THRESHOLD: " << thresholdMs << " ms\n";
    cout << "CHAR GAP : " << charGapMs << " ms\n";
    cout << "WORD GAP : " << wordGapMs << " ms\n\n";

    cout << "ESC = Exit\n\n";

    MorseDecoder decoder;

	string currentMorse;    //現在入力中のモールス信号格納
	string translatedText;  //翻訳済み文字列格納

	vector<HistoryItem> history;    //履歴格納

	auto lastInputTime = steady_clock::now();   //最後のキー入力時刻

	auto keyDownTime = steady_clock::now(); //キー押下時刻

	bool hPressed = false;  //Hキー押下状態
	bool tPressed = false;  //Tキー押下状態
	bool spacePressed = false;  //SPACEキー押下状態

    bool characterDecoded = true;   //キー入力受付フラグ

    bool usbDitPressed = false;
    bool usbDahPressed = false;

    auto AddDit = [&]()
        {
			currentMorse += '.';    //モールス信号にDIT追加
			cout << '.';    // 画面に表示
			cout.flush();   // 画面に表示
			Beep(700, ditMs);   // DIT音を鳴らす
			lastInputTime = steady_clock::now();    //最後のキー入力時刻更新
			characterDecoded = false;   //キー入力受付フラグOFF
        };

    auto AddDah = [&]()
        {
            currentMorse += '-';
            cout << '-';
            cout.flush();
            Beep(700, ditMs * 3);
            lastInputTime = steady_clock::now();
            characterDecoded = false;
        };

    while (true)
    {
		if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)   // ESCキー押下で終了
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

			hPressed = hNow;    // Hキー押下状態更新
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
				keyDownTime = steady_clock::now();  // キー押下時刻記録
				spacePressed = true;    // SPACEキー押下状態更新
            }

            if (!spaceNow && spacePressed)
            {
				auto pressTime = duration_cast<milliseconds>(steady_clock::now() - keyDownTime).count();    // キー押下時間計測

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
            bool ditNow = (GetAsyncKeyState(VK_LCONTROL) & 0x8000) != 0;
            bool dahNow = (GetAsyncKeyState('A') & 0x8000) != 0;

            if (ditNow && !usbDitPressed)
            {
                cout << "[DIT]";
                AddDit();
            }

            if (dahNow && !usbDahPressed)
            {
                cout << "[DAH]";
                AddDah();
            }

            usbDitPressed = ditNow;
            usbDahPressed = dahNow;
        }


		auto now = steady_clock::now(); // 現在時刻取得

		auto idleMs = duration_cast<milliseconds>(now - lastInputTime).count(); // 最後のキー入力からの経過時間（ミリ秒）

        //-----------------------------------
        // 文字確定
        //-----------------------------------

		if (!characterDecoded &&        // 文字確定前
			!currentMorse.empty() &&    // モールス信号が入力されている
			idleMs >= charGapMs)        // 一定時間入力がなければ文字確定
        {
			MorseDecoder decoder;
            std::string result = decoder.Decode(currentMorse); // result を宣言して代入
            cout << " => " << result << "\n";

            if (!result.empty())
            {
                translatedText += result;
                // history.push_back({ currentMorse, result });
                history.emplace_back(currentMorse, result);
                // history.push_back(HistoryItem{ currentMorse, result });

                cout << " => " << result << "\n";
            }
            else
            {
                cout << " => ?\n";
            }

            translatedText += result;                       //文字確定
			history.push_back({ currentMorse, result });    //履歴に追加
			cout << " => " << result << "\n";               //変換結果を画面に表示
//			cout << "CHAR : " << translatedText << "\n";    //翻訳済み文字列を画面に表示
            currentMorse.clear();                           //入力文字クリア
			characterDecoded = true;                        //キー入力受付フラグON
        }

        //-----------------------------------
        // 単語区切り
        //-----------------------------------

		if (characterDecoded &&         // 文字確定済み
			idleMs >= wordGapMs)       // 一定時間入力がなければ単語区切り
        {
            if (!translatedText.empty() &&
                translatedText.back() != ' ')
            {
                translatedText += ' ';  //単語確定

				cout << "WORD : " << translatedText << "\n";    //翻訳済み文字列を画面に表示
            }

            lastInputTime = steady_clock::now();
        }

        Sleep(10);
    }

    cout << "\n\n========== RESULT ==========\n";
	cout << translatedText << "\n"; //翻訳済み文字列を画面に表示

#if 0
    cout << "\n========== HISTORY =========\n";

    for (size_t i = 0; i < history.size(); ++i)
    {
        cout << setw(3) << i + 1 << " : " << setw(6) << history[i].morse << " -> " << history[i].decoded << "\n";
    }
#endif
    cout << "\n=============================\n";

    return 0;
}
