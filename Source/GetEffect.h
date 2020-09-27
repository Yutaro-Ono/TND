#pragma once
#include "Actor.h"
#include <vector>

class GetEffect : public Actor
{

public:

	typedef enum          // エフェクトのランク(高いほどパーティクル量が増える)
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

	std::vector<class Particle*> m_effects;          // 取得時エフェクト

	EFFECT_RANK m_effectRank;

	class Player* m_targetPlayer;                    // ターゲットするプレイヤー

	float m_effectVal[EFFECT_RANK::RANK_ALL];          // すべてのランク分のエフェクト量

	float m_speed;                                   // エフェクトを飛ばすスピード

	float m_angle;                                   // エフェクトを飛ばす際の角度

	float m_timer;

};