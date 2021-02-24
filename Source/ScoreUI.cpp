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
#include "Canvas.h"
#include "GameWorld.h"
#include <string>
#include <sstream>


// �R���X�g���N�^
ScoreUI::ScoreUI(Canvas* in_canvas)
	:m_canvas(in_canvas)
	,m_scoreTexPos(0.0f, 0.0f)
	,m_fontSize(64)
	,m_score(0)
	,m_addScore(0)
	,m_drawSubScore(false)
	,m_coolTime(2)
	,m_startSub(false)
{

	m_score = 500;
	// �X�R�A�i�[�p
	std::stringstream ssScore;
	std::stringstream ssSub;
	// ���[���̃X�R�A���e�L�X�g�ɕϊ�����
	ssScore << "$" << std::to_string(m_score);
	ssSub << "-$" << "50";
	// �X�R�A�p�̃t�H���g�𐶐� (0 = �e, 1 = �{��)
	for (int i = 0; i < 2; i++)
	{
		m_scoreTex[i] = m_font->RenderText(ssScore.str(), Vector3(1.0f, 1.0f, 1.0f) * i, m_fontSize);
		m_addScoreTex[i] = m_font->RenderText(ssScore.str(), Vector3(1.0f, 1.0f, 1.0f) * i, m_fontSize);
		m_subScoreTex[i] = m_font->RenderText(ssSub.str(), Vector3(0.0f, 0.0f, 1.0f) * i, m_fontSize);
	}
	// �\�����W�̐ݒ�
	m_scoreTexPos = Vector2(SCREEN_HALF_W - m_scoreTex[0]->GetHalfWidth() -3.0f, SCREEN_HALF_H - m_scoreTex[0]->GetHalfHeight() - 3.0f);
	m_addScoreTexPos = Vector2(SCREEN_HALF_W - m_addScoreTex[0]->GetHalfWidth() - 3.0f, m_scoreTexPos.y - m_addScoreTex[0]->GetHeight() - 3.0f);
	m_subScoreTexPos = Vector2(SCREEN_HALF_W - m_subScoreTex[0]->GetHalfWidth() - 3.0f, m_scoreTexPos.y - m_subScoreTex[0]->GetHeight() - 3.0f);
	// ���Ԃ�b�P�ʂŎ擾
	m_time = SDL_GetTicks() / 1000;

}


// �f�X�g���N�^
ScoreUI::~ScoreUI()
{

	if (m_scoreTex)
	{
		m_scoreTex[0]->Delete();
		m_scoreTex[1]->Delete();
	}

}


// �X�V����
void ScoreUI::Update(float in_deltaTime)
{
	// �X�R�A
	std::stringstream ssScore;

	// �X�R�A�����Z���ꂽ��
	if (m_addScore > 0)
	{
		AddScore();     // �X�R�A�̉��Z����

		// ���[���̃X�R�A���擾����
		ssScore << "$" << m_score;
		// �X�R�A�X�V�ɔ������O�̃e�N�X�`���폜�Ɛ�������
		for (int i = 0; i < 2; i++)
		{
			m_scoreTex[i]->Delete();
			m_scoreTex[i] = m_font->RenderText(ssScore.str(), Vector3(1.0f, 1.0f, 1.0f) * i, m_fontSize);
		}
		// �\�����W�̍X�V
		m_scoreTexPos = Vector2(SCREEN_HALF_W - m_scoreTex[0]->GetHalfWidth() - 3.0f, SCREEN_HALF_H - m_scoreTex[0]->GetHalfHeight() - 3.0f);

		// �e�N�X�`���𐶐�
		std::stringstream ssadd;
		ssadd << "+$" << m_addScore;
		// �X�R�A�X�V�ɔ������O�̃e�N�X�`���폜�Ɛ�������
		for (int i = 0; i < 2; i++)
		{
			m_addScoreTex[i]->Delete();
			m_addScoreTex[i] = m_font->RenderText(ssadd.str(), Vector3(0.0f, 1.0f, 0.7f) * i, 48);
		}

		m_addScoreTexPos = Vector2(SCREEN_HALF_W - m_addScoreTex[0]->GetHalfWidth() - 3.0f, m_scoreTexPos.y - m_addScoreTex[0]->GetHeight() - 3.0f);
	}

	// �X�R�A���Z����
	// �v���C���[���������Ă��鎞�̂�
	m_startSub = m_canvas->GetWorld()->GetFoundPlayer();
	if (m_startSub)
	{
		SubScore();

		// ���[���̃X�R�A���擾����
		ssScore << "$" << m_score;
		// �X�R�A�X�V�ɔ������O�̃e�N�X�`���폜�Ɛ�������
		for (int i = 0; i < 2; i++)
		{
			m_scoreTex[i]->Delete();
			m_scoreTex[i] = m_font->RenderText(ssScore.str(), Vector3(1.0f, 1.0f, 1.0f) * i, m_fontSize);
		}
		// �\�����W�̍X�V
		m_scoreTexPos = Vector2(SCREEN_HALF_W - m_scoreTex[0]->GetHalfWidth() - 3.0f, SCREEN_HALF_H - m_scoreTex[0]->GetHalfHeight() - 3.0f);
	}
	else
	{
		m_drawSubScore = false;
		m_time = SDL_GetTicks() / 1000;
	}
	
}


// �`�揈��
void ScoreUI::Draw(Shader * in_shader)
{

	// �X�R�A�̕`�� (0 = �e, 1 = �{��)
	for (int i = 0; i < 2; i++)
	{
		// ���݂̃X�R�A
		DrawTexture(in_shader, m_scoreTex[i], m_scoreTexPos + Vector2(3.0f, 3.0f) * i, 1.0f);
	}

	// ���Z���X�R�A�̕`�� (���Z�������̂�)
	if (m_addScore > 0)
	{
		for (int i = 0; i < 2; i++)
		{
			// ���݂̃X�R�A
			DrawTexture(in_shader, m_addScoreTex[i], m_addScoreTexPos + Vector2(3.0f, 3.0f) * i, 1.0f);
		}
	}
	// ���Z���̃X�R�A
	else if (m_addScore == 0 && m_drawSubScore)
	{
		for (int i = 0; i < 2; i++)
		{
			// ���݂̃X�R�A
			DrawTexture(in_shader, m_subScoreTex[i], m_subScoreTexPos + Vector2(3.0f, 3.0f) * i, 1.0f);
		}
	}
}

// ���Z�p�X�R�A���X�R�A�ɒǉ�����(+10����)
void ScoreUI::AddScore()
{
	// �X�R�A��10�����Z (�ǉ�����10��������Ă�����A���̂܂܃X�R�A�ɉ��Z��0��)
	if (m_addScore >= 10)
	{
		m_score += 10;
		m_addScore -= 10;
	}
	else
	{
		m_score += m_addScore;
		m_addScore = 0;
	}

	// ���Z����0�������Ȃ�
	if (m_addScore < 0)
	{
		m_addScore = 0;
	}
}

void ScoreUI::SubScore()
{
	// 3�b���ƂɌ��Z
	if (SDL_GetTicks() / 1000 > m_time + m_coolTime)
	{
		m_score -= 50;
		m_drawSubScore = true;
		m_time = SDL_GetTicks() / 1000;
	}

	if (SDL_GetTicks() / 1000 > m_time + 1)
	{
		m_drawSubScore = false;
	}

	// �X�R�A��0�������Ȃ�
	if (m_score < 0)
	{
		m_score = 0;
	}
}

// ���Z���X�R�A�̃Z�b�g�Ƃ���ɔ����e�N�X�`������
void ScoreUI::SetAddScore(int in_score)
{
	// ���Z����ǉ�
	m_addScore += in_score;
	// �e�N�X�`���𐶐�
	std::stringstream ssadd;
	ssadd << "+$" << in_score;
	// �X�R�A�X�V�ɔ������O�̃e�N�X�`���폜�Ɛ�������
	for (int i = 0; i < 2; i++)
	{
		m_addScoreTex[i]->Delete();
		m_addScoreTex[i] = m_font->RenderText(ssadd.str(), Vector3(0.0f, 1.0f, 0.7f) * i, 48);
	}

	m_addScoreTexPos = Vector2(SCREEN_HALF_W - m_addScoreTex[0]->GetHalfWidth() - 3.0f, m_scoreTexPos.y - m_addScoreTex[0]->GetHeight() - 3.0f);
}
