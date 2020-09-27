#pragma once
#include "Actor.h"


class SandSmoke : public Actor
{


public:

	enum OccurreState
	{
		DISABLE,
		ACTIVE
	};

	SandSmoke(class Player* in_target);
	~SandSmoke();

	void UpdateActor(float in_deltaTime) override;

	// 発生ステート
	void SetOccurre(OccurreState in_state) { m_state = in_state; }


private:

	class Particle* m_particle;

	class Texture* m_texture;

	class Player* m_target;

	OccurreState m_state;


};