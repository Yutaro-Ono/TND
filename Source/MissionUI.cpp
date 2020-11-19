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
{

	// レンダリングするフォントのカラー
	Vector3 color = Vector3(1.0f, 1.0f, 1.0f);
	// フォントサイズ
	int fontSize = 32;
	// 制限時間を文字列として取得
	std::stringstream timestream;
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


		timestream << "TIME:" << m_mission->GetTimeLimit();
		// 制限時間のフォントテクスチャを生成
		m_timeTex[i] = m_font->RenderText(timestream.str(), color * i, fontSize);

		m_distanceTex[i] = nullptr;
	}

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

	// レンダリングするフォントのカラー
	Vector3 color = Vector3(1.0f, 1.0f, 1.0f);
	// フォントサイズ
	int fontSize = 32;

	// 制限時間を文字列として取得
	timestream << "TIME:" << m_mission->GetTimeLimit();
	// 耐久値を文字列として取得
	durablestream << "HP:" << m_mission->GetDurableValue();
	// 距離を文字列として取得
	diststream << (double)m_mission->GetPlayerDistance() / 20.0f << "m";

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
	}

}

// 描画処理
void MissionUI::Draw(Shader* in_shader)
{
	
	// 画像スケール
	float scale = 0.5f;
	// 選択中のミッションだったらスケール拡大
	if (m_mission->GetMissionManager()->GetSelectedMission() == m_mission->GetListNum())
	{
		scale = 1.0f;
	}

	for (int i = 0; i < 2; i++)
	{
		// ミッション概要テクスチャの描画
		DrawTexture(in_shader, m_detailTex[i], m_uiPos + Vector2(0.0f - 3.0f * i, -(30 * m_mission->GetListNum()) + 5.0f * i), scale);

		// 制限時間の描画
		if (m_timeTex[i] != nullptr)
		{
			DrawTexture(in_shader, m_timeTex[i], m_uiPos + Vector2(300.0f - 3.0f * i, -(30 * m_mission->GetListNum()) + 5.0f * i), scale);
		}

		// 耐久値の描画
		if (m_durableValTex[i] != nullptr)
		{

			// 耐久度の描画
			DrawTexture(in_shader, m_durableValTex[i], m_uiPos + Vector2(500.0f - 3.0f * i, -(30 * m_mission->GetListNum()) + 5.0f * i), scale);
		}


		// 距離の描画
		if (m_distanceTex[i] != nullptr)
		{
			// 距離の描画
			DrawTexture(in_shader, m_distanceTex[i], m_uiPos + Vector2(900.0f - 3.0f * i, -(30 * m_mission->GetListNum()) + 5.0f * i), scale);
		}
	}




}


// UI表示座標のセット
void MissionUI::SetUIPosition()
{
	// UI表示座標 (画面の中心 : x = 0.0, 画面上方 : y = 画面の高さ÷5
	Vector2 screenUIPos = Vector2(-500.0f, (GAME_CONFIG->GetScreenHeight() / 3));

	// UI表示位置をスクリーンの幅・高さから設定
	m_uiPos = screenUIPos;
}
