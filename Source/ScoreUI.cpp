//-----------------------------------------------------------------------+
// スコアUIクラス.
// 
// 
// copyright (C) 2020 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#include "ScoreUI.h"
#include "GameMain.h"
#include "Renderer.h"
#include "Texture.h"
#include "Font.h"
#include "RuleScore.h"
#include "Canvas.h"
#include "GameWorld.h"
#include <string>
#include <sstream>


// コンストラクタ
ScoreUI::ScoreUI(Canvas* in_canvas)
	:m_canvas(in_canvas)
	,m_scoreTexPos(0.0f, 0.0f)
	,m_fontSize(64)
	,m_score(0)
	,m_addScore(0)
	,m_drawSubScore(false)
	,m_coolTime(2)
	,m_startSub(false)
{

	m_score = 500;
	// スコア格納用
	std::stringstream ssScore;
	std::stringstream ssSub;
	// ルールのスコアをテキストに変換する
	ssScore << "$" << std::to_string(m_score);
	ssSub << "-$" << "50";
	// スコア用のフォントを生成 (0 = 影, 1 = 本体)
	for (int i = 0; i < 2; i++)
	{
		m_scoreTex[i] = m_font->RenderText(ssScore.str(), Vector3(1.0f, 1.0f, 1.0f) * i, m_fontSize);
		m_addScoreTex[i] = m_font->RenderText(ssScore.str(), Vector3(1.0f, 1.0f, 1.0f) * i, m_fontSize);
		m_subScoreTex[i] = m_font->RenderText(ssSub.str(), Vector3(0.0f, 0.0f, 1.0f) * i, m_fontSize);
	}
	// 表示座標の設定
	m_scoreTexPos = Vector2(SCREEN_HALF_W - m_scoreTex[0]->GetHalfWidth() -3.0f, SCREEN_HALF_H - m_scoreTex[0]->GetHalfHeight() - 3.0f);
	m_addScoreTexPos = Vector2(SCREEN_HALF_W - m_addScoreTex[0]->GetHalfWidth() - 3.0f, m_scoreTexPos.y - m_addScoreTex[0]->GetHeight() - 3.0f);
	m_subScoreTexPos = Vector2(SCREEN_HALF_W - m_subScoreTex[0]->GetHalfWidth() - 3.0f, m_scoreTexPos.y - m_subScoreTex[0]->GetHeight() - 3.0f);
	// 時間を秒単位で取得
	m_time = SDL_GetTicks() / 1000;

}


// デストラクタ
ScoreUI::~ScoreUI()
{

	if (m_scoreTex)
	{
		m_scoreTex[0]->Delete();
		m_scoreTex[1]->Delete();
	}

}


// 更新処理
void ScoreUI::Update(float in_deltaTime)
{
	// スコア
	std::stringstream ssScore;

	// スコアが加算された時
	if (m_addScore > 0)
	{
		AddScore();     // スコアの加算処理

		// ルールのスコアを取得する
		ssScore << "$" << m_score;
		// スコア更新に伴う事前のテクスチャ削除と生成処理
		for (int i = 0; i < 2; i++)
		{
			m_scoreTex[i]->Delete();
			m_scoreTex[i] = m_font->RenderText(ssScore.str(), Vector3(1.0f, 1.0f, 1.0f) * i, m_fontSize);
		}
		// 表示座標の更新
		m_scoreTexPos = Vector2(SCREEN_HALF_W - m_scoreTex[0]->GetHalfWidth() - 3.0f, SCREEN_HALF_H - m_scoreTex[0]->GetHalfHeight() - 3.0f);

		// テクスチャを生成
		std::stringstream ssadd;
		ssadd << "+$" << m_addScore;
		// スコア更新に伴う事前のテクスチャ削除と生成処理
		for (int i = 0; i < 2; i++)
		{
			m_addScoreTex[i]->Delete();
			m_addScoreTex[i] = m_font->RenderText(ssadd.str(), Vector3(0.0f, 1.0f, 0.7f) * i, 48);
		}

		m_addScoreTexPos = Vector2(SCREEN_HALF_W - m_addScoreTex[0]->GetHalfWidth() - 3.0f, m_scoreTexPos.y - m_addScoreTex[0]->GetHeight() - 3.0f);
	}

	// スコア減算処理
	// プレイヤーが見つかっている時のみ
	m_startSub = m_canvas->GetWorld()->GetFoundPlayer();
	if (m_startSub)
	{
		SubScore();

		// ルールのスコアを取得する
		ssScore << "$" << m_score;
		// スコア更新に伴う事前のテクスチャ削除と生成処理
		for (int i = 0; i < 2; i++)
		{
			m_scoreTex[i]->Delete();
			m_scoreTex[i] = m_font->RenderText(ssScore.str(), Vector3(1.0f, 1.0f, 1.0f) * i, m_fontSize);
		}
		// 表示座標の更新
		m_scoreTexPos = Vector2(SCREEN_HALF_W - m_scoreTex[0]->GetHalfWidth() - 3.0f, SCREEN_HALF_H - m_scoreTex[0]->GetHalfHeight() - 3.0f);
	}
	else
	{
		m_drawSubScore = false;
		m_time = SDL_GetTicks() / 1000;
	}
	
}


// 描画処理
void ScoreUI::Draw(Shader * in_shader)
{

	// スコアの描画 (0 = 影, 1 = 本体)
	for (int i = 0; i < 2; i++)
	{
		// 現在のスコア
		DrawTexture(in_shader, m_scoreTex[i], m_scoreTexPos + Vector2(3.0f, 3.0f) * i, 1.0f);
	}

	// 加算分スコアの描画 (加算処理時のみ)
	if (m_addScore > 0)
	{
		for (int i = 0; i < 2; i++)
		{
			// 現在のスコア
			DrawTexture(in_shader, m_addScoreTex[i], m_addScoreTexPos + Vector2(3.0f, 3.0f) * i, 1.0f);
		}
	}
	// 減算分のスコア
	else if (m_addScore == 0 && m_drawSubScore)
	{
		for (int i = 0; i < 2; i++)
		{
			// 現在のスコア
			DrawTexture(in_shader, m_subScoreTex[i], m_subScoreTexPos + Vector2(3.0f, 3.0f) * i, 1.0f);
		}
	}
}

// 加算用スコアをスコアに追加する(+10ずつ)
void ScoreUI::AddScore()
{
	// スコアを10ずつ加算 (追加分が10を下回っていたら、そのままスコアに加算し0に)
	if (m_addScore >= 10)
	{
		m_score += 10;
		m_addScore -= 10;
	}
	else
	{
		m_score += m_addScore;
		m_addScore = 0;
	}

	// 加算分は0を下回らない
	if (m_addScore < 0)
	{
		m_addScore = 0;
	}
}

void ScoreUI::SubScore()
{
	// 3秒ごとに減算
	if (SDL_GetTicks() / 1000 > m_time + m_coolTime)
	{
		m_score -= 50;
		m_drawSubScore = true;
		m_time = SDL_GetTicks() / 1000;
	}

	if (SDL_GetTicks() / 1000 > m_time + 1)
	{
		m_drawSubScore = false;
	}

	// スコアは0を下回らない
	if (m_score < 0)
	{
		m_score = 0;
	}
}

// 加算分スコアのセットとそれに伴うテクスチャ生成
void ScoreUI::SetAddScore(int in_score)
{
	// 加算分を追加
	m_addScore += in_score;
	// テクスチャを生成
	std::stringstream ssadd;
	ssadd << "+$" << in_score;
	// スコア更新に伴う事前のテクスチャ削除と生成処理
	for (int i = 0; i < 2; i++)
	{
		m_addScoreTex[i]->Delete();
		m_addScoreTex[i] = m_font->RenderText(ssadd.str(), Vector3(0.0f, 1.0f, 0.7f) * i, 48);
	}

	m_addScoreTexPos = Vector2(SCREEN_HALF_W - m_addScoreTex[0]->GetHalfWidth() - 3.0f, m_scoreTexPos.y - m_addScoreTex[0]->GetHeight() - 3.0f);
}
