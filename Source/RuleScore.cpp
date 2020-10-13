//-----------------------------------------------------------------------+
// �X�R�A�N���X
// 
// 
// copyright (C) 2020 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#include "RuleScore.h"


const int RuleScore::SCORE_POINT = 100;                // ���Z�����X�R�A�p�|�C���g�̊�l

// �R���X�g���N�^
RuleScore::RuleScore(Player * in_target)
	:m_target(in_target)
	,m_score(0)
	,m_chain(0)
	,m_chainRate(0.0f)
	,m_chainTimer(0.0f)
	,m_chainTimerLimit(0.0f)
	,m_playerRank(0)
	,m_scoreRate(1.0f)
{
	//m_playerAccelLimit = m_target->GetAccelLimit();

	//m_chainTimerLimit = m_target->GetChainTimerLimit();

	//m_playerRank = m_target->GetSpeedRank();
}

// �f�X�g���N�^
RuleScore::~RuleScore()
{
}

// �X�V����
void RuleScore::UpdateActor(float in_deltaTime)
{
	//// �v���C���[�̃A�N�Z���̌��E�l���X�V
	//float prevPlayerAccelLimit = m_target->GetAccelLimit();

	//// �^�C�}�[�̎擾
	//m_chainTimer = m_target->GetChainTimer();

	//// �v���C���[�������擾���������Ă�����
	//if (m_playerAccelLimit != prevPlayerAccelLimit)
	//{
	//	AddScore(m_scoreRate);
	//	m_playerAccelLimit = prevPlayerAccelLimit;
	//}

	////--------------------------------------------------------+
	//// �`�F�C���̍X�V
	////--------------------------------------------------------+
	//int nowChain = m_target->GetFlameChain();

	//// �`�F�C�����̍X�V
	//if (m_chain != nowChain)
	//{
	//	m_chain = nowChain;

	//	// �`�F�C�����ɉ������X�R�A���[�g(50�`�F�C�����A��2�{��)
	//	m_chainRate = 1.0f + ((m_chain - 1) * 0.125);
	//}

	//// �`�F�C�����[�g�̐��K��
	//if (m_chain == 0)
	//{
	//	m_chainRate = 1.0f;
	//}

	//// �X�R�A���[�g���v���C���[�̑��x�ɉ����čX�V
	//m_scoreRate = m_target->GetAccel() / m_target->GetAccelMax();

	////--------------------------------------------------------+
	//// �����N�̍X�V
	////--------------------------------------------------------+
	//int m_nowRank = m_target->GetSpeedRank();

	//if (m_playerRank != m_nowRank)
	//{
	//	// �����N�X�V
	//	m_playerRank = m_nowRank;
	//}


}

// �X�R�A�̉��Z
void RuleScore::AddScore(float in_scoreRate)
{
	m_score += SCORE_POINT * m_chainRate;
}
