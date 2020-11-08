//-----------------------------------------------------------------------+
// �J�E���g�_�E��UI�N���X.
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


// �R���X�g���N�^
CountDownUI::CountDownUI(RuleTime * in_time)
	:m_time(in_time)
	,m_timeTex(nullptr)
	,m_isAdding(false)
	,m_timer(0.0f)
	,m_fontSize(48)
{
	
	// ��������(�b)
	m_timeTex = m_font->RenderText("FREE MODE", Vector3(1.0f, 1.0f, 1.0f), m_fontSize);
	// ��������(�b)�̉e�𐶐�
	m_timeTexShadow = m_font->RenderText("FREE MODE", Vector3(0.0f, 0.0f, 0.0f), m_fontSize);

	// �X�L�b�v
	m_skipXpad = RENDERER->GetTexture("Data/Interface/Tutorial/Result/ScoreScreenSkip_XPad.png");
	m_skipKey = RENDERER->GetTexture("Data/Interface/Tutorial/Result/ScoreScreenSkip_Keyboard.png");

	for (int i = 0; i < 2; i++)
	{
		// "��������"�̉e
		if (i == 0)
		{
			m_timeLimitTex[i] = m_font->RenderText("TIME", Vector3(0.0f, 0.0f, 0.0f), 44);
		}

		// "��������"
		if (i == 1)
		{
			m_timeLimitTex[i] = m_font->RenderText("TIME", Vector3(1.0f, 1.0f, 1.0f), 44);
		}
	}

	// ���Ԓǉ����t�H���g�𐶐�
	m_addTimeTex = m_font->RenderText("+1sec", Vector3(4.0f, 1.0f, 0.0f), 34);
	// �e�𐶐�
	m_addTimeTexShadow = m_font->RenderText("+1sec", Vector3(0.0f, 0.0f, 0.0f), 34);

	// �t�H���g�̕\�����W��ݒ�
	m_timeTexPos = Vector2(0.0f, (RENDERER->GetScreenHeight() / 2.0f) - (m_fontSize / 2.0f));


}

// �f�X�g���N�^
CountDownUI::~CountDownUI()
{

}

// �X�V����
void CountDownUI::Update(float in_deltaTime)
{
	// �o�ߎ��Ԃ�int�^�Ŏ擾���Astring�^�ɕϊ�
	int tempTime = (int)m_time->GetNowCount();
	std::string str = std::to_string(tempTime);

	// �t���[���[�h���̕\��
	if (m_time->GetState() != m_time->STATE_FREE)
	{

	}

	// �Q�[���J�n���J�E���g�_�E���p�t�H���g�̍X�V
	if (m_time->GetCountState() == m_time->STATE_START)
	{
		// �X�V���邽�߉��
		if(m_timeTex != nullptr) m_timeTex->Delete();
		if (m_timeTexShadow != nullptr) m_timeTexShadow->Delete();


		// �J�E���g��1�b��荂����ΐ����ŕ\���A�Ⴏ��΃e�L�X�g�ŁuSTART�v�ƕ\��
		if (m_time->GetNowCount() > 0.0f)
		{
			// �������Ԃ̖{��
			m_timeTex = m_font->RenderText(str, Vector3(1.0f, 1.0f, 1.0f), m_fontSize);
			// �������Ԃ̉e
			m_timeTexShadow = m_font->RenderText(str, Vector3(0.0f, 0.0f, 0.0f), m_fontSize);
		}
		else
		{
			// �������Ԃ̖{��
			m_timeTex = m_font->RenderText("START", Vector3(1.0f, 1.0f, 1.0f), m_fontSize);
			// �e
			m_timeTexShadow = m_font->RenderText("START", Vector3(0.0f, 0.0f, 0.0f), m_fontSize);
		}

	}

	// �������ԃt�H���g�̍X�V����
	if (m_time->GetCountState() == m_time->STATE_GAME)
	{


		// �c�莞�Ԃɂ���ĐF��ύX�����t�H���g�𐶐�
		if (tempTime > m_time->GetLimitTime() / 2.0f)
		{
			// �X�V���邽�߉��
			if (m_timeTex != nullptr) m_timeTex->Delete();
			if (m_timeTexShadow != nullptr) m_timeTexShadow->Delete();

			m_timeTex = m_font->RenderText(str, Vector3(1.0f, 1.0f, 1.0f), m_fontSize);
		}
		else if (tempTime > m_time->GetLimitTime() / 3.0f)
		{
			// �X�V���邽�߉��
			if (m_timeTex != nullptr) m_timeTex->Delete();
			if (m_timeTexShadow != nullptr) m_timeTexShadow->Delete();

			m_timeTex = m_font->RenderText(str, Vector3(0.0f, 1.0f, 1.0f), m_fontSize);
		}
		else if (tempTime > 10)
		{
			// �X�V���邽�߉��
			if (m_timeTex != nullptr) m_timeTex->Delete();
			if (m_timeTexShadow != nullptr) m_timeTexShadow->Delete();

			m_timeTex = m_font->RenderText(str, Vector3(0.0f, 0.6f, 1.0f), m_fontSize);
		}
		else
		{
			// �X�V���邽�߉��
			if (m_timeTex != nullptr) m_timeTex->Delete();
			if (m_timeTexShadow != nullptr) m_timeTexShadow->Delete();

			m_timeTex = m_font->RenderText(str, Vector3(0.0f, 0.2f, 1.0f), m_fontSize);
		}

		// �������Ԃɉ����ĐF��ϊ�
		// Vector3 timeColor = Vector3(0.0f, tempTime / m_time->GetLimitTime(), m_time->GetLimitTime() / tempTime);
		// �t�H���g�̍X�V
		// m_timeTex = m_font->RenderText(str, timeColor, m_fontSize);

		// �e�̐���
		m_timeTexShadow = m_font->RenderText(str, Vector3(0.0f, 0.0f, 0.0f), m_fontSize);
		
	}

	if (m_time->GetCountState() == m_time->STATE_FINISH)
	{
		// �X�V���邽�߉��
		if (m_timeTex != nullptr) m_timeTex->Delete();
		if (m_timeTexShadow != nullptr) m_timeTexShadow->Delete();

		m_timeTex = m_font->RenderText("FINISH", Vector3(1.0f, 1.0f, 1.0f), m_fontSize);
		// �e
		m_timeTexShadow = m_font->RenderText("FINISH", Vector3(0.0f, 0.0f, 0.0f), m_fontSize);
	}

}

// �`�揈��
void CountDownUI::Draw(Shader * in_shader)
{
	// �t���[���[�h����"Skip"
	if (m_time->GetCountState() == m_time->STATE_FREE)
	{
		// �R���g���[���ڑ����Ɩ��ڑ����ŕ\����؂�ւ�
		if (CONTROLLER_INSTANCE.IsAvailable() == true)
		{
			//DrawTexture(in_shader, m_skipXpad, Vector2(0.0f, -RENDERER->GetScreenHeight() / 2.2f), 0.8f);
		}
		else
		{
			//DrawTexture(in_shader, m_skipKey, Vector2(0.0f, -RENDERER->GetScreenHeight() / 2.2f), 0.8f);
		}
	}

	// �������Ԃ̕`��
	if (m_timeTexShadow)
	{
		// �e
		DrawTexture(in_shader, m_timeTexShadow, Vector2(m_timeTexPos.x, m_timeTexPos.y - 3.0f), 1.0f);
	}
	if (m_timeTex)
	{
		// �{��
		DrawTexture(in_shader, m_timeTex, m_timeTexPos, 1.0f);
	}

	// "TIME"�̕`�� ���Q�[�����̂�
	if (m_time->GetCountState() == m_time->STATE_GAME)
	{
		for (int i = 0; i < 2; i++)
		{

			if (i == 0)
			{
				// �e
				DrawTexture(in_shader, m_timeLimitTex[i], Vector2(m_timeTexPos.x, m_timeTexPos.y + 57.0f), 1.0f);
			}

			if (i == 1)
			{
				// �e
				DrawTexture(in_shader, m_timeLimitTex[i], Vector2(m_timeTexPos.x, m_timeTexPos.y + 60.0f), 1.0f);
			}

		}
	}

	// �ǉ����ꂽ���Ԃ̕\��
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
