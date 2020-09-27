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

#include <string>
#include <sstream>


// コンストラクタ
ScoreUI::ScoreUI(class RuleScore* in_rule)
	:m_ruleScore(in_rule)
	,m_scoreTexPos(0.0f, 0.0f)
	,m_fontSize(64)
{
	// スコア格納用
	std::string strScore;
	std::stringstream ssScore;

	// ルールのスコアを取得し、テキストに変換する
	strScore = std::to_string(m_ruleScore->GetScore());
	ssScore << strScore;
	strScore = ssScore.str();

	// スコア用のフォントを生成
	m_scoreTex = m_font->RenderText(strScore, Vector3(1.0f, 1.0f, 1.0f), m_fontSize);
	// 影を生成
	m_scoreTexShadow = m_font->RenderText(strScore, Vector3(0.0f, 0.0f, 0.0f), m_fontSize);

	// スコアレート用のフォントを生成
	for (int i = 0; i < 2; i++)
	{
		// 影
		if (i == 0)
		{
			m_scoreRateTex[i] = m_font->RenderText("x1", Vector3(0.0f, 0.0f, 0.0f), m_fontSize / 2);
		}
		// 本体
		if (i == 1)
		{
			m_scoreRateTex[i] = m_font->RenderText("x1", Vector3(1.0f, 1.0f, 1.0f), m_fontSize / 2);
		}
	}

	// 表示座標の設定
	// m_scoreTexPos = Vector2(RENDERER->GetScreenWidth() - m_scoreTex->GetWidth(), m_scoreTex->GetHeight());
	m_scoreTexPos = Vector2(0.0f, -RENDERER->GetScreenHeight() / 1.9f + m_scoreTex->GetHeight());

	// スコアの監視
	m_prevScore = m_ruleScore->GetScore();
	m_nowScore = m_prevScore;
}


// デストラクタ
ScoreUI::~ScoreUI()
{

	if (m_scoreTex)
	{
		m_scoreTex->Delete();
	}


	if (m_scoreTexShadow)
	{
		m_scoreTexShadow->Delete();
	}

}


// 更新処理
void ScoreUI::Update(float in_deltaTime)
{
	// スコア
	std::stringstream ssScore;

	// 毎フレームスコアを監視
	m_nowScore = m_ruleScore->GetScore();

	// スコアが変動した時
	if (m_prevScore != m_nowScore)
	{
		// ルールのスコアを取得する
		ssScore << m_ruleScore->GetScore();


		// 解放処理
		m_scoreTex->Delete();
		m_scoreTexShadow->Delete();


		// 描画するフォントを生成
		m_scoreTex = m_font->RenderText(ssScore.str(), Vector3(1.0f, 1.0f, 1.0f), m_fontSize);
		// 影を生成
		m_scoreTexShadow = m_font->RenderText(ssScore.str(), Vector3(0.0f, 0.0f, 0.0f), m_fontSize);

		// 同じ値にする
		m_prevScore = m_nowScore;
	}


	// スコアレート(チェイン)
	std::stringstream ssRate;
	// ルールのチェインレートを取得する
	ssRate << "x" << m_ruleScore->GetChainRate();

	// レート用フォントを生成
	for (int i = 0; i < 2; i++)
	{
		m_scoreRateTex[i]->Delete();

		if (i == 0)
		{
			m_scoreRateTex[i] = m_font->RenderText(ssRate.str(), Vector3(0.0f, 0.0f, 0.0f), m_fontSize / 2);
		}

		if (i == 1)
		{
			m_scoreRateTex[i] = m_font->RenderText(ssRate.str(), Vector3(1.0f, 1.0f, 1.0f), m_fontSize / 2);
		}

	}

}


// 描画処理
void ScoreUI::Draw(Shader * in_shader)
{

	// スコアの描画
	if (m_scoreTexShadow)
	{
		// 影
		DrawTexture(in_shader, m_scoreTexShadow, Vector2(m_scoreTexPos.x + 3.0f, m_scoreTexPos.y - 3.0f), 1.0f);
	}
	if (m_scoreTex)
	{
		// 本体
		DrawTexture(in_shader, m_scoreTex, m_scoreTexPos, 1.0f);
	}

	for (int i = 0; i < 2; i++)
	{
		if (m_scoreRateTex[i])
		{
			DrawTexture(in_shader, m_scoreRateTex[i], Vector2(m_scoreTex->GetWidth() + m_scoreTexPos.x + 3.0f, m_scoreTexPos.y));
		}
	}


}
