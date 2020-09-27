//-----------------------------------------------------------------------+
// チュートリアル用UI(ゲームシーン).
// 
// 
// copyright (C) 2020 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#include "TutorialUI.h"
#include "GameMain.h"
#include "Renderer.h"
#include "Texture.h"
#include "InputController.h"


// コンストラクタ
TutorialUI::TutorialUI()
{

	// コントローラ用テクスチャ
	m_controllerTex = RENDERER->GetTexture("Data/Interface/Tutorial/Game/TutorialUI_GameScene_Controller.png");
	// キーボード用テクスチャ
	m_keyboardTex = RENDERER->GetTexture("Data/Interface/Tutorial/Game/TutorialUI_GameScene_Keyboard.png");

	// タスク用テクスチャ
	m_taskTex = RENDERER->GetTexture("Data/Interface/Tutorial/Game/Task.png");

}


// デストラクタ
TutorialUI::~TutorialUI()
{
	// 処理なし
}


// 更新処理
void TutorialUI::Update(float in_deltaTime)
{
	// 処理なし
}


// 描画処理
void TutorialUI::Draw(Shader * in_shader)
{

	// コントローラ接続時とそうでない時とで描画を変える
	if (m_controllerTex && CONTROLLER_INSTANCE.IsAvailable() == true)
	{
		DrawTexture(in_shader, m_controllerTex, Vector2(0.0f, -RENDERER->GetScreenHeight() / 2.7f), 0.75f);
	}
	else if (m_keyboardTex)
	{
		DrawTexture(in_shader, m_keyboardTex, Vector2(0.0f, -RENDERER->GetScreenHeight() / 2.7f), 0.75f);
	}

	// 目標の表示
	DrawTexture(in_shader, m_taskTex, Vector2(-RENDERER->GetScreenWidth() / 2.7f, RENDERER->GetScreenHeight() / 2.3f), 0.75f);

}
