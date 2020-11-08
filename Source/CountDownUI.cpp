//-----------------------------------------------------------------------+
// カウントダウンUIクラス.
// 
// 
// copyright (C) 2019 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#include "CountDownUI.h"
#include "GameMain.h"
#include "Renderer.h"
#include "Texture.h"
#include "Font.h"
#include "RuleTime.h"
#include "Input.h"
#include "InputController.h"
#include <string>


// コンストラクタ
CountDownUI::CountDownUI(RuleTime * in_time)
	:m_time(in_time)
	,m_timeTex(nullptr)
	,m_isAdding(false)
	,m_timer(0.0f)
	,m_fontSize(48)
{
	
	// 制限時間(秒)
	m_timeTex = m_font->RenderText("FREE MODE", Vector3(1.0f, 1.0f, 1.0f), m_fontSize);
	// 制限時間(秒)の影を生成
	m_timeTexShadow = m_font->RenderText("FREE MODE", Vector3(0.0f, 0.0f, 0.0f), m_fontSize);

	// スキップ
	m_skipXpad = RENDERER->GetTexture("Data/Interface/Tutorial/Result/ScoreScreenSkip_XPad.png");
	m_skipKey = RENDERER->GetTexture("Data/Interface/Tutorial/Result/ScoreScreenSkip_Keyboard.png");

	for (int i = 0; i < 2; i++)
	{
		// "制限時間"の影
		if (i == 0)
		{
			m_timeLimitTex[i] = m_font->RenderText("TIME", Vector3(0.0f, 0.0f, 0.0f), 44);
		}

		// "制限時間"
		if (i == 1)
		{
			m_timeLimitTex[i] = m_font->RenderText("TIME", Vector3(1.0f, 1.0f, 1.0f), 44);
		}
	}

	// 時間追加分フォントを生成
	m_addTimeTex = m_font->RenderText("+1sec", Vector3(4.0f, 1.0f, 0.0f), 34);
	// 影を生成
	m_addTimeTexShadow = m_font->RenderText("+1sec", Vector3(0.0f, 0.0f, 0.0f), 34);

	// フォントの表示座標を設定
	m_timeTexPos = Vector2(0.0f, (RENDERER->GetScreenHeight() / 2.0f) - (m_fontSize / 2.0f));


}

// デストラクタ
CountDownUI::~CountDownUI()
{

}

// 更新処理
void CountDownUI::Update(float in_deltaTime)
{
	// 経過時間をint型で取得し、string型に変換
	int tempTime = (int)m_time->GetNowCount();
	std::string str = std::to_string(tempTime);

	// フリーモード時の表示
	if (m_time->GetState() != m_time->STATE_FREE)
	{

	}

	// ゲーム開始時カウントダウン用フォントの更新
	if (m_time->GetCountState() == m_time->STATE_START)
	{
		// 更新するため解放
		if(m_timeTex != nullptr) m_timeTex->Delete();
		if (m_timeTexShadow != nullptr) m_timeTexShadow->Delete();


		// カウントが1秒より高ければ数字で表示、低ければテキストで「START」と表示
		if (m_time->GetNowCount() > 0.0f)
		{
			// 制限時間の本体
			m_timeTex = m_font->RenderText(str, Vector3(1.0f, 1.0f, 1.0f), m_fontSize);
			// 制限時間の影
			m_timeTexShadow = m_font->RenderText(str, Vector3(0.0f, 0.0f, 0.0f), m_fontSize);
		}
		else
		{
			// 制限時間の本体
			m_timeTex = m_font->RenderText("START", Vector3(1.0f, 1.0f, 1.0f), m_fontSize);
			// 影
			m_timeTexShadow = m_font->RenderText("START", Vector3(0.0f, 0.0f, 0.0f), m_fontSize);
		}

	}

	// 制限時間フォントの更新処理
	if (m_time->GetCountState() == m_time->STATE_GAME)
	{


		// 残り時間によって色を変更したフォントを生成
		if (tempTime > m_time->GetLimitTime() / 2.0f)
		{
			// 更新するため解放
			if (m_timeTex != nullptr) m_timeTex->Delete();
			if (m_timeTexShadow != nullptr) m_timeTexShadow->Delete();

			m_timeTex = m_font->RenderText(str, Vector3(1.0f, 1.0f, 1.0f), m_fontSize);
		}
		else if (tempTime > m_time->GetLimitTime() / 3.0f)
		{
			// 更新するため解放
			if (m_timeTex != nullptr) m_timeTex->Delete();
			if (m_timeTexShadow != nullptr) m_timeTexShadow->Delete();

			m_timeTex = m_font->RenderText(str, Vector3(0.0f, 1.0f, 1.0f), m_fontSize);
		}
		else if (tempTime > 10)
		{
			// 更新するため解放
			if (m_timeTex != nullptr) m_timeTex->Delete();
			if (m_timeTexShadow != nullptr) m_timeTexShadow->Delete();

			m_timeTex = m_font->RenderText(str, Vector3(0.0f, 0.6f, 1.0f), m_fontSize);
		}
		else
		{
			// 更新するため解放
			if (m_timeTex != nullptr) m_timeTex->Delete();
			if (m_timeTexShadow != nullptr) m_timeTexShadow->Delete();

			m_timeTex = m_font->RenderText(str, Vector3(0.0f, 0.2f, 1.0f), m_fontSize);
		}

		// 制限時間に応じて色を変換
		// Vector3 timeColor = Vector3(0.0f, tempTime / m_time->GetLimitTime(), m_time->GetLimitTime() / tempTime);
		// フォントの更新
		// m_timeTex = m_font->RenderText(str, timeColor, m_fontSize);

		// 影の生成
		m_timeTexShadow = m_font->RenderText(str, Vector3(0.0f, 0.0f, 0.0f), m_fontSize);
		
	}

	if (m_time->GetCountState() == m_time->STATE_FINISH)
	{
		// 更新するため解放
		if (m_timeTex != nullptr) m_timeTex->Delete();
		if (m_timeTexShadow != nullptr) m_timeTexShadow->Delete();

		m_timeTex = m_font->RenderText("FINISH", Vector3(1.0f, 1.0f, 1.0f), m_fontSize);
		// 影
		m_timeTexShadow = m_font->RenderText("FINISH", Vector3(0.0f, 0.0f, 0.0f), m_fontSize);
	}

}

// 描画処理
void CountDownUI::Draw(Shader * in_shader)
{
	// フリーモード時の"Skip"
	if (m_time->GetCountState() == m_time->STATE_FREE)
	{
		// コントローラ接続時と未接続時で表示を切り替え
		if (CONTROLLER_INSTANCE.IsAvailable() == true)
		{
			//DrawTexture(in_shader, m_skipXpad, Vector2(0.0f, -RENDERER->GetScreenHeight() / 2.2f), 0.8f);
		}
		else
		{
			//DrawTexture(in_shader, m_skipKey, Vector2(0.0f, -RENDERER->GetScreenHeight() / 2.2f), 0.8f);
		}
	}

	// 制限時間の描画
	if (m_timeTexShadow)
	{
		// 影
		DrawTexture(in_shader, m_timeTexShadow, Vector2(m_timeTexPos.x, m_timeTexPos.y - 3.0f), 1.0f);
	}
	if (m_timeTex)
	{
		// 本体
		DrawTexture(in_shader, m_timeTex, m_timeTexPos, 1.0f);
	}

	// "TIME"の描画 ※ゲーム中のみ
	if (m_time->GetCountState() == m_time->STATE_GAME)
	{
		for (int i = 0; i < 2; i++)
		{

			if (i == 0)
			{
				// 影
				DrawTexture(in_shader, m_timeLimitTex[i], Vector2(m_timeTexPos.x, m_timeTexPos.y + 57.0f), 1.0f);
			}

			if (i == 1)
			{
				// 影
				DrawTexture(in_shader, m_timeLimitTex[i], Vector2(m_timeTexPos.x, m_timeTexPos.y + 60.0f), 1.0f);
			}

		}
	}

	// 追加された時間の表示
	if (m_time->GetIsAdding() == true)
	{
		if (m_addTimeTexShadow)
		{
			DrawTexture(in_shader, m_addTimeTexShadow, m_timeTexPos + Vector2(0.0f, -53.0f), 1.0f);
		}
		if (m_addTimeTex)
		{
			DrawTexture(in_shader, m_addTimeTex, m_timeTexPos + Vector2(0.0f, -50.0f), 1.0f);
		}
	}



}
