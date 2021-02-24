//-----------------------------------------------------------------------+
// リザルトシーン用UIクラス.
// 
// 
// copyright (C) 2020 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#include "ResultScreen.h"
#include "ResultScene.h"
#include "GameMain.h"
#include "Renderer.h"
#include "Texture.h"
#include "Font.h"
#include "Shader.h"
#include "Input.h"
#include "InputController.h"
#include <sstream>

// コンストラクタ
ResultScreen::ResultScreen(class ResultScene* in_targetScene, int in_score, float in_bestSpeed)
	:m_targetScene(in_targetScene)
	,m_score(in_score)
	,m_rank(0)
	,m_dispScore(0)
	,m_counter(0.0f)
	,m_fontSize(64)
	,m_resultTexPos(0.0f, 0.0f)
{

	// スピード格納用
	std::string strSpeed;
	std::stringstream ssSpeed;
	
	ssSpeed << "MAX SPEED:" << (int)in_bestSpeed << "km/h";
	strSpeed = ssSpeed.str();

	// フォントのロード(0 = 影、1 = 本体)
	for (int i = 0; i < 2; i++)
	{
		m_resultTex[i] = m_font->RenderText("RESULT", Vector3(1.0f * i, 1.0f * i, 1.0f * i), m_fontSize);
		m_restartButton[i] = m_font->RenderText("RESTART", Vector3(1.0f * i, 1.0f * i, 1.0f * i), m_fontSize);
		m_quitButton[i] = m_font->RenderText("QUIT", Vector3(1.0f * i, 1.0f * i, 1.0f * i), m_fontSize);
		m_scoreTex[i] = nullptr;
	}

	// チュートリアル用テクスチャのロード
	m_xpadSkipTex = RENDERER->GetTexture("Data/Interface/Tutorial/Result/ScoreScreenSkip_XPad.png");
	m_keyboardSkipTex = RENDERER->GetTexture("Data/Interface/Tutorial/Result/ScoreScreenSkip_Keyboard.png");

	m_xpadTutorialTex = RENDERER->GetTexture("Data/Interface/Tutorial/Result/ScoreScreenUI_XPad.png");
	m_keyboardTutorialTex = RENDERER->GetTexture("Data/Interface/Tutorial/Result/ScoreScreenUI_Keyboard.png");

	// RESULT
	m_resultTexPos = Vector2(0.0f, RENDERER->GetScreenHeight() / 2 - m_resultTex[0]->GetHeight());
	
}

ResultScreen::~ResultScreen()
{
	// フォントの削除
	for (int i = 0; i < 2; i++)
	{
		m_resultTex[i]->Delete();
		m_restartButton[i]->Delete();
		m_quitButton[i]->Delete();
	}

}

void ResultScreen::Update(float in_deltaTime)
{

	// Scoreをstringstreamに変換
	std::stringstream strScore;

	// スコアの加算演出
	if (m_dispScore != m_score && m_targetScene->GetResultState() == m_targetScene->STATE_SCORE_DISPLAY)
	{
		m_dispScore += 1;

		if (INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_SPACE) || CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_A))
		{
			m_dispScore = m_score;
		}
	}

	strScore << "Your SCORE:" << m_dispScore;


	for (int i = 0; i < 2; i++)
	{
		// フォントの削除処理
		if (m_scoreTex[i] != nullptr)
		{
			m_scoreTex[i]->Delete();
		}

		if (i == 0)
		{
			m_scoreTex[i] = m_font->RenderText(strScore.str(), Vector3(0.0f, 0.0f, 0.0f), 72);
		}
		if (i == 1)
		{
			m_scoreTex[i] = m_font->RenderText(strScore.str(), Vector3(1.0f, 1.0f, 1.0f), 72);
		}

	}

}

void ResultScreen::Draw(Shader * in_shader)
{

	for (int i = 0; i < 2; i++)
	{

		// 影
		if (i == 0)
		{
			// "RESULT" の描画
			if (m_resultTex)
			{
				// DrawTexture(in_shader, m_resultTex[i], m_resultTexPos + Vector2(0.0f, -3.0f), 1.0f);
			}


			// スコア情報の描画
			if (m_scoreTex)
			{
				// DrawTexture(in_shader, m_scoreTex[i], Vector2(0.0f, RENDERER->GetScreenHeight() / 5) + Vector2(0.0f, -3.0f), 1.0f);
				DrawTexture(in_shader, m_scoreTex[i], m_resultTexPos + Vector2(0.0f, -3.0f), 1.0f);

			}

			// スコアの表示が終わったら描画
			if (m_targetScene->GetResultState() == m_targetScene->STATE_ONE_MORE || m_targetScene->GetResultState() == m_targetScene->STATE_GAME_QUIT)
			{

				if (m_restartButton)
				{
					// リスタート選択時と非選択時
					if (m_targetScene->GetResultState() == ResultScene::STATE_ONE_MORE)
					{

						DrawTexture(in_shader, m_restartButton[i], Vector2(-m_restartButton[i]->GetWidth(), -RENDERER->GetScreenHeight() / 2.9f) + Vector2(0.0f, -3.0f), 1.0f);
					}
					else
					{
						DrawTexture(in_shader, m_restartButton[i], Vector2(-m_restartButton[i]->GetWidth(), -RENDERER->GetScreenHeight() / 2.9f) + Vector2(0.0f, -3.0f), 0.5f);
					}

				}

				if (m_quitButton)
				{
					// QUIT選択時と非選択時
					if (m_targetScene->GetResultState() == ResultScene::STATE_GAME_QUIT)
					{
						DrawTexture(in_shader, m_quitButton[i], Vector2(m_quitButton[i]->GetWidth() * 1.5f, -RENDERER->GetScreenHeight() / 2.9f) + Vector2(0.0f, -3.0f), 1.0f);
					}
					else
					{
						DrawTexture(in_shader, m_quitButton[i], Vector2(m_quitButton[i]->GetWidth() * 1.5f, -RENDERER->GetScreenHeight() / 2.9f) + Vector2(0.0f, -3.0f), 0.7f);
					}
				}
			}
		}

		// 本体
		if (i == 1)
		{
			if (m_resultTex)
			{
				// DrawTexture(in_shader, m_resultTex[i], m_resultTexPos, 1.0f);
			}

			if (m_scoreTex)
			{
				// DrawTexture(in_shader, m_scoreTex[i], Vector2(0.0f, RENDERER->GetScreenHeight() / 5), 1.0f);
				DrawTexture(in_shader, m_scoreTex[i], m_resultTexPos, 1.0f);
			}


			if (m_targetScene->GetResultState() == m_targetScene->STATE_ONE_MORE || m_targetScene->GetResultState() == m_targetScene->STATE_GAME_QUIT)
			{


				if (m_restartButton)
				{
					// リスタート選択時と非選択時
					if (m_targetScene->GetResultState() == ResultScene::STATE_ONE_MORE)
					{
						DrawTexture(in_shader, m_restartButton[i], Vector2(-m_restartButton[i]->GetWidth(), -RENDERER->GetScreenHeight() / 2.9f), 1.0f);
					}
					else
					{
						DrawTexture(in_shader, m_restartButton[i], Vector2(-m_restartButton[i]->GetWidth(), -RENDERER->GetScreenHeight() / 2.9f), 0.5f);
					}

				}

				if (m_quitButton)
				{
					// QUIT選択時と非選択時
					if (m_targetScene->GetResultState() == ResultScene::STATE_GAME_QUIT)
					{
						DrawTexture(in_shader, m_quitButton[i], Vector2(m_quitButton[i]->GetWidth() * 1.5f, -RENDERER->GetScreenHeight() / 2.9f), 1.0f);
					}
					else
					{
						DrawTexture(in_shader, m_quitButton[i], Vector2(m_quitButton[i]->GetWidth() * 1.5f, -RENDERER->GetScreenHeight() / 2.9f), 0.7f);
					}
				}
			}
		}

	}



	// ランキングの描画

	// 点滅用カウンター処理
	m_counter += 0.1f;
	if (m_counter >= 100.0f)
	{
		m_counter = 0.0f;
	}


	float scoreScale = 1.0f;
	
	for (int i = 0; i < 5; i++)
	{
		if (m_targetScene->GetResultState() == m_targetScene->STATE_ONE_MORE || m_targetScene->GetResultState() == m_targetScene->STATE_GAME_QUIT)
		{
			if (i != (m_rank - 1))
			{
				DrawTexture(in_shader, m_rankingScore[i], Vector2(RENDERER->GetScreenWidth() / 2 - m_rankingScore[i]->GetHalfWidth() * scoreScale,
					                                              (10.0f * (5 - i)) + (-m_rankingScore[i]->GetHeight()) * i), scoreScale);
			}
			else if ((int)m_counter % 3 == 0)
			{

				DrawTexture(in_shader, m_rankingScore[i], Vector2(RENDERER->GetScreenWidth() / 2 - m_rankingScore[i]->GetHalfWidth() * scoreScale,
					                                              (10.0f * (5 - i)) + (-m_rankingScore[i]->GetHeight()) * i), scoreScale);
			}
		}
	}

	//-----------------------------------------------------------------------------------------------------------------+
	// チュートリアルテクスチャの描画(コントローラ接続時と未接続時で分岐)
	//-----------------------------------------------------------------------------------------------------------------+
	// スコア計測演出中にのみ描画
	if (m_targetScene->GetResultState() == m_targetScene->STATE_SCORE_DISPLAY)
	{
		if (CONTROLLER_INSTANCE.IsAvailable() == true)
		{
			DrawTexture(in_shader, m_xpadSkipTex, Vector2(0.0f, -RENDERER->GetScreenHeight() / 2.3f), 1.0f);
		}
		else
		{
			DrawTexture(in_shader, m_keyboardSkipTex, Vector2(0.0f, -RENDERER->GetScreenHeight() / 2.3f), 1.0f);
		}

	}
	// ランキング表示中にのみ描画
	if (m_targetScene->GetResultState() == m_targetScene->STATE_ONE_MORE || m_targetScene->GetResultState() == m_targetScene->STATE_GAME_QUIT)
	{
		if (CONTROLLER_INSTANCE.IsAvailable() == true)
		{
			DrawTexture(in_shader, m_xpadTutorialTex, Vector2(0.0f, -RENDERER->GetScreenHeight() / 2.3f), 0.8f);
		}
		else
		{
			DrawTexture(in_shader, m_keyboardTutorialTex, Vector2(0.0f, -RENDERER->GetScreenHeight() / 2.3f), 0.8f);

		}
	}

}

void ResultScreen::SetScore(const int& in_num, const int& in_score)
{

	// スコアをstringstreamに格納
	std::stringstream strScore;
	strScore << (in_num + 1) << ":" << in_score;

	if ((in_num + 1) == m_rank)
	{
		m_rankingScore[in_num] = m_font->RenderText(strScore.str(), Vector3(0.0f, 0.2f, 1.0f), 64);
	}
	else
	{
		m_rankingScore[in_num] = m_font->RenderText(strScore.str(), Vector3(1.0f, 0.8f, 0.5f), 64);
	}

}
