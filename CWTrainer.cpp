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

    singleInput =
        new SingleInput(
            thresholdMs);
}

CWTrainer::~CWTrainer()
{
    delete singleInput;
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

    //モールス入力キー
    bool hPressed = false;  //Hキー押下状態
    bool tPressed = false;  //Tキー押下状態

    bool spacePressed = false;  //SPACEキー押下状態

    bool usbDitPressed = false; //USB DITキー押下状態
    bool usbDahPressed = false; //USB DAHキー押下状態

    //キーによる機能
    bool deletePressed = false; // DELETEキー押下状態
    bool tabPressed = false; // TABキー押下状態




    //マイク入力関連
    int micDevice = -1; // マイクデバイス番号
    PaStream* stream = nullptr; // PortAudioストリーム

    bool micTone = false;   // マイク入力の700Hzトーン検出状態
    bool micTonePrev = false;   // マイク入力の700Hzトーン検出状態（前回）

    auto micToneStart = steady_clock::now();    // マイク入力の700Hzトーン検出開始時刻
    auto micToneEnd = steady_clock::now();      // マイク入力の700Hzトーン検出終了時刻

    float micBuffer[FRAME_SIZE];    // マイク入力バッファ

    if (mode == InputMode::Mic)
    {
        PaError err = Pa_Initialize();

        if (err != paNoError)
        {
            cout << "PortAudio initialize failed.\n";
            return -1;
        }

        cout << "\n=== Input Devices ===\n";

        int deviceCount = Pa_GetDeviceCount();

        for (int i = 0; i < deviceCount; i++)
        {
            const PaDeviceInfo* info = Pa_GetDeviceInfo(i);

            if (info->maxInputChannels > 0)
            {
                cout
                    << i
                    << " : "
                    << info->name
                    << "\n";
            }
        }

        cout << "\nDevice Number > ";
        cin >> micDevice;

        if (micDevice < 0 || micDevice >= deviceCount)
        {
            cout << "Invalid device.\n";
            return -1;
        }

        PaStreamParameters inputParams;

        inputParams.device = micDevice;
        inputParams.channelCount = 1;
        inputParams.sampleFormat = paFloat32;
        inputParams.suggestedLatency =
            Pa_GetDeviceInfo(micDevice)->defaultLowInputLatency;
        inputParams.hostApiSpecificStreamInfo = nullptr;

        err = Pa_OpenStream(
            &stream,
            &inputParams,
            nullptr,
            SAMPLE_RATE,
            FRAME_SIZE,
            paNoFlag,
            nullptr,
            nullptr);

        if (err != paNoError)
        {
            cout << "Pa_OpenStream failed.\n";
            return -1;
        }

        err = Pa_StartStream(stream);

        if (err != paNoError)
        {
            cout << "Pa_StartStream failed.\n";
            return -1;
        }

        cout << "\nMIC monitoring start...\n";
    }



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
        // ESCキーで終了
        //-----------------------------------
        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)   // ESCキー押下で終了
        {
            break;
        }

        //-----------------------------------
        // DELETEキーでWORDバッファクリア
        //-----------------------------------

        bool deleteNow =
            (GetAsyncKeyState(VK_DELETE) & 0x8000) != 0;

        if (deleteNow && !deletePressed)
        {
            translatedText.clear();

            cout << "\n========== WORD BUFFER CLEARED ==========\n";
            cout << "CHAR : " << translatedText << "\n";
            cout << "\n***** My call signe *********************\n";
            cout << "\n JL1IRF(.---/.-../.----/../.-./..-.)\n";
            cout << "\n*****************************************\n";
        }

        deletePressed = deleteNow;

        //-----------------------------------
        // TABキーでWORDバッファ表示
        //-----------------------------------

        bool dispNow =
            (GetAsyncKeyState(VK_TAB) & 0x8000) != 0;

        if (dispNow && !tabPressed) //TABキーの状態が!falseなら
        {
            cout << "\n========== WORD BUFFER NOW ==============\n";
            cout << "CHAR : " << translatedText << "\n"; //翻訳済み文字列を画面に表示
            cout << "\n***** My call signe *********************\n";
            cout << "\n JL1IRF(.---/.-../.----/../.-./..-.)\n";
            cout << "\n*****************************************\n";
        }

        tabPressed = dispNow;   //次のループではここに入らない

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
            auto result =
                singleInput->Poll();

            switch (result)
            {
            case MorseElement::Dit:

                AddDit();

                break;

            case MorseElement::Dah:

                AddDah();

                break;

            default:

                break;
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

        //-----------------------------------
        // Mic入力モード
        //-----------------------------------
#if 0
        if (mode == InputMode::Mic)
        {
            PaError err =
                Pa_ReadStream(
                    stream,
                    micBuffer,
                    FRAME_SIZE);

            if (err == paNoError)
            {
                double power =
                    Goertzel(
                        micBuffer,
                        FRAME_SIZE,
                        SAMPLE_RATE,
                        TARGET_FREQ);

                const double threshold = 100.0;

                micTone = (power > threshold);

                cout << "\r700Hz Power="
                    << fixed
                    << setprecision(0)
                    << power
                    << "     ";

                if (micTone && !micTonePrev)
                {
                    micToneStart =
                        steady_clock::now();
                }

                if (!micTone && micTonePrev)
                {
                    micToneEnd =
                        steady_clock::now();

                    auto toneMs =
                        duration_cast<milliseconds>(
                            micToneEnd -
                            micToneStart)
                        .count();

                    cout << "\nTone "
                        << toneMs
                        << " ms ";

                    if (toneMs < thresholdMs)
                    {
                        AddDit();
                    }
                    else
                    {
                        AddDah();
                    }
                }

                micTonePrev = micTone;
            }
        }
#endif


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

    if (stream)
    {
        Pa_StopStream(stream);
        Pa_CloseStream(stream);
    }

    if (mode == InputMode::Mic)
    {
        Pa_Terminate();
    }



    return 0;
}

