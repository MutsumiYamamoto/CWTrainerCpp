#include "Training.h"

void RandomTraining::Initialize()
{
	//ランダムモードの初期化処理
	std::cout << "RandomTraining Initialized" << std::endl;
	this->piontCalculate = true;
}
void RandomTraining::questionPosing()
{
	// 1～5文字のランダム長
	std::uniform_int_distribution<> lenDist(1, questionLength);
	int length = lenDist(mt);

	currentQuestion.clear();

	// ランダムな文字列を作成
	std::uniform_int_distribution<> charDist(
		0,
		static_cast<int>(candidates.size()) - 1);

	for (int i = 0; i < length; i++)
	{
		currentQuestion += candidates[charDist(mt)];
	}
	std::cout << "=========================\n";
	std::cout << "Question "
		<< this->questionCount
		<< ": "
		<< currentQuestion
		<< std::endl;
}

bool RandomTraining::checkAnswer(const std::string& answer)
{
	std::string trimmed = answer;
	trimmed.erase(trimmed.find_last_not_of(" \t\r\n") + 1);

	std::cout << "charactor=[" << charactor << "]" << std::endl;
	std::cout << "answer=[" << answer << "]" << std::endl;

	std::cout << "Question " << this->questionCount << ": " << currentQuestion
		<< " (Answer: " << trimmed << ")" << std::endl;
	return currentQuestion == trimmed;
}