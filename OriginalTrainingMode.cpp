#include "TrainingMode.h"
#include "ScenarioManager.h"

void OriginalTrainingMode::Initialize()
{
	//独自モードの初期化処理
	std::cout << "OriginalTraining Initialized" << std::endl;
	this->piontCalculate = true;	//独自モードでは、正解数をカウントするためにpiontCalculateをtrueに設定


	//シナリオファイルをバッファに読み込み
	scenarioManager = new ScenarioManager();
	scenarioManager->LoadScenarioFile("scenario.txt");


}
void OriginalTrainingMode::questionPosing()
{
	//独自モードの問題出題処理
	std::cout << "OriginalTraining Question" << std::endl;

	//バッファに読み込んだシナリオから1行を出題
	this->currentQuestion = scenarioManager->GetNextLine();
	if (this->currentQuestion.empty())
	{
		std::cout << "No more questions in the scenario." << std::endl;
		return;
	}

	std::cout << "=========================\n";
	std::cout << "Question "
		<< this->questionCount
		<< ": "
		<< this->currentQuestion
		<< std::endl;


}
bool OriginalTrainingMode::checkAnswer(const std::string& answer)
{
	//独自モードの解答判定処理
	std::cout << "OriginalTraining Answer" << std::endl;
	std::string trimmed = answer;
	trimmed.erase(trimmed.find_last_not_of(" \t\r\n") + 1);
	std::cout << "Question " << this->questionCount << ": " << charactor
		<< " (Answer: " << trimmed << ")" << std::endl;
	return charactor == trimmed;
}
