#pragma once
#include <iostream>



class Training
{
public:
	Training() = default;
	~Training() = default;


	virtual	void Initialize();

	int GetTotalScore() const { return totalScore; }

	void AddTotalScore(int score) { totalScore += score; }


	virtual void questionPosing()
	{
		//問題出題処理
	}

	virtual bool checkAnswer()
	{
		//解答判定処理
		return true;
	}

private:

	int fullScore = 100;
	int questionCount = 10;


	int totalScore;

    

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
	}
	bool checkAnswer() override
	{
		//通常モードの解答判定処理
		std::cout << "NormalTraining Answer" << std::endl;
		return true;
	}
};


class RandomTraining : public Training
{
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
		//ランダムモードの問題出題処理
		std::cout << "RandomTraining Question" << std::endl;
	}
	bool checkAnswer() override
	{
		//ランダムモードの解答判定処理
		std::cout << "RandomTraining Answer" << std::endl;
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
	}
	bool checkAnswer() override
	{
		//CWモードの解答判定処理
		std::cout << "CWTraining Answer" << std::endl;
		return true;
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
	}
	bool checkAnswer() override
	{
		//独自モードの解答判定処理
		std::cout << "OriginalTraining Answer" << std::endl;
		return true;
	}
};