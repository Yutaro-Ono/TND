//-----------------------------------------------------------------------+
// スコアの記録クラス.
// 
// 
// copyright (C) 2020 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#pragma once
#include "ResultScene.h"
#include "RapidJsonHelper.h"

#include <string>
#include <vector>
#include <rapidjson/document.h>
#include "Math.h"


class RecordScore
{


public:

	RecordScore(int in_score);                                  // コンストラクタ
	~RecordScore();                                             // デストラクタ


	int LoadRankingScore(const std::string& in_fileName, int in_rankNum);

	void SortRecord(int& in_score);

	void SaveRecord(const std::string& in_fileName, const int& in_rank, const int& in_score);

	const int& GetRank() const { return m_rank; }

	const int& GetRankingScore(const int& in_num) const;     // 順位ごとのスコアのゲッター


private:

	int m_score;

	int m_rank;

	int m_rankingScore[10];
	std::vector<int> m_ranking;

	static const int RANK_LIMIT;


};