#pragma once
#include "Actor.h"


class FlameSpark : public Actor
{


public:

	enum OccurreState
	{
		DISABLE,
		ACTIVE
	};

	FlameSpark(class Player* in_target);
	~FlameSpark();

	void UpdateActor(float in_deltaTime) override;



private:


	class Particle* m_particle;

	class Texture* m_texture;

	class Player* m_target;

	OccurreState m_state;

};