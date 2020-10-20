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
	,m_timeTex(nullptr)
	,m_detailTex(nullptr)
	,m_distanceTex(nullptr)
	,m_uiPos(Vector2::Zero)
{

	// レンダリングするフォントのカラー
	Vector3 color = Vector3(1.0f, 1.0f, 1.0f);
	// フォントサイズ
	float fontSize = 32.0f;
	//------------------------------------------------------------------+
	// ミッション概要フォントの生成
	//------------------------------------------------------------------+
	if (m_mission->GetMissionType() == MissionBase::DELIVERY)
	{
		// ミッション概要テクスチャの生成
		m_detailTex = m_font->RenderText("DELIVERY", color, fontSize);
	}

	if (m_mission->GetMissionType() == MissionBase::TAXI)
	{
		// ミッション概要テクスチャの生成
		m_detailTex = m_font->RenderText("TAXI", color, fontSize);
	}

	// 耐久値のフォントテクスチャを生成
	m_durableValTex = m_font->RenderText("100%", color, fontSize);
}

// デストラクタ
MissionUI::~MissionUI()
{
}

// 更新処理
void MissionUI::Update(float in_deltaTime)
{

	// 耐久値、時間などの取得用
	std::string str;
	std::stringstream sstream;
	// レンダリングするフォントのカラー
	Vector3 color = Vector3(1.0f, 1.0f, 1.0f);
	// フォントサイズ
	float fontSize = 32.0f;

	// ミッション終了でUIを閉じる
	if (m_mission->GetMissionState() == MissionBase::MISSION_STATE::FAILED ||
		m_mission->GetMissionState() == MissionBase::MISSION_STATE::SUCCESS)
	{
		m_state = CLOSE;
	}

	//-----------------------------------------------------------------------+
	// フォントテクスチャ生成処理
	//-----------------------------------------------------------------------+
	// 配達ミッション時
	if (m_mission->GetMissionType() == MissionBase::DELIVERY)
	{
		
		//-------------------------------------------------+
		// 制限時間
		//-------------------------------------------------+
		// 制限時間を文字列として取得
		sstream << "TIME:" << m_mission->GetTimeLimit();
		// 制限時間のテクスチャを削除
		if (m_timeTex != nullptr) m_timeTex->Delete();
		// 制限時間のフォントテクスチャを生成
		m_timeTex = m_font->RenderText(sstream.str(), color, fontSize);
		sstream.str("");
		//-------------------------------------------------+
		// 耐久値
		//-------------------------------------------------+
		// 耐久値を文字列として取得
		sstream << m_mission->GetDurableValue() << "%";
		// 耐久値のテクスチャを削除
		if (m_durableValTex != nullptr) m_durableValTex->Delete();
		// 耐久値のフォントテクスチャを生成
		m_durableValTex = m_font->RenderText(sstream.str(), color, fontSize);
		sstream.str("");

		//--------------------------------------------------+
		// 距離
		//--------------------------------------------------+
		// 距離を文字列として取得
		sstream << m_mission->GetPlayerDistance() / 100.0f << "m";
		// 距離のテクスチャを削除
		if (m_distanceTex != nullptr) m_distanceTex->Delete();
		// 距離のフォントテクスチャを生成
		m_distanceTex = m_font->RenderText(sstream.str(), color, fontSize);
		sstream.str("");
	}

	// タクシーミッション時
	else if (m_mission->GetMissionType() == MissionBase::TAXI)
	{
		//--------------------------------------------------+
		// 制限時間
		//--------------------------------------------------+
		// 制限時間を文字列として取得
		sstream << "TIME:" << m_mission->GetTimeLimit();
		// 制限時間のテクスチャを削除
		if (m_timeTex != nullptr) m_timeTex->Delete();
		// 制限時間のフォントテクスチャを生成
		m_timeTex = m_font->RenderText(sstream.str(), color, fontSize);
		sstream.str("");
		
		//-------------------------------------------------+
        // 耐久値
        //-------------------------------------------------+
        // 耐久値を文字列として取得
		sstream << m_mission->GetDurableValue() << "%";
		// 耐久値のテクスチャを削除
		if (m_durableValTex != nullptr) m_durableValTex->Delete();
		// 耐久値のフォントテクスチャを生成
		m_durableValTex = m_font->RenderText(sstream.str(), color, fontSize);
		sstream.str("");

		//--------------------------------------------------+
		// 距離
		//--------------------------------------------------+
		// 距離を文字列として取得
		sstream << m_mission->GetPlayerDistance() / 100.0f << "m";
		// 距離のテクスチャを削除
		if (m_distanceTex != nullptr) m_distanceTex->Delete();
		// 距離のフォントテクスチャを生成
		m_distanceTex = m_font->RenderText(sstream.str(), color, fontSize);
		sstream.str("");
	}

}

// 描画処理
void MissionUI::Draw(Shader* in_shader)
{
	// 画像スケール
	float scale = 1.0f;

	// ミッション概要テクスチャの描画
	DrawTexture(in_shader, m_detailTex, m_uiPos, scale);

	// 制限時間の描画
	if (m_timeTex != nullptr)
	{
		DrawTexture(in_shader, m_timeTex, m_uiPos + Vector2(200.0f, +(30 * m_mission->GetListNum())), scale);
	}

	// 耐久値の描画
	if (m_durableValTex != nullptr)
	{
	
		// 耐久度の描画
		DrawTexture(in_shader, m_durableValTex, m_uiPos + Vector2(500.0f, +(30 * m_mission->GetListNum())), scale);
	}


	// 距離の描画
	if (m_distanceTex != nullptr)
	{
		// 距離の描画
		DrawTexture(in_shader, m_distanceTex, m_uiPos + Vector2(900.0f, +(30 * m_mission->GetListNum())), scale);
	}


}


// UI表示座標のセット
void MissionUI::SetUIPosition()
{
	// UI表示座標 (画面の中心 : x = 0.0, 画面上方 : y = 画面の高さ÷5
	Vector2 screenUIPos = Vector2(-600.0f, (GAME_CONFIG->GetScreenHeight() / 5));

	// UI表示位置をスクリーンの幅・高さから設定
	m_uiPos = screenUIPos;
}
