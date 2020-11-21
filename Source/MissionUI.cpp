#include "MissionUI.h"
#include "MissionBase.h"
#include "MissionManager.h"
#include "Font.h"
#include "Texture.h"
#include <iostream>
#include <sstream>

// コンストラクタ
MissionUI::MissionUI(MissionBase* in_mission)
	:m_mission(in_mission)
	,m_uiPos(Vector2::Zero)
	,m_windowTex(nullptr)
	,m_generalWindow(nullptr)
	,m_pointTex(nullptr)
	,m_anim(0.0f)
{

	// レンダリングするフォントのカラー
	Vector3 color = Vector3(1.0f, 1.0f, 1.0f);
	// フォントサイズ
	int fontSize = 32;
	// 制限時間・スコアを文字列として取得
	std::stringstream timestream;
	std::stringstream scoreStream;
	//------------------------------------------------------------------+
	// ミッション概要フォントの生成
	//------------------------------------------------------------------+

	for (int i = 0; i < 2; i++)
	{
		if (m_mission->GetMissionType() == MissionBase::DELIVERY)
		{
			// ミッション概要テクスチャの生成
			m_detailTex[i] = m_font->RenderText("DELIVERY", color * i, fontSize);
		}

		if (m_mission->GetMissionType() == MissionBase::TAXI)
		{
			// ミッション概要テクスチャの生成
			m_detailTex[i] = m_font->RenderText("TAXI", color * i, fontSize);
		}

		// 耐久値のフォントテクスチャを生成
		m_durableValTex[i] = m_font->RenderText("100%", color * i, fontSize);

		timestream << m_mission->GetTimeLimit();
		// 制限時間のフォントテクスチャを生成
		m_timeTex[i] = m_font->RenderText(timestream.str(), color * i, fontSize);

		// スコア用フォントテクスチャ生成
		scoreStream << "$" << "0";
		m_scoreTex[i] = m_font->RenderText(scoreStream.str(), color * i, fontSize);

		m_distanceTex[i] = nullptr;

		if (m_mission->GetListNum() == 0)
		{
			m_missionID[i] = m_font->RenderText("A", color * i, fontSize);
		}
		else if (m_mission->GetListNum() == 1)
		{
			m_missionID[i] = m_font->RenderText("B", color * i, fontSize);
		}
		else
		{
			m_missionID[i] = m_font->RenderText("C", color * i, fontSize);
		}
	}



	// 背景ウィンドウテクスチャ
	m_windowTex = RENDERER->GetTexture("Data/Interface/TND/Control/Mission_Index.png");
	// 配達依頼テクスチャ (リストAのみ)
	m_generalWindow = RENDERER->GetTexture("Data/Interface/TND/Control/Mission_General.png");
	// 選択ポイントテクスチャ
	m_pointTex = RENDERER->GetTexture("Data/Interface/TND/Control/Mission_CursolDot.png");
}

// デストラクタ
MissionUI::~MissionUI()
{
	Close();
}

// 更新処理
void MissionUI::Update(float in_deltaTime)
{


	// ミッション終了でUIを閉じる
	if (m_mission->GetMissionState() == MissionBase::MISSION_STATE::FAILED ||
		m_mission->GetMissionState() == MissionBase::MISSION_STATE::SUCCESS)
	{
		m_state = CLOSE;
	}

	// ミッション情報更新
	UpdateMissionInfo();

}

// ミッション情報の生成・更新処理
void MissionUI::UpdateMissionInfo()
{
	// 耐久値、時間などの取得用
	std::string str;
	std::stringstream timestream;
	std::stringstream durablestream;
	std::stringstream diststream;
	std::stringstream scoreStream;


	// レンダリングするフォントのカラー
	Vector3 color = Vector3(1.0f, 1.0f, 1.0f);
	// フォントサイズ
	int fontSize = 32;

	// 制限時間を文字列として取得
	timestream << m_mission->GetTimeLimit();
	// 耐久値を文字列として取得
	durablestream << "HP:" << m_mission->GetDurableValue();
	// 距離を文字列として取得
	diststream << (int)m_mission->GetPlayerDistance() / 20 << "m";
	// スコアを文字列として取得
	int score = m_mission->GetBaseScore();
	Vector3 scoreColor = Vector3(1.0f, 1.0f, 1.0f);
	if (score >= 2000)
	{
		scoreColor = Vector3(0.5f, 0.6f, 1.0f);
	}
	scoreStream << "$" << score;

	//-----------------------------------------------------------------------+
    // フォントテクスチャ生成処理
    //-----------------------------------------------------------------------+
	for (int i = 0; i < 2; i++)
	{
		//-------------------------------------------------+
		// 制限時間
		//-------------------------------------------------+
		// 制限時間のテクスチャを削除
		if (m_timeTex[i] != nullptr) m_timeTex[i]->Delete();
		// 制限時間のフォントテクスチャを生成
		m_timeTex[i] = m_font->RenderText(timestream.str(), color * i, fontSize);

		//-------------------------------------------------+
		// 耐久値
		//-------------------------------------------------+
		// 耐久値のテクスチャを削除
		if (m_durableValTex[i] != nullptr) m_durableValTex[i]->Delete();
		// 耐久値のフォントテクスチャを生成
		m_durableValTex[i] = m_font->RenderText(durablestream.str(), color * i, fontSize);

		//--------------------------------------------------+
		// 距離
		//--------------------------------------------------+
		// 距離のテクスチャを削除
		if (m_distanceTex[i] != nullptr) m_distanceTex[i]->Delete();
		// 距離のフォントテクスチャを生成
		m_distanceTex[i] = m_font->RenderText(diststream.str(), color * i, fontSize);

		//--------------------------------------------------+
		// スコア
		//--------------------------------------------------+
		// スコア用フォントテクスチャ生成
		if (m_scoreTex[i] != nullptr) m_scoreTex[i]->Delete();
		m_scoreTex[i] = m_font->RenderText(scoreStream.str(), scoreColor * i, fontSize);

		//--------------------------------------------------+
        // ミッションID
        //--------------------------------------------------+
		if (m_missionID != nullptr) m_missionID[i]->Delete();
		if (m_mission->GetListNum() == 0)
		{
			m_missionID[i] = m_font->RenderText("A", color * i, fontSize);
		}
		else if (m_mission->GetListNum() == 1)
		{
			m_missionID[i] = m_font->RenderText("B", color * i, fontSize);
		}
		else
		{
			m_missionID[i] = m_font->RenderText("C", color * i, fontSize);
		}
	}

}

// 描画処理
void MissionUI::Draw(Shader* in_shader)
{
	glEnable(GL_BLEND);
	// 画像スケール
	float scale = 0.5f;
	// 選択時のスクリーン座標調整
	Vector2 adjustPos = m_uiPos + Vector2(0.0f, -m_windowTex->GetHalfHeight() / 3 * (m_mission->GetListNum() + 1)) * scale;

	
	// 選択中のミッションだったらUIウィンドウを強調する
	// 強調するためにラープ処理を用いて座標をスクリーン左側へ移動する (選択中じゃない場合もラープ処理で戻す)
	if (m_mission->GetMissionManager()->GetSelectedMission() == m_mission->GetListNum())
	{
		m_anim += 10.0f * GAME_INSTANCE.GetDeltaTime();

		if (m_anim >= 1.0f)
		{
			m_anim = 1.0f;
		}

		adjustPos = Vector2::Lerp(adjustPos, m_uiPos + Vector2(-m_windowTex->GetHalfWidth(), -m_windowTex->GetHalfHeight() / 3 * (m_mission->GetListNum() + 1)) * scale, 1.0f * m_anim);
	}
	else
	{
		if (m_anim > 0.0f)
		{
			m_anim -= 10.0f * GAME_INSTANCE.GetDeltaTime();
		}
		else
		{
			m_anim = 0.0f;
		}

		adjustPos = Vector2::Lerp(adjustPos, m_uiPos + Vector2(-m_windowTex->GetHalfWidth(), -m_windowTex->GetHalfHeight() / 3 * (m_mission->GetListNum() + 1)) * scale, 1.0f * m_anim);
	}
	

	// 背景ウィンドウ描画
	if (m_windowTex != nullptr)
	{
		DrawTexture(in_shader, m_windowTex, adjustPos, scale);
	}


	for (int i = 0; i < 2; i++)
	{
		// ミッション概要テクスチャの描画
		//DrawTexture(in_shader, m_detailTex[i], m_uiPos + Vector2(-3.0f * i, -(30 * m_mission->GetListNum()) + 5.0f * i), scale);

		// 制限時間の描画
		if (m_timeTex[i] != nullptr)
		{
			DrawTexture(in_shader, m_timeTex[i], adjustPos + Vector2(m_windowTex->GetWidth() / 3 + m_timeTex[i]->GetHalfWidth(), -5.0f * i) * scale, 1.0f);
		}

		// 耐久値の描画
		if (m_durableValTex[i] != nullptr)
		{

			// 耐久度の描画
			//DrawTexture(in_shader, m_durableValTex[i], m_uiPos + Vector2(500.0f - 3.0f * i, scale);
		}


		// 距離の描画
		if (m_distanceTex[i] != nullptr)
		{
			DrawTexture(in_shader, m_distanceTex[i], adjustPos - Vector2(m_windowTex->GetWidth() / 2 + m_distanceTex[i]->GetWidth(), m_distanceTex[i]->GetWidth() / 3 + -5.0f * i) * scale, 1.0f);
		}

		// スコアの描画
		if (m_scoreTex[i] != nullptr)
		{
			DrawTexture(in_shader, m_scoreTex[i], adjustPos - Vector2(m_windowTex->GetHalfWidth() / 2, -5.0f * i) * scale, 1.0f);
		}

		// ミッションID描画
		DrawTexture(in_shader, m_missionID[i], adjustPos + Vector2(-m_windowTex->GetHalfWidth() + m_missionID[i]->GetHalfWidth() * 4, -5.0f * i) * scale, 1.0f);
	}

	// このミッションが選択されている時のみ、選択ポイントを描画
	if (m_mission->GetMissionManager()->GetSelectedMission() == m_mission->GetListNum())
	{
		DrawTexture(in_shader, m_pointTex, adjustPos + Vector2(-m_windowTex->GetHalfWidth() * 1.1, m_pointTex->GetHeight() * 0.04f) * scale, 0.04f);
	}

	// 「配達依頼」
	if (m_mission->GetListNum() == 1)
	{
		DrawTexture(in_shader, m_generalWindow, m_uiPos + Vector2(0.0f, -m_windowTex->GetHalfHeight() / 3 * (m_mission->GetListNum() + 1)) * scale + Vector2(-m_generalWindow->GetHalfWidth(), m_generalWindow->GetHeight() * 2.0 * 0.275f) * 0.3f, 0.3f);
	}
	

}


// UI表示座標のセット
void MissionUI::SetUIPosition()
{
	// UI表示座標 (画面の中心 : x = 0.0, 画面上方 : y = 0)
	Vector2 screenUIPos = Vector2(GAME_CONFIG->GetScreenWidth() / 2, -GAME_CONFIG->GetScreenHeight() / 20);

	// UI表示位置をスクリーンの幅・高さから設定
	m_uiPos = screenUIPos;
}
