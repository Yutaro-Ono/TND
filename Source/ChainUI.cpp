//-----------------------------------------------------------------------+
// �`�F�C��UI�N���X.
// 
// 
// copyright (C) 2020 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#include "ChainUI.h"
#include "GameMain.h"
#include "Renderer.h"
#include "Texture.h"
#include "RuleScore.h"
#include "Font.h"
#include "SpriteComponent.h"

#include <sstream>


// �R���X�g���N�^
ChainUI::ChainUI(RuleScore * in_rule)
	:m_rule(in_rule)
	,m_chainNum(0)
	,m_rank(0)
	,m_timeGauge(nullptr)
	,m_gaugeWidth(0)
{

	// �t�H���g�e�N�X�`���̃��[�h
	for (int i = 0; i < 2; i++)
	{
		if (i == 0)
		{
			m_chain[i] = m_font->RenderText("0 CHAIN", Vector3(0.0f, 0.0f, 0.0f), 48);
			m_rankTex[i] = m_font->RenderText("RANK", Vector3(0.0f, 0.0f, 0.0f), 30);
			m_rankNum[i] = m_font->RenderText("0", Vector3(0.0f, 0.0f, 0.0f), 72);
		}

		if (i == 1)
		{
			m_chain[i] = m_font->RenderText("0 CHAIN", Vector3(1.0f, 0.3f, 0.0f), 48);
			m_rankTex[i] = m_font->RenderText("RANK", Vector3(1.0f, 1.0f, 1.0f), 30);
			m_rankNum[i] = m_font->RenderText("0", Vector3(1.0f, 1.0f, 1.0f), 72);
		}

	}

	// �^�C�}�[�Q�[�W�̃e�N�X�`���擾
	m_timeGauge = RENDERER->GetTexture("Data/Interface/HUD/TimerGauge/TimerGauge.png");
	m_timeGauge->SetWidth(1024);                   // �Q�[�W�̃T�C�Y��������
	m_gaugeWidth = m_timeGauge->GetWidth();

}


// �f�X�g���N�^
ChainUI::~ChainUI()
{
	for (int i = 0; i < 2; i++)
	{
		m_chain[i]->Delete();
		// m_chainRank[i]->Delete();
		// m_rank[i]->Delete();
	}

}


// �X�V����
void ChainUI::Update(float in_deltaTime)
{
	// �`�F�C�����i�[
	std::stringstream ssChain;

	// �`�F�C���������[���N���X����擾
	int nowChain = m_rule->GetChain();

	// �`�F�C�����𕶎���Ƃ��ďo��
	ssChain << nowChain - 1 << " CHAIN";




	// �`�F�C�����ɕύX����������X�V
	if (m_chainNum != nowChain)
	{
		// �t�H���g�̐���
		for (int i = 0; i < 2; i++)
		{
			// �|�C���^�������ꂩ���w���Ă����ꍇ
			if (m_chain[i] != nullptr)
			{
				// �t�H���g�̉������
				m_chain[i]->Delete();
			}


			// �e
			if (i == 0)
			{
				m_chain[i] = m_font->RenderText(ssChain.str(), Vector3(1.0f, 0.3f, 0.0f), 60);
			}
			
			// �{��
			if (i == 1)
			{
				m_chain[i] = m_font->RenderText(ssChain.str(), Vector3(1.0f, 0.8f, 0.6f), 60);
			}
		}

		// �`�F�C�����X�V
		m_chainNum = nowChain;
	}

	//------------------------------------------------------------------------+
	// �����N
	//------------------------------------------------------------------------+
	// �����N���i�[
	std::stringstream ssRank;

	int nowRank = m_rule->GetPlayerRank();

	if (m_rank != nowRank)
	{
		ssRank << nowRank;

		for (int i = 0; i < 2; i++)
		{
			m_rankNum[i]->Delete();


			// �e
			if (i == 0)
			{
				m_rankNum[i] = m_font->RenderText(ssRank.str(), Vector3(0.0f, 0.0f, 0.0f), 72);
			}

			// �{��
			if (i == 1)
			{
				m_rankNum[i] = m_font->RenderText(ssRank.str(), Vector3(1.0f, 1.0f, 1.0f), 72);
			}
		}

		m_rank = nowRank;

	}

	// �^�C���Q�[�W�̉����̍X�V
	m_timeGauge->SetWidth(m_gaugeWidth * (m_rule->GetChainTimer() / m_rule->GetChainTimerLimit()));

}


// �`�揈��
void ChainUI::Draw(Shader * in_shader)
{
	// �e�t���`��
	for (int i = 0; i < 2; i++)
	{
		// �`�F�C��
		if (m_chain[i] && (m_chainNum - 1) >= 1)
		{

			for (int i = 0; i < 2; i++)
			{
				// �e
				if (i == 0)
				{
					DrawTexture(in_shader, m_chain[i], Vector2(0.0f, RENDERER->GetScreenHeight() / 4.1f - 3.0f), 1.0f);
				}

				// �{��
				if (i == 1)
				{
					DrawTexture(in_shader, m_chain[i], Vector2(0.0f, RENDERER->GetScreenHeight() / 4.1f), 1.0f);
				}

			}

		}


		// �����N
		// �e
		if (i == 0)
		{
			DrawTexture(in_shader, m_rankTex[i], Vector2(0.0f, -RENDERER->GetScreenHeight() / 4.5f - 3.0f), 1.0f);
		}

		// �{��
		if (i == 1)
		{
			DrawTexture(in_shader, m_rankTex[i], Vector2(0.0f, -RENDERER->GetScreenHeight() / 4.5f), 1.0f);
		}

		// �����N(����)
		// �e
		if (i == 0)
		{
			DrawTexture(in_shader, m_rankNum[i], Vector2(0.0f, -RENDERER->GetScreenHeight() / 3.6f - 3.0f), 1.0f);
		}

		// �{��
		if (i == 1)
		{
			DrawTexture(in_shader, m_rankNum[i], Vector2(0.0f, -RENDERER->GetScreenHeight() / 3.6f), 1.0f);
		}
	}

	// �`�F�C���Q�[�W�̕`��
	if (m_timeGauge && m_rule->GetChainTimer() > 0.0f && m_rule->GetChain() > 0.0f)
	{
		DrawTexture(in_shader, m_timeGauge, Vector2(0.0f, RENDERER->GetScreenHeight() / 4.5f - 30.0f), 0.7f);
	}

}
