#pragma once
#include <iostream>
#include <random>


class Training
{
public:
	Training()
		: questionCount(0)
		, totalScore(0)
		, correctNum(0)
		, fullScore(0)
		, totalQuestion(0)
	{}
	~Training() = default;


	virtual	void Initialize();

	int GetTotalScore() const { return totalScore; }
	void AddTotalScore(int score) { totalScore += score; }

	int GetFullScore() const { return fullScore; }
	int GetTotalQuestion() const { return totalQuestion; }

	void SetFullScore(int score) { fullScore = score; }
	void SetTotalQuestion(int count) { totalQuestion = count; }

	int GetCorrectNum() const { return correctNum; }

	void ResetQuestionCount() { questionCount = 0; }
	void ResetTotalScore() { totalScore = 0; }
	void ResetCorrectNum() { correctNum = 0; }

//	std::string charactor = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
	std::string charactor = "E";

	int questionLength = 1; // 問題の文字数


	virtual void questionPosing()
	{
		//問題出題処理
		questionCount++;
		totalQuestion = questionCount;

		//charactorにランダムに１文字を入れる
		static std::mt19937 mt{ std::random_device{}() };
		std::uniform_int_distribution<> dist('A', 'Z');

		charactor = std::string(1, static_cast<char>(dist(mt)));

		std::cout << "Question " << questionCount << ": " << charactor << std::endl;
	}

	virtual bool checkAnswer(const std::string& answer)
	{
		//解答判定処理
//		std::cout << "charactor=[" << charactor << "]" << std::endl;
//		std::cout << "answer=[" << answer << "]" << std::endl

		std::string trimmed = answer;
		trimmed.erase(trimmed.find_last_not_of(" \t\r\n") + 1);
		std::cout << "Question " << questionCount << ": " << charactor
			<< " (Answer: " << trimmed << ")" << std::endl;
		return charactor == trimmed;
	}


	void AddScore(int score) { totalScore += score; }

public: // "questionCount" メンバーのアクセス修飾子を "public" に変更
	int questionCount;	// 現在の問題番号を追跡するメンバー変数
	int totalScore;		// 現在の合計スコアを追跡するメンバー変数
	int correctNum; // 正解数を追跡するメンバー変数

protected:
private:
	int fullScore;		//満点
	int totalQuestion;	//問題数



    

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
		questionCount++;
		totalQuestion = questionCount;
		
		//通常モードの問題出題処理
		std::cout << "NormalTraining Question" << std::endl;
		//charactorにランダムに１文字を入れる
		std::cout << "Question " << questionCount << ": " << charactor << std::endl;

	}
	bool checkAnswer(const std::string& answer) override
	{
		//通常モードの解答判定処理
		std::cout << "NormalTraining Answer" << std::endl;

//		std::cout << "charactor=[" << charactor << "]" << std::endl;
//		std::cout << "answer=[" << answer << "]" << std::endl

		std::string trimmed = answer;
		trimmed.erase(trimmed.find_last_not_of(" \t\r\n") + 1);
		std::cout << "Question " << questionCount << ": " << charactor
			<< " (Answer: " << trimmed << ")" << std::endl;
		return charactor == trimmed;
	}
};


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
		questionCount++;
		totalQuestion = questionCount;

		// 1～5文字のランダム長
		std::uniform_int_distribution<> lenDist(1, 2);
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

		std::cout << "Question "
			<< questionCount
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

		std::cout << "Question " << questionCount << ": " << currentQuestion
			<< " (Answer: " << trimmed << ")" << std::endl;
		return currentQuestion == trimmed;
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
		//charactorにランダムに１文字を入れる
		std::cout << "Question " << questionCount << ": " << charactor << std::endl;

	}
	bool checkAnswer(const std::string& answer) override
	{
		//CWモードの解答判定処理
		std::cout << "CWTraining Answer" << std::endl;
		std::string trimmed = answer;
		trimmed.erase(trimmed.find_last_not_of(" \t\r\n") + 1);
		std::cout << "Question " << questionCount << ": " << charactor
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
		//charactorにランダムに１文字を入れる
		std::cout << "Question " << questionCount << ": " << charactor << std::endl;

	}
	bool checkAnswer(const std::string& answer) override
	{
		//独自モードの解答判定処理
		std::cout << "OriginalTraining Answer" << std::endl;
		std::string trimmed = answer;
		trimmed.erase(trimmed.find_last_not_of(" \t\r\n") + 1);
		std::cout << "Question " << questionCount << ": " << charactor
			<< " (Answer: " << trimmed << ")" << std::endl;
		return charactor == trimmed;
	}
};
