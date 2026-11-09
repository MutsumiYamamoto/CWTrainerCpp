#pragma once

#include <string>
#include <vector>
#include <chrono>

#include "MorseDecoder.h"
#include "SingleInput.h"
#include "PaddleInput.h"
#include "UsbInput.h"
#include "MicInput.h"
#include "CommandInput.h"
#include "DisplayManager.h"

struct HistoryItem
{
    std::string code;
    std::string text;

    HistoryItem(
        const std::string& c,
        const std::string& t)
        :
        code(c),
        text(t)
    {
    }
};

enum class InputMode
{
    USB,
    Paddle,
    Single,
    Mic
};


class CWTrainer
{
public:

    CWTrainer();
    ~CWTrainer();

    int Run();

private:

    void AddDit();
    void AddDah();

private:

    MorseDecoder decoder;

    std::string currentMorse;
    std::string translatedText;

    std::vector<HistoryItem> history;

    int wpm;

    int ditMs;
    int thresholdMs;
    int charGapMs;
    int wordGapMs;

    std::chrono::steady_clock::time_point
        lastInputTime;

    bool characterDecoded;

	IInputDevice* inputDevice; // ← 追加: IInputDevice型のポインタをメンバーとして宣言

	CommandInput commandInput;

    DisplayManager display;

};