#pragma once
#include "Actor.h"


class LandMarkIndicator : public Actor
{

public:

	LandMarkIndicator(class PlayerManager* in_player);
	~LandMarkIndicator();

	void UpdateActor(float in_deltaTime) override;

private:

	class PlayerManager* m_player;

};
