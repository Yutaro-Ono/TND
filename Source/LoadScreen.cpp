#include "LoadScreen.h"
#include "GameMain.h"
#include "Renderer.h"
#include "Texture.h"
#include "Font.h"

#include <sstream>

// コンストラクタ
LoadScreen::LoadScreen()
	:m_state(DISABLE)
	,m_gaugeNum(0)
	,m_isGame(false)
{

	// "Loading"生成
	m_loading = m_font->RenderText("Loading", Vector3(1.0f, 1.0f, 1.0f), 64);


	// ロード用ゲージ
	for (int i = 0; i < 11; i++)
	{
		std::stringstream ssGauge;
		// ファイルパス
		ssGauge << "Data/Interface/System/Load/Loading_UI_" << i << ".png";

		m_loadGauge[i] = RENDERER->GetTexture(ssGauge.str());

	}

	// 背景
	m_bgTexture = RENDERER->GetTexture("Data/Interface/System/Load/LoadScreen_bg.png");

	// チュートリアルメッセージ
	m_tutorialMsg = RENDERER->GetTexture("Data/Interface/System/Load/LoadScreen_tuto_1.png");

	// 座標
	m_loadingPos = Vector2(0.0f, -RENDERER->GetScreenHeight() / 2 + m_loading->GetHeight() + 180.0f);
	m_loadGaugePos = Vector2(0.0f, -RENDERER->GetScreenHeight() / 2 + 100.0f);
}

// デストラクタ
LoadScreen::~LoadScreen()
{
	// 処理なし
}

// 初期化処理
void LoadScreen::Initialize()
{
	m_gaugeNum = 0;
	m_state = ENABLE;
}

void LoadScreen::Update(float in_deltaTime)
{
}

void LoadScreen::Draw(Shader * in_shader)
{

	if (m_state == ENABLE)
	{
		// 背景
		if (m_bgTexture)
		{
			DrawTexture(in_shader, m_bgTexture, Vector2(0.0f, 0.0f), 1.0f);
		}

		// 操作説明
		if (m_tutorialMsg && m_isGame == true)
		{
			DrawTexture(in_shader, m_tutorialMsg, Vector2(0.0f, 0.0f), 1.0f);
		}

		if (m_loading)
		{
			// DrawTexture(in_shader, m_loading, m_loadingPos, 1.0f);
		}

		// ロードゲージの描画
		for (int i = 0; i < m_gaugeNum; i++)
		{
			if (m_loadGauge[i])
			{
				DrawTexture(in_shader, m_loadGauge[i], m_loadGaugePos, 0.25f);
			}
		}
	}


}

void LoadScreen::AddGauge()
{
	if (m_gaugeNum < 11)
	{
		m_gaugeNum++;
	}
}
