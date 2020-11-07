//-----------------------------------------------------------------------+
// �^�C�g���V�[��UI�N���X.
// 
// 
// copyright (C) 2020 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#include "TitleScreen.h"
#include "GameMain.h"
#include "Renderer.h"
#include "Input.h"
#include "InputController.h"
#include "Font.h"
#include "Texture.h"
#include "Math.h"
#include "TitleScene.h"
#include <sstream>
#include <SDL.h>

// �R���X�g���N�^
TitleScreen::TitleScreen(class TitleScene* in_target)
	:m_logo(nullptr)
	,m_cursol(nullptr)
	,m_logoPos(0.0f, RENDERER->GetScreenHeight() / 3)
	,m_logoJpPos(320.0f, 30.0f)
	,m_anyKeyPos(0.0f, -275.0f)
	,m_startPos(0.0f, 0.0f)
	,m_quitPos(0.0f, -315.0f)
{

	// �^�C�g���V�[����ێ�
	m_targetScene = in_target;

	// �^�C�g�����S�̓ǂݍ���
	//m_logo = RENDERER->GetTexture("Data/Interface/Title/Title_LogoScreen.png");
	m_logo = m_font->RenderText("The Night Driver", Vector3(1.0f, 1.0f, 1.0f), 72);
	m_logoPos = Vector2((-GAME_CONFIG->GetScreenWidth() / 2) + (m_logo->GetWidth() / 2) + 80, RENDERER->GetScreenHeight() / 2 - m_logo->GetHeight() - 80);

	m_cursol = RENDERER->GetTexture("Data/Interface/System/Cursol_UI.png");
	m_cross = RENDERER->GetTexture("Data/Interface/TND/Control/Cursol_UI_2.png");

	// �I�����ڊ֘A�̃t�H���g
	for (int i = 0; i < 2; i++)
	{
		m_anyKey[i] = m_font->RenderText("PRESS ANY KEY", Vector3(0.0f + i, 0.0f + i, 0.0f + i), 48);
		m_startButton[i] = m_font->RenderText("GAME START", Vector3(0.0f + i, 0.0f + i, 0.0f + i), 32);
		m_quitButton[i] = m_font->RenderText("QUIT TO DESKTOP", Vector3(0.0f + i, 0.0f + i, 0.0f + i), 32);
	}

	// �`���[�g���A���p�摜�̓ǂݍ���
	m_xpadTutorialTex = RENDERER->GetTexture("Data/Interface/Tutorial/Title/TitleScreenUI_XPad.png");
	m_keyboardTutorialTex = RENDERER->GetTexture("Data/Interface/Tutorial/Title/TitleScreenUI_Keyboard.png");

	m_counter = 0.0f;

	// �X�e�[�W�̃t�H���g����
	for (int i = 0; i < m_targetScene->GetAllStageNum(); i++)
	{
		std::stringstream ssStage;
		ssStage << "STAGE " << (i + 1);

		m_stage.emplace_back();
		m_stage[i] = m_font->RenderText(ssStage.str(), Vector3(1.0f, 1.0f, 1.0f), 40);
	}

}


// �f�X�g���N�^
TitleScreen::~TitleScreen()
{

	for (int i = 0; i < 2; i++)
	{
		m_anyKey[i]->Delete();
		m_startButton[i]->Delete();
		m_quitButton[i]->Delete();
	}

}


// �X�V����
void TitleScreen::Update(float in_deltaTime)
{

	m_counter += 1.0f * in_deltaTime;
	// �㉺����A�j���[�V����
	m_startPos.y = m_startPos.y + Math::Sin(m_counter) * 0.2f;
	m_quitPos.y = m_quitPos.y + Math::Sin(m_counter) * 0.2f;
	m_anyKeyPos.y = m_anyKeyPos.y + Math::Sin(m_counter) * 0.2f;
}


// �`�揈��
void TitleScreen::Draw(Shader * in_shader)
{

	//---------------------------------------------------------------------------+
	// ��{�`��(�^�C�g�����S, �`���[�g���A���\��)
	//---------------------------------------------------------------------------+
	// �^�C�g�����S
	if (m_logo)
	{
		DrawTexture(in_shader, m_logo, m_logoPos, 1.0f);
	}
	// �`���[�g���A��UI(�p�b�h�ڑ����Ɩ��ڑ����ŏ����𕪂���)
	if (CONTROLLER_INSTANCE.IsAvailable() == true)
	{
		DrawTexture(in_shader, m_xpadTutorialTex, Vector2(0.0f, -RENDERER->GetScreenHeight() / 2.4f), 0.9f);
	}
	else
	{
		DrawTexture(in_shader, m_keyboardTutorialTex, Vector2(0.0f, -RENDERER->GetScreenHeight() / 2.4f), 0.9f);
	}




	switch (m_targetScene->GetState())
	{


	case TitleScene::PRESS_ANY_KEY:


		// PRESS ANY KEY
		if (m_anyKey[0] != nullptr || m_anyKey[1] != nullptr)
		{
			DrawTexture(in_shader, m_anyKey[0], m_anyKeyPos, 1.0f);
			DrawTexture(in_shader, m_anyKey[1], m_anyKeyPos + Vector2(-3.0f, 3.0f), 1.0f);
			
		}

		break;


	case TitleScene::GAME_START:

		// �X�^�[�g�{�^��
		if (m_startButton[0] != nullptr || m_startButton[1] != nullptr)
		{
			DrawTexture(in_shader, m_startButton[0], Vector2(GAME_CONFIG->GetScreenWidth() / 2 - m_startButton[0]->GetWidth() / 2 - 80, -RENDERER->GetScreenHeight() / 2 + m_startButton[0]->GetHeight() + 280), 1.0f);
			DrawTexture(in_shader, m_startButton[1], Vector2(GAME_CONFIG->GetScreenWidth() / 2 - m_startButton[1]->GetWidth() / 2 - 80, -RENDERER->GetScreenHeight() / 2 + m_startButton[1]->GetHeight() + 280) + Vector2(-3.0f, 3.0f), 1.0f);
			
		}
		// �I���{�^��
		if (m_quitButton[0] != nullptr || m_quitButton[1] != nullptr)
		{
			DrawTexture(in_shader, m_quitButton[0], Vector2(GAME_CONFIG->GetScreenWidth() / 2 - m_quitButton[0]->GetWidth() / 2 - 80, -RENDERER->GetScreenHeight() / 2 + m_quitButton[0]->GetHeight() + 180), 1.0f);
			DrawTexture(in_shader, m_quitButton[1], Vector2(GAME_CONFIG->GetScreenWidth() / 2 - m_quitButton[1]->GetWidth() / 2 - 80, -RENDERER->GetScreenHeight() / 2 + m_quitButton[1]->GetHeight() + 180) + Vector2(-3.0f, 3.0f), 1.0f);
		}

		// �J�[�\��
		if (m_cursol != nullptr)
		{
			DrawTexture(in_shader, m_cursol, Vector2(GAME_CONFIG->GetScreenWidth() / 2 - m_startButton[1]->GetWidth() - m_cursol->GetWidth() * 2, -RENDERER->GetScreenHeight() / 2 + m_startButton[1]->GetHeight() + 280) + m_startPos, 0.9f);
		}
		// �\���L�[
		if (m_cross != nullptr)
		{
			DrawTexture(in_shader, m_cross, Vector2(GAME_CONFIG->GetScreenWidth() / 2 - m_startButton[1]->GetWidth() - m_cursol->GetWidth() * 3, -RENDERER->GetScreenHeight() / 2 + m_startButton[1]->GetHeight() + 280) + m_startPos, 0.1f);
		}


		break;


	case TitleScene::STAGE_SELECT:

		// �X�e�[�W���ڂ̕`��
		for (int i = 0; i < m_stage.size(); i++)
		{
			// �I�𒆂̃X�e�[�W�͊g�債�ĕ`��
			if (i != m_targetScene->GetNowSelectStage())
			{
				DrawTexture(in_shader, m_stage[i], Vector2(m_startPos.x, m_startPos.y - (m_stage[i]->GetHeight() * 1.5f * i)), 0.5f);
			}
			else
			{
				DrawTexture(in_shader, m_stage[i], Vector2(m_startPos.x, m_startPos.y - (m_stage[i]->GetHeight() * 1.5f * i)), 1.0f);

			}

		}


		break;


	case TitleScene::GAME_QUIT:

		// �X�^�[�g�{�^��
		if (m_startButton[0] != nullptr || m_startButton[1] != nullptr)
		{
			DrawTexture(in_shader, m_startButton[0], Vector2(GAME_CONFIG->GetScreenWidth() / 2 - m_startButton[0]->GetWidth() / 2 - 80, -RENDERER->GetScreenHeight() / 2 + m_startButton[0]->GetHeight() + 280), 1.0f);
			DrawTexture(in_shader, m_startButton[1], Vector2(GAME_CONFIG->GetScreenWidth() / 2 - m_startButton[1]->GetWidth() / 2 - 80, -RENDERER->GetScreenHeight() / 2 + m_startButton[1]->GetHeight() + 280) + Vector2(-3.0f, 3.0f), 1.0f);

		}
		// �I���{�^��
		if (m_quitButton[0] != nullptr || m_quitButton[1] != nullptr)
		{
			DrawTexture(in_shader, m_quitButton[0], Vector2(GAME_CONFIG->GetScreenWidth() / 2 - m_quitButton[0]->GetWidth() / 2 - 80, -RENDERER->GetScreenHeight() / 2 + m_quitButton[0]->GetHeight() + 180), 1.0f);
			DrawTexture(in_shader, m_quitButton[1], Vector2(GAME_CONFIG->GetScreenWidth() / 2 - m_quitButton[1]->GetWidth() / 2 - 80, -RENDERER->GetScreenHeight() / 2 + m_quitButton[1]->GetHeight() + 180) + Vector2(-3.0f, 3.0f), 1.0f);
		}

		// �J�[�\��
		if (m_cursol != nullptr)
		{
			DrawTexture(in_shader, m_cursol, Vector2(GAME_CONFIG->GetScreenWidth() / 2 - m_quitButton[1]->GetWidth() - m_cursol->GetWidth() * 2, -RENDERER->GetScreenHeight() / 2 + m_quitButton[1]->GetHeight() + 180) + m_startPos, 0.9f);
		}
		// �\���L�[
		if (m_cross != nullptr)
		{
			DrawTexture(in_shader, m_cross, Vector2(GAME_CONFIG->GetScreenWidth() / 2 - m_quitButton[1]->GetWidth() - m_cursol->GetWidth() * 3, -RENDERER->GetScreenHeight() / 2 + m_quitButton[1]->GetHeight() + 180) + m_startPos, 0.1f);
		}

		break;


	default:

		break;


	}


}
