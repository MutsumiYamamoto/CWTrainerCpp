#pragma once

class ScoreManager
{
public:
	ScoreManager()
		: totalScore(0), fullScore(0)
	{

	}

	~ScoreManager() = default;

	void Initialize();

	int GetTotalScore() const { return totalScore; }
	void AddTotalScore(int score) { totalScore += score; }

	int GetFullScore() const { return fullScore; }
	void SetFullScore(int score) { fullScore = score; }



	void ResetTotalScore() { totalScore = 0; }


	void AddScore(int score) { totalScore += score; }

public: // "questionCount" メンバーのアクセス修飾子を "public" に変更
	int totalScore;		// 現在の合計スコアを追跡するメンバー変数


	int fullScore;		//満点

};