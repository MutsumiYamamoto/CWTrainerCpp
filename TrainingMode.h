#pragma once

#include <iostream>
#include <random>

#include "ScoreManager.h"
#include "ScenarioManager.h"


class Training
{
protected:
	ScoreManager* score; // ← 追加: Score型のポインタをメンバーとして宣言

public:
	Training();

	~Training() = default;

	void SetQuestionLength(int length) { questionLength = length; }

//	std::string charactor = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
	std::string charactor = "E";

	int questionLength = 1; // 問題の文字数

	bool piontCalculate = false;	//出題時にポイント計算を行うかどうかのフラグ

	virtual	void Initialize();

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




class RandomTrainingMode : public Training
{
private:
	std::string candidates =
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

	std::string currentQuestion;    // 今回の問題

	std::mt19937 mt{ std::random_device{}() };

public:
	RandomTrainingMode() = default;
	~RandomTrainingMode() = default;

	void Initialize() override;
	void questionPosing() override;
	bool checkAnswer(const std::string& answer) override;
};


class NormalTrainingMode : public Training
{
public:
	NormalTrainingMode() = default;
	~NormalTrainingMode() = default;

	void Initialize() override
	{
		//通常モードの初期化処理
		std::cout << "NormalTraining Initialized" << std::endl;

	}
#if 0
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
#endif

};



class CWTrainingMode : public Training
{
	public:
	CWTrainingMode() = default;
	~CWTrainingMode() = default;

	void Initialize() override;
	void questionPosing() override;
	bool checkAnswer(const std::string& answer) override;
};


class OriginalTrainingMode : public Training
{
private:
	ScenarioManager* scenarioManager;
	std::string currentQuestion;

public:
	OriginalTrainingMode() = default;
	~OriginalTrainingMode() = default;

	void Initialize() override;
	void questionPosing() override;
	bool checkAnswer(const std::string& answer) override;
};
