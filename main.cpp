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

using namespace std;
using namespace std::chrono;

enum class InputMode
{
    USB,
    Paddle,
    Single,
    Mic
};

struct HistoryItem {
    std::string code;
    std::string text;
    HistoryItem(const std::string& c, const std::string& t) : code(c), text(t) {}
};

double Goertzel(const float* samples,
    int sampleCount,
    int sampleRate,
    double targetFreq)
{
    int k =
        static_cast<int>(
            0.5 +
            ((sampleCount * targetFreq) /
                sampleRate));

    double omega =
        (2.0 * 3.141592653589793 * k) /
        sampleCount;

    double coeff = 2.0 * cos(omega);

    double q0 = 0.0;
    double q1 = 0.0;
    double q2 = 0.0;

    for (int i = 0; i < sampleCount; ++i)
    {
        q0 = coeff * q1 - q2 + samples[i];
        q2 = q1;
        q1 = q0;
    }

    double power =
        q1 * q1 +
        q2 * q2 -
        coeff * q1 * q2;

    return power;
};


int main()
{
    cout << "=========================\n";
    cout << "      CW Trainer\n";
    cout << "=========================\n\n";

    cout << "u : USB Paddle\n";
    cout << "p : PC Paddle (H/T)\n";
    cout << "s : Single Key (SPACE)\n";
    cout << "m : input MIC\n\n";

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

    cout << "C = Clear\n\n";

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

    constexpr int SAMPLE_RATE = 8000;
    constexpr int FRAME_SIZE = 256;
    constexpr double TARGET_FREQ = 700.0;

    bool deletePressed = false;

    int micDevice = -1;
    PaStream* stream = nullptr;

    bool micTone = false;
    bool micTonePrev = false;

    auto micToneStart = steady_clock::now();
    auto micToneEnd = steady_clock::now();

    float micBuffer[FRAME_SIZE];

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

            cout << "\n*** WORD BUFFER CLEARED ***\n";
            cout << "CHAR : " << translatedText << "\n";
        }

        deletePressed = deleteNow;



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

        //-----------------------------------
        // Mic入力モード
        //-----------------------------------

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

#if 0
    cout << "\n========== HISTORY =========\n";

    for (size_t i = 0; i < history.size(); ++i)
    {
        cout << setw(3) << i + 1 << " : " << setw(6) << history[i].morse << " -> " << history[i].decoded << "\n";
    }
#endif
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
