#include "MissionUI.h"
#include "MissionBase.h"
#include "MissionManager.h"
#include "Font.h"
#include "Texture.h"
#include <iostream>

// コンストラクタ
MissionUI::MissionUI(MissionBase* in_mission)
	:m_mission(in_mission)
	,m_timeTex(nullptr)
	,m_detailTex(nullptr)
	,m_distanceTex(nullptr)
	,m_durableValTex(nullptr)
	,m_uiPos(Vector2::Zero)
{

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
		// 耐久値を文字列として取得
		str = m_mission->GetDurableValue();

		// 耐久値のテクスチャを削除
		if (m_durableValTex != nullptr) m_durableValTex->Delete();

		// 耐久値のフォントテクスチャを生成
		m_durableValTex = m_font->RenderText(str, color, fontSize);

	}

	// タクシーミッション時
	else if (m_mission->GetMissionType() == MissionBase::TAXI)
	{
		// 制限時間を文字列として取得
		str = m_mission->GetTimeLimit();

		// 耐久値のテクスチャを削除
		if (m_timeTex != nullptr) m_timeTex->Delete();

		// 耐久値のフォントテクスチャを生成
		m_timeTex = m_font->RenderText(str, color, fontSize);
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

	// 耐久度の描画

	// 距離の描画

}

// UI表示座標のセット
void MissionUI::SetUIPosition()
{
	// UI表示座標 (画面の中心 : x = 0.0, 画面上方 : y = 画面の高さ÷2+任務情報テクスチャの高さ)
	Vector2 screenUIPos = Vector2(0.0f, -GAME_CONFIG->GetScreenHeight() / 2.0f + m_detailTex->GetHeight());

	// UI表示位置をスクリーンの幅・高さから設定
	m_uiPos = screenUIPos;
}
