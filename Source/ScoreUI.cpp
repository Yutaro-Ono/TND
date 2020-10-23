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
ScoreUI::ScoreUI()
	:m_scoreTexPos(0.0f, 0.0f)
	,m_fontSize(64)
{
	// �X�R�A�̏�����
	m_prevScore = m_nowScore = 0;

	// �X�R�A�i�[�p
	std::stringstream ssScore;

	// ���[���̃X�R�A���e�L�X�g�ɕϊ�����
	ssScore << "$" << std::to_string(m_nowScore);

	// �X�R�A�p�̃t�H���g�𐶐�
	m_scoreTex = m_font->RenderText(ssScore.str(), Vector3(1.0f, 1.0f, 1.0f), m_fontSize);
	// �e�𐶐�
	m_scoreTexShadow = m_font->RenderText(ssScore.str(), Vector3(0.0f, 0.0f, 0.0f), m_fontSize);

	// �\�����W�̐ݒ�
	m_scoreTexPos = Vector2(GAME_CONFIG->GetScreenWidth() / 2 - m_scoreTex->GetWidth() / 2, RENDERER->GetScreenHeight() / 1.9f - m_scoreTex->GetHeight());


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

	// �X�R�A���ϓ�������
	if (m_prevScore != m_nowScore)
	{
		// ���[���̃X�R�A���擾����
		ssScore << "$" << m_nowScore;

		// �������
		m_scoreTex->Delete();
		m_scoreTexShadow->Delete();


		// �`�悷��t�H���g�𐶐�
		m_scoreTex = m_font->RenderText(ssScore.str(), Vector3(1.0f, 1.0f, 1.0f), m_fontSize);
		// �e�𐶐�
		m_scoreTexShadow = m_font->RenderText(ssScore.str(), Vector3(0.0f, 0.0f, 0.0f), m_fontSize);
		// �\�����W�̍X�V
		m_scoreTexPos = Vector2(GAME_CONFIG->GetScreenWidth() / 2 - m_scoreTex->GetWidth() / 2, RENDERER->GetScreenHeight() / 1.9f - m_scoreTex->GetHeight());

		// �����l�ɂ���
		m_prevScore = m_nowScore;
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


}
