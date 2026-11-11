#pragma once
#include <iostream>
#include <random>

#include "Training.h"
#include "ScoreManager.h"


class Training
{
protected:
	ScoreManager* score; // ← 追加: Score型のポインタをメンバーとして宣言

public:
	Training();

	~Training() = default;

	virtual	void Initialize();

	void SetQuestionLength(int length) { questionLength = length; }

//	std::string charactor = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
	std::string charactor = "E";

	int questionLength = 1; // 問題の文字数


	virtual void questionPosing();

	virtual bool checkAnswer(const std::string& answer);


	int GetTotalQuestion() const { return totalQuestion; }
	void SetTotalQuestion(int count) { totalQuestion = count; }

	int GetCorrectNum() const { return correctNum; }

	void ResetCorrectNum() { correctNum = 0; }

	void ResetQuestionCount() { questionCount = 0; }


	int questionCount;	// 現在の問題番号を追跡するメンバー変数
	int totalQuestion;	//問題数
	int correctNum; // 正解数を追跡するメンバー変数
};


class NormalTraining : public Training
{
public:
	NormalTraining() = default;
	~NormalTraining() = default;

	void Initialize() override
	{
		//通常モードの初期化処理
		std::cout << "NormalTraining Initialized" << std::endl;

	}
	void questionPosing() override
	{


		//通常モードの問題出題処理
		std::cout << "NormalTraining Question" << std::endl;

		//通常モードでは問題は出さず、常にデコードを繰り返す

	}
	bool checkAnswer(const std::string& answer) override
	{
		//通常モードの解答判定処理
		std::cout << "NormalTraining Answer" << std::endl;

		//通常モードでは判定せずに、常にデコードを繰り返す
		return true;
	}
};



class CWTraining : public Training
{
	public:
	CWTraining() = default;
	~CWTraining() = default;

	void Initialize() override
	{
		//CWモードの初期化処理
		std::cout << "CWTraining Initialized" << std::endl;
	}
	void questionPosing() override
	{
		//CWモードの問題出題処理
		std::cout << "CWTraining Question" << std::endl;
		//CWモードでは、実際更新をしているようなシナリオに応答する

	}
	bool checkAnswer(const std::string& answer) override
	{
		//CWモードの解答判定処理
		std::cout << "CWTraining Answer" << std::endl;

		std::string trimmed = answer;
		trimmed.erase(trimmed.find_last_not_of(" \t\r\n") + 1);
		std::cout << "Question " << this->questionCount << ": " << charactor
			<< " (Answer: " << trimmed << ")" << std::endl;
		return charactor == trimmed;
	}
};

class OriginalTraining : public Training
{
	public:
	OriginalTraining() = default;
	~OriginalTraining() = default;

	void Initialize() override
	{
		//独自モードの初期化処理
		std::cout << "OriginalTraining Initialized" << std::endl;
	}
	void questionPosing() override
	{
		//独自モードの問題出題処理
		std::cout << "OriginalTraining Question" << std::endl;
		//独自のシナリオで問題を作成

	}
	bool checkAnswer(const std::string& answer) override
	{
		//独自モードの解答判定処理
		std::cout << "OriginalTraining Answer" << std::endl;
		std::string trimmed = answer;
		trimmed.erase(trimmed.find_last_not_of(" \t\r\n") + 1);
		std::cout << "Question " << this->questionCount << ": " << charactor
			<< " (Answer: " << trimmed << ")" << std::endl;
		return charactor == trimmed;
	}
};
