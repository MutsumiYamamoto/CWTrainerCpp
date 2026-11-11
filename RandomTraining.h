#pragma once

#include <iostream>
#include <random>
#include "Training.h"


class RandomTraining : public Training
{
private:
	std::string candidates =
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

	std::string currentQuestion;    // 今回の問題

	std::mt19937 mt{ std::random_device{}() };

public:
	RandomTraining() = default;
	~RandomTraining() = default;

	void Initialize() override
	{
		//ランダムモードの初期化処理
		std::cout << "RandomTraining Initialized" << std::endl;
	}
	void questionPosing() override
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

	bool checkAnswer(const std::string& answer) override
	{
		std::string trimmed = answer;
		trimmed.erase(trimmed.find_last_not_of(" \t\r\n") + 1);

		std::cout << "charactor=[" << charactor << "]" << std::endl;
		std::cout << "answer=[" << answer << "]" << std::endl;

		std::cout << "Question " << this->questionCount << ": " << currentQuestion
			<< " (Answer: " << trimmed << ")" << std::endl;
		return currentQuestion == trimmed;
	}


};
