
#include "TrainingMode.h"

void CWTrainingMode::Initialize()
{
	//CWモードの初期化処理
	std::cout << "CWTraining Initialized" << std::endl;
}
void CWTrainingMode::questionPosing()
{
	//CWモードの問題出題処理
	std::cout << "CWTraining Question" << std::endl;
	//CWモードでは、実際更新をしているようなシナリオに応答する

}
bool CWTrainingMode::checkAnswer(const std::string& answer)
{
	//CWモードの解答判定処理
	std::cout << "CWTraining Answer" << std::endl;

	std::string trimmed = answer;
	trimmed.erase(trimmed.find_last_not_of(" \t\r\n") + 1);
	std::cout << "Question " << this->questionCount << ": " << charactor
		<< " (Answer: " << trimmed << ")" << std::endl;
	return charactor == trimmed;
}