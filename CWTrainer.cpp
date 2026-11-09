#include "CWTrainer.h"

#include <windows.h>
#include <chrono>
#include <cctype>
#include <iomanip>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <cmath>

#include <portaudio.h>

#include "MorseDecoder.h"
#include "SingleInput.h"


using namespace std;
using namespace std::chrono;

void CWTrainer::AddDit()
{
    currentMorse += '.';

    cout << '.';

    cout.flush();

    Beep(700, ditMs);

    lastInputTime =
        steady_clock::now();

    characterDecoded = false;
}

void CWTrainer::AddDah()
{
    currentMorse += '-';

    cout << '-';

    cout.flush();

    Beep(700, ditMs * 3);

    lastInputTime =
        steady_clock::now();

    characterDecoded = false;
}

CWTrainer::CWTrainer()
{
    wpm = 20;

    ditMs = 1200 / wpm;

    thresholdMs =
        ditMs * 2;

    charGapMs =
        ditMs * 3;

    wordGapMs =
        ditMs * 9;

    lastInputTime =
        steady_clock::now();

    characterDecoded = true;



}

CWTrainer::~CWTrainer()
{
    delete inputDevice;
}

int CWTrainer::Run()
{
    cout << "=========================\n";
    cout << "      CW Trainer\n";
    cout << "=========================\n\n";

    //モードの選択
     //   cout << "u : *USB Paddle\n";
    cout << "p : PC Paddle (H/T)\n";
    cout << "s : Single Key (SPACE)\n";
    //   cout << "m : *input MIC\n\n";

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

    case 'm':
        mode = InputMode::Mic;
        break;

    default:
        return 0;
    }

    switch (mode)
    {
    case InputMode::Single:

        inputDevice =
            new SingleInput(
                thresholdMs);

        break;

    case InputMode::Paddle:

        inputDevice =
            new PaddleInput();

        break;

    case InputMode::USB:

        inputDevice =
            new UsbInput();

        break;

    case InputMode::Mic:

        inputDevice =
            new MicInput(
                thresholdMs);

        break;
    }

    int wpm;    // Words Per Minute

#if 0
    cout << "WPM (5-40) > ";
    cin >> wpm;

    if (wpm < 5)
        wpm = 5;

    if (wpm > 40)
        wpm = 40;
#else
    wpm = 20;
#endif

    int ditMs = 1200 / wpm; // DITの長さ（ミリ秒）
    int thresholdMs = ditMs * 2;    // DITとDAHの判定閾値（ミリ秒）
    int charGapMs = ditMs * 3;    // 文字間隔（ミリ秒）
    int wordGapMs = ditMs * 9;    // 単語間隔（ミリ秒）

    cout << "\n";
    cout << "DIT      : " << ditMs << " ms\n";
    cout << "THRESHOLD: " << thresholdMs << " ms\n";
    cout << "CHAR GAP : " << charGapMs << " ms\n";
    cout << "WORD GAP : " << wordGapMs << " ms\n\n";

    cout << "TAB = Disp\n\n";

    cout << "DELETE = Clear\n\n";

    cout << "ESC = Exit\n\n";

    cout << "\n\n========== My call signe ==========\n";
    cout << "\n JL1IRF(.---/.-../.----/../.-./..-.)\n";
    cout << "\n=====================================\n";

    MorseDecoder decoder;

    string currentMorse;    //現在入力中のモールス信号格納
    string translatedText;  //翻訳済み文字列格納

    vector<HistoryItem> history;    //履歴格納

    //検出パラメータ
    constexpr int SAMPLE_RATE = 8000;   // サンプリング周波数
    constexpr int FRAME_SIZE = 256;     // フレームサイズ
    constexpr double TARGET_FREQ = 700.0;   // 700Hzの周波数

    auto lastInputTime = steady_clock::now();   //最後のキー入力時刻
    auto keyDownTime = steady_clock::now(); //キー押下時刻
    bool characterDecoded = true;   //キー入力受付フラグ






    //短音入力時の処理
    auto AddDit = [&]()
        {
            currentMorse += '.';    //モールス信号にDIT追加
            cout << '.';    // 画面に表示
            cout.flush();   // 画面に表示
            Beep(700, ditMs);   // DIT音を鳴らす
            lastInputTime = steady_clock::now();    //最後のキー入力時刻更新
            characterDecoded = false;   //キー入力受付フラグOFF
        };

    //長音入力の処理
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
        //-----------------------------------
        // コマンド入力
        //-----------------------------------
        auto action =
            commandInput.Poll();

        switch (action)
        {
        case CommandAction::Exit:

            return 0;

        case CommandAction::ClearBuffer:

            translatedText.clear();

            cout
                << "\n========== WORD BUFFER CLEARED ==========\n";

            cout
                << "CHAR : "
                << translatedText
                << "\n";

            cout
                << "\n***** My call signe *********************\n";

            cout
                << "\n JL1IRF(.---/.-../.----/../.-./..-.)\n";

            cout
                << "\n*****************************************\n";

            break;

        case CommandAction::DisplayBuffer:

            cout
                << "\n========== WORD BUFFER NOW ==============\n";

            cout
                << "CHAR : "
                << translatedText
                << "\n";

            cout
                << "\n***** My call signe *********************\n";

            cout
                << "\n JL1IRF(.---/.-../.----/../.-./..-.)\n";

            cout
                << "\n*****************************************\n";

            break;

        default:

            break;
        }

        //-----------------------------------
        // 各動作モードを継承
        //-----------------------------------

        auto result =
            inputDevice->Poll();

        switch (result)
        {
        case MorseElement::Dit:

            if (mode == InputMode::USB)
            {
                cout << "[DIT]";
            }

            AddDit();

            break;

        case MorseElement::Dah:

            if (mode == InputMode::USB)
            {
                cout << "[DAH]";
            }

            AddDah();

            break;

        default:

            break;
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
    cout << "\n=============================\n";

//    if (stream)
//    {
//       Pa_StopStream(stream);
//        Pa_CloseStream(stream);
//    }

    if (mode == InputMode::Mic)
    {
        Pa_Terminate();
    }



    return 0;
}

