//-----------------------------------------------------------------------+
// スピードメーターUIクラス.
// 
// 
// copyright (C) 2019 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#include "SpeedMeterUI.h"
#include "GameMain.h"
#include "Renderer.h"
#include "Texture.h"
#include "Font.h"
#include <string>
#include <sstream>


// コンストラクタ
SpeedMeterUI::SpeedMeterUI(Player * in_target)
	:m_player(in_target)
	,m_fontSize(68)
	,m_currentSpeedRate(0)
{

	//// プレイヤーの速度をint型で取得
	//int tempSpeed = (int)m_player->GetAccel();
	//std::string strSpeed;          // string
	//std::stringstream ssSpeed;     // stringstream

	//strSpeed = std::to_string(tempSpeed);     // int型の速度をstringに変換
	//ssSpeed << strSpeed << "km/h";             // sstreamで速度単位を追加
	//strSpeed = ssSpeed.str();                 // 最終出力

	//// 描画するフォントを生成
	////m_speedTex = m_font->RenderText(strSpeed, Vector3(1.0f, 1.0f, 0.0f), m_fontSize);
	//// 表示座標の設定
	//m_speedTexPos = Vector2(0.0f, (-RENDERER->GetScreenHeight() / 1.7f) + (m_fontSize * 2));

	//// ゲージのロード
	//for (int i = 0; i < 20; i++)
	//{
	//	// ファイルパス取得用
	//	std::stringstream ssGauge;
	//	std::stringstream ssShadow;

	//	// ゲージ
	//	ssGauge << "Data/Interface/HUD/SpeedMeter/Meter/SpeedMeter_" << (i + 1) << ".png";
	//	m_gaugeTex[i] = RENDERER->GetTexture(ssGauge.str());
	//}

	//// 影
	//m_gaugeShadowTex = RENDERER->GetTexture("Data/Interface/HUD/SpeedMeter/Shadow/SpeedMeterShadow.png");

}

// デストラクタ
SpeedMeterUI::~SpeedMeterUI()
{
}

// 更新処理
void SpeedMeterUI::Update(float in_deltaTime)
{


	//int tempSpeed = (int)m_player->GetAccel();
	//std::string strSpeed;

	//std::stringstream ssSpeed;

	//// int
	//strSpeed = std::to_string(tempSpeed);     // int型の速度をstringに変換
	//ssSpeed << strSpeed << "km/h";             // sstreamで速度単位を追加
	//strSpeed = ssSpeed.str();                 // 最終出力

	//// 描画するフォントの生成
	//// m_speedTex = m_font->RenderText(strSpeed, Vector3(1.0f, 1.0f, 0.0f), m_fontSize);


	//// プレイヤーの最高速度と現在速度の比率を格納
	//m_currentSpeedRate = m_player->GetAccel() / m_player->GetAccelMax() * 20;
	//if (m_currentSpeedRate > 20)
	//{
	//	m_currentSpeedRate = 20;
	//}


}

// 描画処理
void SpeedMeterUI::Draw(Shader * in_shader)
{
	// スピード数値の描画
	// DrawTexture(in_shader, m_speedTex, m_speedTexPos, 1.0f);

	//// スピードメーターの影の描画
	//DrawTexture(in_shader, m_gaugeShadowTex, Vector2(0.0f, -RENDERER->GetScreenHeight() / 3.5f), 0.7f);

	//// スピードメーターの描画
	//for (int i = 0; i < m_currentSpeedRate; i++)
	//{
	//	DrawTexture(in_shader, m_gaugeTex[i], Vector2(0.0f, -RENDERER->GetScreenHeight() / 3.5f), 0.7f);
	//}


}
