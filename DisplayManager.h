#pragma once

#include <string>

class DisplayManager
{
public:

    void ShowTitle();
    void ShowSelectMode();
    void ShowSelectTrainingMode();
	void ShowInputQuestionNum();

    void ShowTiming(
        int ditMs,
        int thresholdMs,
        int charGapMs,
        int wordGapMs);

    void ShowCallSign();

	void ShowTrainingStart();

    void ShowBuffer(
        const std::string& text);

    void ShowBufferCleared();

    void ShowCharacter(
        const std::string& result);

    void ShowUnknownCharacter();

    void ShowWord(
        const std::string& word);

    void ShowResult(
        const int totaScore, const int totalQuestion, const int correctNum);

    void ShowKeyCommands();

};