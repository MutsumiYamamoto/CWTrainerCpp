#include "IInputDevice.h"

#include <iostream>
#include <iomanip>
#include <cmath>

using namespace std;
using namespace std::chrono;

//constexpr double MicInput::TARGET_FREQ = 700.0;

MicInput::MicInput(
    int threshold)
    : thresholdMs(threshold),
    micDevice(-1),
    stream(nullptr),
    micTone(false),
    micTonePrev(false),
    micBuffer{ 0.0f }
{
}

MicInput::~MicInput()
{
    if (stream)
    {
        Pa_StopStream(stream);

        Pa_CloseStream(stream);
    }

    Pa_Terminate();
}

double MicInput::Goertzel(
    const float* samples,
    int sampleCount,
    int sampleRate,
    double targetFreq)
{
    int k =
        static_cast<int>(
            0.5 +
            ((sampleCount *
                targetFreq)
                / sampleRate));

    double omega =
        (2.0 * 3.141592653589793 * k)
        / sampleCount;

    double coeff =
        2.0 * cos(omega);

    double q0 = 0.0;
    double q1 = 0.0;
    double q2 = 0.0;

    for (int i = 0;
        i < sampleCount;
        i++)
    {
        q0 =
            coeff * q1
            - q2
            + samples[i];

        q2 = q1;
        q1 = q0;
    }

    return
        q1 * q1 +
        q2 * q2 -
        coeff * q1 * q2;
}

bool MicInput::Initialize()
{
    PaError err =
        Pa_Initialize();

    if (err != paNoError)
    {
        return false;
    }

    cout
        << "\n=== Input Devices ===\n";

    int deviceCount =
        Pa_GetDeviceCount();

    for (int i = 0;
        i < deviceCount;
        i++)
    {
        auto info =
            Pa_GetDeviceInfo(i);

        if (info->maxInputChannels > 0)
        {
            cout
                << i
                << " : "
                << info->name
                << "\n";
        }
    }

    cout
        << "\nDevice Number > ";

    cin
        >> micDevice;

    if (micDevice < 0 ||
        micDevice >= deviceCount)
    {
        return false;
    }

    PaStreamParameters inputParams;

    inputParams.device =
        micDevice;

    inputParams.channelCount = 1;

    inputParams.sampleFormat =
        paFloat32;

    inputParams.suggestedLatency =
        Pa_GetDeviceInfo(micDevice)
        ->defaultLowInputLatency;

    inputParams.hostApiSpecificStreamInfo =
        nullptr;

    err =
        Pa_OpenStream(
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
        return false;
    }

    err =
        Pa_StartStream(stream);

    if (err != paNoError)
    {
        return false;
    }

    cout
        << "\nMIC monitoring start...\n";

    return true;
}


MorseElement
MicInput::Poll()
{
    PaError err =
        Pa_ReadStream(
            stream,
            micBuffer,
            FRAME_SIZE);

    if (err != paNoError)
    {
        return MorseElement::None;
    }

    double power =
        Goertzel(
            micBuffer,
            FRAME_SIZE,
            SAMPLE_RATE,
            TARGET_FREQ);

    const double threshold =
        100.0;

    micTone =
        (power > threshold);

    cout
        << "\r700Hz Power="
        << fixed
        << setprecision(0)
        << power
        << "     ";

    if (micTone &&
        !micTonePrev)
    {
        micToneStart =
            steady_clock::now();
    }

    if (!micTone &&
        micTonePrev)
    {
        micToneEnd =
            steady_clock::now();

        auto toneMs =
            duration_cast<milliseconds>(
                micToneEnd -
                micToneStart)
            .count();

        cout
            << "\nTone "
            << toneMs
            << " ms ";

        micTonePrev =
            micTone;

        if (toneMs < thresholdMs)
        {
            return MorseElement::Dit;
        }

        return MorseElement::Dah;
    }

    micTonePrev =
        micTone;

    return MorseElement::None;
}

