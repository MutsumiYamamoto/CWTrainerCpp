#pragma once


#include <chrono>

#include <portaudio.h>

#include "MorseElement.h"


class IInputDevice
{
public:

    virtual ~IInputDevice() = default;

    virtual MorseElement Poll() = 0;

private:
    IInputDevice* inputDevice = nullptr;
};


class PaddleInput : public IInputDevice
{
public:

    MorseElement Poll() override;

private:

    bool hPressed = false;

    bool tPressed = false;
};


class SingleInput : public IInputDevice
{
public:

    explicit SingleInput(
        int thresholdMs);

    MorseElement Poll() override;

private:

    int thresholdMs;

    bool spacePressed;

    std::chrono::steady_clock::
        time_point keyDownTime;
};



class UsbInput : public IInputDevice
{
public:

    MorseElement Poll() override;

private:

    bool usbDitPressed = false;

    bool usbDahPressed = false;
};





class MicInput : public IInputDevice
{
public:

    explicit MicInput(
        int thresholdMs);

    ~MicInput();

    bool Initialize();

    MorseElement Poll() override;

private:

    static constexpr int SAMPLE_RATE = 8000;

    static constexpr int FRAME_SIZE = 256;

    static constexpr double TARGET_FREQ = 700.0;   // 700HzÇÃé¸îgêî

private:

    double Goertzel(
        const float* samples,
        int sampleCount,
        int sampleRate,
        double targetFreq);

private:

    int thresholdMs;

    int micDevice;

    PaStream* stream;

    float micBuffer[FRAME_SIZE];

    bool micTone;

    bool micTonePrev;

    std::chrono::steady_clock::time_point
        micToneStart;

    std::chrono::steady_clock::time_point
        micToneEnd;
};
