#pragma once
#include "Actor.h"


class LandMarkIndicator : public Actor
{

public:

	LandMarkIndicator(class PlayerCar* in_car);
	~LandMarkIndicator();

	void UpdateActor(float in_deltaTime) override;

private:

	class PlayerCar* m_car;

	class WorldSpaceUI* m_ui;

};
