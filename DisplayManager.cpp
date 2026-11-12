#include "DisplayManager.h"
#include "TrainingMode.h"

#include <windows.h>
#include <chrono>
#include <cctype>
#include <iomanip>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <cmath>
#include "CWTrainer.h"

void DisplayManager::ShowTitle()
{
    std::cout
        << "=========================\n";

    std::cout
        << "      CW Trainer\n";

    std::cout
        << "=========================\n";
}

void DisplayManager::ShowSelectMode()
{
    std::cout
        << "\n***** SELECT MODE *****\n";

    std::cout << "u : *USB Paddle\n";
    std::cout << "p : PC Paddle (H/T)\n";
    std::cout << "s : Single Key (SPACE)\n";
    std::cout << "m : *input MIC\n";
    std::cout << "Select > ";

    //    std::cout
    //        << "\n*****************************************\n";
}

void DisplayManager::ShowSelectTrainingMode()
{
    std::cout
        << "\n***** SELECT TRAINING MODE *****\n";

    std::cout << "n : NORMAL\n";       //打って表示するだけのモード
	std::cout << "r : RANDOM\n";       //ランダムなお題を出題するモード
    std::cout << "c : CW\n";           //CW（定型文）の練習モード
    std::cout << "o : ORIGINAL\n";    //独自に作成した問題を出題するモード
    std::cout << "Select > ";

    //    std::cout
    //        << "\n*****************************************\n";
}

void DisplayManager::ShowInputQuestionNum()
{
    std::cout << "How many Questions?" << std::endl;


}

void DisplayManager::ShowCallSign()
{
    std::cout
        << "\n***** My call signe *****\n";

    std::cout
        << "JL1IRF(.---/.-../.----/../.-./..-.)\n";

}

void DisplayManager::ShowHelp()
{
    std::cout << "\n***** HELP *****\n";
	this->ShowKeyCommands();
    this->ShowMorseSignal();
    this->ShowCallSign();

}

void DisplayManager::ShowTrainingStart()
{
    std::cout
        << "\n***** Start Training! *****\n";

}

void DisplayManager::ShowKeyCommands()
{
    std::cout << "\n***** KEY COMMANDS *****\n";

    std::cout << "TAB = Display Buffer and Next Question\n";
    std::cout << "DELETE = Clear\n";
    std::cout << "ESC = Exit\n";

//    std::cout
//        << "\n*****************************************\n";
}
void DisplayManager::ShowMorseSignal()
{

    std::cout << "\n***** MORSE CODE TABLE *****\n";
    this->ShowMorseCodeTable();

    //    std::cout
    //        << "\n*****************************************\n";
}

void DisplayManager::ShowMorseCodeTable()
{
    // モールス符号表の一例（必要に応じて拡張可能）
    std::cout << "A: .-\tB: -...\tC: -.-.\tD: -..\tE: .\n";
    std::cout << "F: ..-.\tG: --.\tH: ....\tI: ..\tJ: .---\n";
    std::cout << "K: -.-\tL: .-..\tM: --\tN: -.\tO: ---\n";
    std::cout << "P: .--.\tQ: --.-\tR: .-.\tS: ...\tT: -\n";
    std::cout << "U: ..-\tV: ...-\tW: .--\tX: -..-\tY: -.--\tZ: --..\n";
    std::cout << "1: .----\t2: ..---\t3: ...--\t4: ....-\t5: .....\n";
    std::cout << "6: -....\t7: --...\t8: ---..\t9: ----.\t0: -----\n";
}

void DisplayManager::ShowBuffer(
    const std::string& text)
{
    std::cout
        << "\n===== WORD BUFFER NOW =====\n";

    std::cout
        << "WORD : "
        << text
        << "\n";
}

void DisplayManager::ShowBufferCleared()
{
    std::cout
        << "\n===== WORD BUFFER CLEARED =====\n";

    std::cout
        << "WORD : \n";
}

void DisplayManager::ShowCharacter(
    const std::string& result)
{
    std::cout
        << " => "
        << result
        << "\n";
}

void DisplayManager::ShowWord(
    const std::string& word)
{
    std::cout
        << "WORD : "
        << word
        << "\n";
}

void DisplayManager::ShowResult(
    const int totaScore, const int totalQuestion, const int correctNum )
{
    std::cout << "\n\n========== RESULT ==========\n";

    std::cout << "Total Score:      " << totaScore << " points" << std::endl;
	std::cout << " Total Questions: " << totalQuestion << std::endl;
    std::cout << "  Correct:        " << correctNum << std::endl;
    std::cout << "  Incorrect:      " << totalQuestion - correctNum << std::endl;

    std::cout << "=============================\n";
}

void DisplayManager::ShowTiming(
    int ditMs,
    int thresholdMs,
    int charGapMs,
    int wordGapMs)
{
    // 既存のプロジェクト標準に従い、タイミング情報を表示
    std::cout << "Dit: " << ditMs
        << " ms, Threshold: " << thresholdMs
        << " ms, Char Gap: " << charGapMs
        << " ms, Word Gap: " << wordGapMs
        << " ms" << std::endl;
}

void DisplayManager::ShowUnknownCharacter()
{
    // 未知の文字が入力された場合の表示処理
    std::cout << "Unknown character detected." << std::endl;
}