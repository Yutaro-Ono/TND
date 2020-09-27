//-----------------------------------------------------------------------+
// �X�R�AUI�N���X.
// 
// 
// copyright (C) 2020 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#include "ScoreUI.h"
#include "GameMain.h"
#include "Renderer.h"
#include "Texture.h"
#include "Font.h"
#include "RuleScore.h"

#include <string>
#include <sstream>


// �R���X�g���N�^
ScoreUI::ScoreUI(class RuleScore* in_rule)
	:m_ruleScore(in_rule)
	,m_scoreTexPos(0.0f, 0.0f)
	,m_fontSize(64)
{
	// �X�R�A�i�[�p
	std::string strScore;
	std::stringstream ssScore;

	// ���[���̃X�R�A���擾���A�e�L�X�g�ɕϊ�����
	strScore = std::to_string(m_ruleScore->GetScore());
	ssScore << strScore;
	strScore = ssScore.str();

	// �X�R�A�p�̃t�H���g�𐶐�
	m_scoreTex = m_font->RenderText(strScore, Vector3(1.0f, 1.0f, 1.0f), m_fontSize);
	// �e�𐶐�
	m_scoreTexShadow = m_font->RenderText(strScore, Vector3(0.0f, 0.0f, 0.0f), m_fontSize);

	// �X�R�A���[�g�p�̃t�H���g�𐶐�
	for (int i = 0; i < 2; i++)
	{
		// �e
		if (i == 0)
		{
			m_scoreRateTex[i] = m_font->RenderText("x1", Vector3(0.0f, 0.0f, 0.0f), m_fontSize / 2);
		}
		// �{��
		if (i == 1)
		{
			m_scoreRateTex[i] = m_font->RenderText("x1", Vector3(1.0f, 1.0f, 1.0f), m_fontSize / 2);
		}
	}

	// �\�����W�̐ݒ�
	// m_scoreTexPos = Vector2(RENDERER->GetScreenWidth() - m_scoreTex->GetWidth(), m_scoreTex->GetHeight());
	m_scoreTexPos = Vector2(0.0f, -RENDERER->GetScreenHeight() / 1.9f + m_scoreTex->GetHeight());

	// �X�R�A�̊Ď�
	m_prevScore = m_ruleScore->GetScore();
	m_nowScore = m_prevScore;
}


// �f�X�g���N�^
ScoreUI::~ScoreUI()
{

	if (m_scoreTex)
	{
		m_scoreTex->Delete();
	}


	if (m_scoreTexShadow)
	{
		m_scoreTexShadow->Delete();
	}

}


// �X�V����
void ScoreUI::Update(float in_deltaTime)
{
	// �X�R�A
	std::stringstream ssScore;

	// ���t���[���X�R�A���Ď�
	m_nowScore = m_ruleScore->GetScore();

	// �X�R�A���ϓ�������
	if (m_prevScore != m_nowScore)
	{
		// ���[���̃X�R�A���擾����
		ssScore << m_ruleScore->GetScore();


		// �������
		m_scoreTex->Delete();
		m_scoreTexShadow->Delete();


		// �`�悷��t�H���g�𐶐�
		m_scoreTex = m_font->RenderText(ssScore.str(), Vector3(1.0f, 1.0f, 1.0f), m_fontSize);
		// �e�𐶐�
		m_scoreTexShadow = m_font->RenderText(ssScore.str(), Vector3(0.0f, 0.0f, 0.0f), m_fontSize);

		// �����l�ɂ���
		m_prevScore = m_nowScore;
	}


	// �X�R�A���[�g(�`�F�C��)
	std::stringstream ssRate;
	// ���[���̃`�F�C�����[�g���擾����
	ssRate << "x" << m_ruleScore->GetChainRate();

	// ���[�g�p�t�H���g�𐶐�
	for (int i = 0; i < 2; i++)
	{
		m_scoreRateTex[i]->Delete();

		if (i == 0)
		{
			m_scoreRateTex[i] = m_font->RenderText(ssRate.str(), Vector3(0.0f, 0.0f, 0.0f), m_fontSize / 2);
		}

		if (i == 1)
		{
			m_scoreRateTex[i] = m_font->RenderText(ssRate.str(), Vector3(1.0f, 1.0f, 1.0f), m_fontSize / 2);
		}

	}

}


// �`�揈��
void ScoreUI::Draw(Shader * in_shader)
{

	// �X�R�A�̕`��
	if (m_scoreTexShadow)
	{
		// �e
		DrawTexture(in_shader, m_scoreTexShadow, Vector2(m_scoreTexPos.x + 3.0f, m_scoreTexPos.y - 3.0f), 1.0f);
	}
	if (m_scoreTex)
	{
		// �{��
		DrawTexture(in_shader, m_scoreTex, m_scoreTexPos, 1.0f);
	}

	for (int i = 0; i < 2; i++)
	{
		if (m_scoreRateTex[i])
		{
			DrawTexture(in_shader, m_scoreRateTex[i], Vector2(m_scoreTex->GetWidth() + m_scoreTexPos.x + 3.0f, m_scoreTexPos.y));
		}
	}


}
