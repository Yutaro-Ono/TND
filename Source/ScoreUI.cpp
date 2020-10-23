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
ScoreUI::ScoreUI()
	:m_scoreTexPos(0.0f, 0.0f)
	,m_fontSize(64)
{
	// スコアの初期化
	m_prevScore = m_nowScore = 0;

	// スコア格納用
	std::stringstream ssScore;

	// ルールのスコアをテキストに変換する
	ssScore << "$" << std::to_string(m_nowScore);

	// スコア用のフォントを生成
	m_scoreTex = m_font->RenderText(ssScore.str(), Vector3(1.0f, 1.0f, 1.0f), m_fontSize);
	// 影を生成
	m_scoreTexShadow = m_font->RenderText(ssScore.str(), Vector3(0.0f, 0.0f, 0.0f), m_fontSize);

	// 表示座標の設定
	m_scoreTexPos = Vector2(GAME_CONFIG->GetScreenWidth() / 2 - m_scoreTex->GetWidth() / 2, RENDERER->GetScreenHeight() / 1.9f - m_scoreTex->GetHeight());


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

	// スコアが変動した時
	if (m_prevScore != m_nowScore)
	{
		// ルールのスコアを取得する
		ssScore << "$" << m_nowScore;

		// 解放処理
		m_scoreTex->Delete();
		m_scoreTexShadow->Delete();


		// 描画するフォントを生成
		m_scoreTex = m_font->RenderText(ssScore.str(), Vector3(1.0f, 1.0f, 1.0f), m_fontSize);
		// 影を生成
		m_scoreTexShadow = m_font->RenderText(ssScore.str(), Vector3(0.0f, 0.0f, 0.0f), m_fontSize);
		// 表示座標の更新
		m_scoreTexPos = Vector2(GAME_CONFIG->GetScreenWidth() / 2 - m_scoreTex->GetWidth() / 2, RENDERER->GetScreenHeight() / 1.9f - m_scoreTex->GetHeight());

		// 同じ値にする
		m_prevScore = m_nowScore;
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


}
