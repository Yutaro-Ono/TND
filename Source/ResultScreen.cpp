//-----------------------------------------------------------------------+
// ���U���g�V�[���pUI�N���X.
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

// �R���X�g���N�^
ResultScreen::ResultScreen(class ResultScene* in_targetScene, int in_score, float in_bestSpeed)
	:m_targetScene(in_targetScene)
	,m_score(in_score)
	,m_rank(0)
	,m_dispScore(0)
	,m_counter(0.0f)
	,m_fontSize(64)
	,m_resultTexPos(0.0f, 0.0f)
{

	// �X�s�[�h�i�[�p
	std::string strSpeed;
	std::stringstream ssSpeed;
	
	ssSpeed << "MAX SPEED:" << (int)in_bestSpeed << "km/h";
	strSpeed = ssSpeed.str();

	// �t�H���g�̃��[�h(0 = �e�A1 = �{��)
	for (int i = 0; i < 2; i++)
	{
		// �e
		if (i == 0)
		{
			m_resultTex[i] = m_font->RenderText("RESULT", Vector3(0.0f, 0.0f, 0.0f), m_fontSize);
			m_bestSpeedTex[i] = m_font->RenderText(strSpeed, Vector3(0.0f, 0.0f, 0.0f), m_fontSize);
			m_restartButton[i] = m_font->RenderText("RESTART", Vector3(0.0f, 0.0f, 0.0f), m_fontSize);
			m_quitButton[i] = m_font->RenderText("QUIT", Vector3(0.0f, 0.0f, 0.0f), m_fontSize);
		}

		// �{��
		if (i == 1)
		{
			m_resultTex[i] = m_font->RenderText("RESULT", Vector3(1.0f, 1.0f, 1.0f), m_fontSize);
			m_bestSpeedTex[i] = m_font->RenderText(strSpeed, Vector3(0.0f, 0.3f, 1.0f), m_fontSize);
			m_restartButton[i] = m_font->RenderText("RESTART", Vector3(1.0f, 1.0f, 1.0f), m_fontSize);
			m_quitButton[i] = m_font->RenderText("QUIT", Vector3(1.0f, 1.0f, 1.0f), m_fontSize);
		}
	}

	// �����L���O�p�E�B���h�E
	m_rankingWindow = RENDERER->GetTexture("Data/Interface/System/Score_Window.png");
	m_rankingWindow->SetWidth(1024);
	m_rankingWindow->SetWidth(m_rankingWindow->GetWidth() / 3.0f);

	// �`���[�g���A���p�e�N�X�`���̃��[�h
	m_xpadSkipTex = RENDERER->GetTexture("Data/Interface/Tutorial/Result/ScoreScreenSkip_XPad.png");
	m_keyboardSkipTex = RENDERER->GetTexture("Data/Interface/Tutorial/Result/ScoreScreenSkip_Keyboard.png");

	m_xpadTutorialTex = RENDERER->GetTexture("Data/Interface/Tutorial/Result/ScoreScreenUI_XPad.png");
	m_keyboardTutorialTex = RENDERER->GetTexture("Data/Interface/Tutorial/Result/ScoreScreenUI_Keyboard.png");

	// RESULT
	m_resultTexPos = Vector2(0.0f, RENDERER->GetScreenHeight() / 2 - m_resultTex[0]->GetHeight());
	
}

ResultScreen::~ResultScreen()
{
	//// �t�H���g�̍폜
	for (int i = 0; i < 2; i++)
	{
		m_resultTex[i]->Delete();
		m_bestSpeedTex[i]->Delete();
		m_restartButton[i]->Delete();
		m_quitButton[i]->Delete();
	}

}

void ResultScreen::Update(float in_deltaTime)
{

	// Score��stringstream�ɕϊ�
	std::stringstream strScore;

	// �X�R�A�̉��Z���o
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
		// �t�H���g�̍폜����
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

		// �e
		if (i == 0)
		{
			// "RESULT" �̕`��
			if (m_resultTex)
			{
				// DrawTexture(in_shader, m_resultTex[i], m_resultTexPos + Vector2(0.0f, -3.0f), 1.0f);
			}


			// �X�R�A���̕`��
			if (m_scoreTex)
			{
				// DrawTexture(in_shader, m_scoreTex[i], Vector2(0.0f, RENDERER->GetScreenHeight() / 5) + Vector2(0.0f, -3.0f), 1.0f);
				DrawTexture(in_shader, m_scoreTex[i], m_resultTexPos + Vector2(0.0f, -3.0f), 1.0f);

			}

			// �X�R�A�̕\�����I�������`��
			if (m_targetScene->GetResultState() != m_targetScene->STATE_SCORE_DISPLAY)
			{
				// �ō����x
				if (m_bestSpeedTex)
				{
					// DrawTexture(in_shader, m_bestSpeedTex[i], Vector2(0.0f, -RENDERER->GetScreenHeight() / 2.6f) + Vector2(0.0f, -3.0f), 1.0f);
				}

				if (m_restartButton)
				{
					// ���X�^�[�g�I�����Ɣ�I����
					if (m_targetScene->GetResultState() == ResultScene::STATE_ONE_MORE)
					{
						m_restartButton[i] = m_font->RenderText("RESTART", Vector3(0.0f, 0.0f, 0.0f), 64);
						DrawTexture(in_shader, m_restartButton[i], Vector2(-m_restartButton[i]->GetWidth(), -RENDERER->GetScreenHeight() / 2.9f) + Vector2(0.0f, -3.0f), 1.0f);
					}
					else
					{
						m_restartButton[i] = m_font->RenderText("RESTART", Vector3(0.0f, 0.0f, 0.0f), 64);
						DrawTexture(in_shader, m_restartButton[i], Vector2(-m_restartButton[i]->GetWidth(), -RENDERER->GetScreenHeight() / 2.9f) + Vector2(0.0f, -3.0f), 0.5f);
					}

				}

				if (m_quitButton)
				{
					// QUIT�I�����Ɣ�I����
					if (m_targetScene->GetResultState() == ResultScene::STATE_GAME_QUIT)
					{
						m_quitButton[i] = m_font->RenderText("QUIT", Vector3(0.0f, 0.0f, 0.0f), 64);
						DrawTexture(in_shader, m_quitButton[i], Vector2(m_quitButton[i]->GetWidth() * 1.5f, -RENDERER->GetScreenHeight() / 2.9f) + Vector2(0.0f, -3.0f), 1.0f);
					}
					else
					{
						m_quitButton[i] = m_font->RenderText("QUIT", Vector3(0.0f, 0.0f, 0.0f), 64);
						DrawTexture(in_shader, m_quitButton[i], Vector2(m_quitButton[i]->GetWidth() * 1.5f, -RENDERER->GetScreenHeight() / 2.9f) + Vector2(0.0f, -3.0f), 0.7f);
					}
				}
			}
		}

		// �{��
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


			if (m_targetScene->GetResultState() != m_targetScene->STATE_SCORE_DISPLAY)
			{

				// �ō����x
				if (m_bestSpeedTex)
				{
					// DrawTexture(in_shader, m_bestSpeedTex[i], Vector2(0.0f, -RENDERER->GetScreenHeight() / 2.6f), 1.0f);
				}

				if (m_restartButton)
				{
					// ���X�^�[�g�I�����Ɣ�I����
					if (m_targetScene->GetResultState() == ResultScene::STATE_ONE_MORE)
					{
						m_restartButton[i] = m_font->RenderText("RESTART", Vector3(1.0f, 1.0f, 1.0f), 64);
						DrawTexture(in_shader, m_restartButton[i], Vector2(-m_restartButton[i]->GetWidth(), -RENDERER->GetScreenHeight() / 2.9f), 1.0f);
					}
					else
					{
						m_restartButton[i] = m_font->RenderText("RESTART", Vector3(0.5f, 0.5f, 0.5f), 64);
						DrawTexture(in_shader, m_restartButton[i], Vector2(-m_restartButton[i]->GetWidth(), -RENDERER->GetScreenHeight() / 2.9f), 0.5f);
					}

				}

				if (m_quitButton)
				{
					// QUIT�I�����Ɣ�I����
					if (m_targetScene->GetResultState() == ResultScene::STATE_GAME_QUIT)
					{
						m_quitButton[i] = m_font->RenderText("QUIT", Vector3(1.0f, 1.0f, 1.0f), 64);
						DrawTexture(in_shader, m_quitButton[i], Vector2(m_quitButton[i]->GetWidth() * 1.5f, -RENDERER->GetScreenHeight() / 2.9f), 1.0f);
					}
					else
					{
						m_quitButton[i] = m_font->RenderText("QUIT", Vector3(0.5f, 0.5f, 0.5f), 64);
						DrawTexture(in_shader, m_quitButton[i], Vector2(m_quitButton[i]->GetWidth() * 1.5f, -RENDERER->GetScreenHeight() / 2.9f), 0.7f);
					}
				}
			}
		}

	}



	// �����L���O�̕`��
		// �E�B���h�E
	if (m_targetScene->GetResultState() != m_targetScene->STATE_SCORE_DISPLAY)
	{
		DrawTexture(in_shader, m_rankingWindow, Vector2(0.0f, 35.0f), 0.8f);
	}

	// �_�ŗp�J�E���^�[����
	m_counter += 0.1f;
	if (m_counter >= 100.0f)
	{
		m_counter = 0.0f;
	}

	for (int i = 0; i < 10; i++)
	{
		if (m_targetScene->GetResultState() != m_targetScene->STATE_SCORE_DISPLAY)
		{
			if (i != (m_rank - 1))
			{
				DrawTexture(in_shader, m_rankingScore[i], Vector2(0.0f, (RENDERER->GetScreenHeight() / 15) * (6 - (i + 1))), 0.7f);
			}
			else if ((int)m_counter % 3 == 0)
			{

				DrawTexture(in_shader, m_rankingScore[i], Vector2(0.0f, (RENDERER->GetScreenHeight() / 15) * (6 - (i + 1))), 0.7f);
			}
		}
	}

	//-----------------------------------------------------------------------------------------------------------------+
	// �`���[�g���A���e�N�X�`���̕`��(�R���g���[���ڑ����Ɩ��ڑ����ŕ���)
	//-----------------------------------------------------------------------------------------------------------------+
	// �X�R�A�v�����o���ɂ̂ݕ`��
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
	// �����L���O�\�����ɂ̂ݕ`��
	if (m_targetScene->GetResultState() != m_targetScene->STATE_SCORE_DISPLAY)
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

	// �X�R�A��stringstream�Ɋi�[
	std::stringstream strScore;
	strScore << (in_num + 1) << ":" << in_score;

	if ((in_num + 1) == m_rank)
	{
		m_rankingScore[in_num] = m_font->RenderText(strScore.str(), Vector3(0.0f, 0.2f, 1.0f), 64);
	}
	else
	{
		m_rankingScore[in_num] = m_font->RenderText(strScore.str(), Vector3(1.0f, 1.0f, 1.0f), 64);
	}

}
