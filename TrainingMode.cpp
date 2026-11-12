#include "TrainingMode.h"

Training::Training()
	: score(nullptr),
	  questionCount(0),
	  totalQuestion(0),
	  correctNum(0)
{
}



void Training::Initialize()
{
	SetTotalQuestion(10);	//–â‘è”‚ğİ’è

}

void Training::questionPosing()
{
	//–â‘èo‘èˆ—

	//charactor‚Éƒ‰ƒ“ƒ_ƒ€‚É‚P•¶š‚ğ“ü‚ê‚é
	static std::mt19937 mt{ std::random_device{}() };
	std::uniform_int_distribution<> dist('A', 'Z');

	charactor = std::string(1, static_cast<char>(dist(mt)));

	std::cout << "Question " << this->questionCount << ": " << charactor << std::endl;
}

bool Training::checkAnswer(const std::string& answer)
{
	//‰ğ“š”»’èˆ—
//		std::cout << "charactor=[" << charactor << "]" << std::endl;
//		std::cout << "answer=[" << answer << "]" << std::endl

	std::string trimmed = answer;
	trimmed.erase(trimmed.find_last_not_of(" \t\r\n") + 1);
	std::cout << "Question " << this->questionCount << ": " << charactor
		<< " (Answer: " << trimmed << ")" << std::endl;
	return charactor == trimmed;

}

