//-----------------------------------------------------------------------+
// �X�R�A�N���X
// 
// 
// copyright (C) 2020 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#pragma once
#include "Actor.h"


class RuleScore : public Actor
{


public:

	RuleScore(class Player* in_target);                                            // �R���X�g���N�^
	~RuleScore();                                                                  // �f�X�g���N�^

	void UpdateActor(float in_deltaTime) override;                                 // �X�V����

	void AddScore(float in_scoreRate);                                             // �X�R�A�̉��Z

	const int& GetScore() const { return m_score; }                                // �X�R�A�̃Q�b�^�[

	const int& GetChain() const { return m_chain; }                                // �`�F�C���̃Q�b�^�[

	const float& GetChainTimer() const { return m_chainTimer; }                    // �`�F�C���^�C�}�[�̃Q�b�^�[

	const float& GetChainTimerLimit() const { return m_chainTimerLimit; }          // �`�F�C���^�C�}�[�̐������Ԃ̒l��Ԃ�

	const float& GetChainRate() const { return m_chainRate; }                      // �`�F�C�����ɂ��X�R�A�{����Ԃ�

	const int& GetPlayerRank() const { return m_playerRank; }                      // �Ƃ����Ύ擾���ɂ��v���C���[�����N�̎擾


private:


	class Player* m_target;                                         // �^�[�Q�b�g����v���C���[

	int m_score;                                                    // �X�R�A

	int m_chain;                                                    // �`�F�C����

	float m_chainRate;                                              // �`�F�C�����ɂ��X�R�A�{��

	float m_chainTimer;

	float m_chainTimerLimit;

	float m_scoreRate;                                              // �X�R�A�̔{��

	float m_playerAccelLimit;                                       // �v���C���[�̃A�N�Z�����E�l

	int m_playerRank;

	static const int SCORE_POINT;                                   // �X�R�A�̉��Z��l
};