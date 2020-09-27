#pragma once
#include "Actor.h"
#include <vector>

class GetEffect : public Actor
{

public:

	typedef enum          // �G�t�F�N�g�̃����N(�����قǃp�[�e�B�N���ʂ�������)
	{
		RANK_1,
		RANK_2,
		RANK_3,

		RANK_ALL
	}EFFECT_RANK;

	GetEffect(class Player* in_target);
	~GetEffect();

	void UpdateActor(float in_deltaTime) override;


private:

	std::vector<class Particle*> m_effects;          // �擾���G�t�F�N�g

	EFFECT_RANK m_effectRank;

	class Player* m_targetPlayer;                    // �^�[�Q�b�g����v���C���[

	float m_effectVal[EFFECT_RANK::RANK_ALL];          // ���ׂẴ����N���̃G�t�F�N�g��

	float m_speed;                                   // �G�t�F�N�g���΂��X�s�[�h

	float m_angle;                                   // �G�t�F�N�g���΂��ۂ̊p�x

	float m_timer;

};