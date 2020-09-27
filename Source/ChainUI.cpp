//-----------------------------------------------------------------------+
// チェインUIクラス.
// 
// 
// copyright (C) 2020 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#include "ChainUI.h"
#include "GameMain.h"
#include "Renderer.h"
#include "Texture.h"
#include "RuleScore.h"
#include "Font.h"
#include "SpriteComponent.h"

#include <sstream>


// コンストラクタ
ChainUI::ChainUI(RuleScore * in_rule)
	:m_rule(in_rule)
	,m_chainNum(0)
	,m_rank(0)
	,m_timeGauge(nullptr)
	,m_gaugeWidth(0)
{

	// フォントテクスチャのロード
	for (int i = 0; i < 2; i++)
	{
		if (i == 0)
		{
			m_chain[i] = m_font->RenderText("0 CHAIN", Vector3(0.0f, 0.0f, 0.0f), 48);
			m_rankTex[i] = m_font->RenderText("RANK", Vector3(0.0f, 0.0f, 0.0f), 30);
			m_rankNum[i] = m_font->RenderText("0", Vector3(0.0f, 0.0f, 0.0f), 72);
		}

		if (i == 1)
		{
			m_chain[i] = m_font->RenderText("0 CHAIN", Vector3(1.0f, 0.3f, 0.0f), 48);
			m_rankTex[i] = m_font->RenderText("RANK", Vector3(1.0f, 1.0f, 1.0f), 30);
			m_rankNum[i] = m_font->RenderText("0", Vector3(1.0f, 1.0f, 1.0f), 72);
		}

	}

	// タイマーゲージのテクスチャ取得
	m_timeGauge = RENDERER->GetTexture("Data/Interface/HUD/TimerGauge/TimerGauge.png");
	m_timeGauge->SetWidth(1024);                   // ゲージのサイズを初期化
	m_gaugeWidth = m_timeGauge->GetWidth();

}


// デストラクタ
ChainUI::~ChainUI()
{
	for (int i = 0; i < 2; i++)
	{
		m_chain[i]->Delete();
		// m_chainRank[i]->Delete();
		// m_rank[i]->Delete();
	}

}


// 更新処理
void ChainUI::Update(float in_deltaTime)
{
	// チェイン数格納
	std::stringstream ssChain;

	// チェイン数をルールクラスから取得
	int nowChain = m_rule->GetChain();

	// チェイン数を文字列として出力
	ssChain << nowChain - 1 << " CHAIN";




	// チェイン数に変更があったら更新
	if (m_chainNum != nowChain)
	{
		// フォントの生成
		for (int i = 0; i < 2; i++)
		{
			// ポインタがいずれかを指していた場合
			if (m_chain[i] != nullptr)
			{
				// フォントの解放処理
				m_chain[i]->Delete();
			}


			// 影
			if (i == 0)
			{
				m_chain[i] = m_font->RenderText(ssChain.str(), Vector3(1.0f, 0.3f, 0.0f), 60);
			}
			
			// 本体
			if (i == 1)
			{
				m_chain[i] = m_font->RenderText(ssChain.str(), Vector3(1.0f, 0.8f, 0.6f), 60);
			}
		}

		// チェイン数更新
		m_chainNum = nowChain;
	}

	//------------------------------------------------------------------------+
	// ランク
	//------------------------------------------------------------------------+
	// ランク数格納
	std::stringstream ssRank;

	int nowRank = m_rule->GetPlayerRank();

	if (m_rank != nowRank)
	{
		ssRank << nowRank;

		for (int i = 0; i < 2; i++)
		{
			m_rankNum[i]->Delete();


			// 影
			if (i == 0)
			{
				m_rankNum[i] = m_font->RenderText(ssRank.str(), Vector3(0.0f, 0.0f, 0.0f), 72);
			}

			// 本体
			if (i == 1)
			{
				m_rankNum[i] = m_font->RenderText(ssRank.str(), Vector3(1.0f, 1.0f, 1.0f), 72);
			}
		}

		m_rank = nowRank;

	}

	// タイムゲージの横幅の更新
	m_timeGauge->SetWidth(m_gaugeWidth * (m_rule->GetChainTimer() / m_rule->GetChainTimerLimit()));

}


// 描画処理
void ChainUI::Draw(Shader * in_shader)
{
	// 影付き描画
	for (int i = 0; i < 2; i++)
	{
		// チェイン
		if (m_chain[i] && (m_chainNum - 1) >= 1)
		{

			for (int i = 0; i < 2; i++)
			{
				// 影
				if (i == 0)
				{
					DrawTexture(in_shader, m_chain[i], Vector2(0.0f, RENDERER->GetScreenHeight() / 4.1f - 3.0f), 1.0f);
				}

				// 本体
				if (i == 1)
				{
					DrawTexture(in_shader, m_chain[i], Vector2(0.0f, RENDERER->GetScreenHeight() / 4.1f), 1.0f);
				}

			}

		}


		// ランク
		// 影
		if (i == 0)
		{
			DrawTexture(in_shader, m_rankTex[i], Vector2(0.0f, -RENDERER->GetScreenHeight() / 4.5f - 3.0f), 1.0f);
		}

		// 本体
		if (i == 1)
		{
			DrawTexture(in_shader, m_rankTex[i], Vector2(0.0f, -RENDERER->GetScreenHeight() / 4.5f), 1.0f);
		}

		// ランク(数字)
		// 影
		if (i == 0)
		{
			DrawTexture(in_shader, m_rankNum[i], Vector2(0.0f, -RENDERER->GetScreenHeight() / 3.6f - 3.0f), 1.0f);
		}

		// 本体
		if (i == 1)
		{
			DrawTexture(in_shader, m_rankNum[i], Vector2(0.0f, -RENDERER->GetScreenHeight() / 3.6f), 1.0f);
		}
	}

	// チェインゲージの描画
	if (m_timeGauge && m_rule->GetChainTimer() > 0.0f && m_rule->GetChain() > 0.0f)
	{
		DrawTexture(in_shader, m_timeGauge, Vector2(0.0f, RENDERER->GetScreenHeight() / 4.5f - 30.0f), 0.7f);
	}

}
