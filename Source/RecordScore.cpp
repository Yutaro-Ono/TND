//-----------------------------------------------------------------------+
// スコアの記録クラス.
// 
// 
// copyright (C) 2020 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#include "RecordScore.h"
#include "RapidJsonHelper.h"

#include <SDL.h>
#include <fstream>
#include <iostream>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/prettywriter.h>


const int RecordScore::RANK_LIMIT = 10;          // 順位の最大値(基本：10位まで)


// コンストラクタ
RecordScore::RecordScore(int in_score)
	:m_score(in_score)
	,m_rank(11)
{

	// 順位ごとにスコア情報を格納
	for (int i = 0; i < RANK_LIMIT; i++)
	{
		// 配列の確保
		m_ranking.push_back(0);

		// 順位配列にデータを格納
		m_ranking[i] = LoadRankingScore("Data/Strings/path/path.json", (i + 1));
	}
	
	// プレイヤースコアも配列に追加
	m_ranking.push_back(m_score);

	// スコアの順序をSort
	std::sort(m_ranking.begin(), m_ranking.end(), std::greater<int>());

	// パス先のファイルにランキングをセーブ
	SaveRecord("Data/Strings/path/path.json", 0, m_ranking[0]);

}


// デストラクタ
RecordScore::~RecordScore()
{
	// 処理なし
}


// ランキング情報のロード
int RecordScore::LoadRankingScore(const std::string& in_fileName, int in_rankNum)
{
	// Jsonファイルをドキュメントにロード
	rapidjson::Document doc;
	if (!OpenJsonFile(doc, in_fileName.c_str()))
	{
		SDL_Log("Failed to Load Score %s", in_fileName.c_str());
		return false;
	}

	std::string rankID;
	// IDを設定
	rankID = std::to_string(in_rankNum);

	// スコア情報を返す
	return OutputInt(doc, rankID.c_str());
}


// ランキング情報の並べ替え(現状不要)
void RecordScore::SortRecord(int& in_score)
{
}


// 記録のセーブ
void RecordScore::SaveRecord(const std::string& in_fileName, const int& in_rank, const int& in_score)
{
	// 記録するID(順位)の項目を作成
	std::string rankID = std::to_string(in_rank);

	// ドキュメント作成
	rapidjson::Document doc;
	doc.SetObject();


	for (int i = 0; i < RANK_LIMIT; i++)
	{
		if (m_ranking[i] == m_score)
		{
			m_rank = (i + 1);
		}


		if (i == 0)
		{
			// スコアを指定した順位に書き込み
			AddInt(doc.GetAllocator(), doc, "1", m_ranking[i]);
		}
		if (i == 1)
		{
			// スコアを指定した順位に書き込み
			AddInt(doc.GetAllocator(), doc, "2", m_ranking[i]);
		}
		if (i == 2)
		{
			// スコアを指定した順位に書き込み
			AddInt(doc.GetAllocator(), doc, "3", m_ranking[i]);
		}
		if (i == 3)
		{
			// スコアを指定した順位に書き込み
			AddInt(doc.GetAllocator(), doc, "4", m_ranking[i]);
		}
		if (i == 4)
		{
			// スコアを指定した順位に書き込み
			AddInt(doc.GetAllocator(), doc, "5", m_ranking[i]);
		}
		if (i == 5)
		{
			// スコアを指定した順位に書き込み
			AddInt(doc.GetAllocator(), doc, "6", m_ranking[i]);
		}
		if (i == 6)
		{
			// スコアを指定した順位に書き込み
			AddInt(doc.GetAllocator(), doc, "7", m_ranking[i]);
		}
		if (i == 7)
		{
			// スコアを指定した順位に書き込み
			AddInt(doc.GetAllocator(), doc, "8", m_ranking[i]);
		}
		if (i == 8)
		{
			// スコアを指定した順位に書き込み
			AddInt(doc.GetAllocator(), doc, "9", m_ranking[i]);
		}
		if (i == 9)
		{
			// スコアを指定した順位に書き込み
			AddInt(doc.GetAllocator(), doc, "10", m_ranking[i]);
		}

		// スコアを指定した順位に書き込み
		// AddInt(doc.GetAllocator(), doc, str, m_ranking[i]);

	}

	// Jsonをstringバッファに書き込み
	rapidjson::StringBuffer buffer;

	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
	doc.Accept(writer);
	const char* output = buffer.GetString();

	// 外部ファイルに書き出し
	std::ofstream outFile(in_fileName);

	if (outFile.is_open())
	{
		outFile << output;
	}
}

// ランキング情報のゲッター
const int & RecordScore::GetRankingScore(const int & in_num) const
{
	return m_ranking[in_num];
}
