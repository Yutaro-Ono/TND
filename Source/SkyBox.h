#pragma once
#include "Actor.h"
#include "CubeMapComponent.h"

class SkyBox : public Actor
{
public:

	SkyBox();
	~SkyBox();

	void UpdateActor(float in_deltaTime) override;



private:

	class CubeMapComponent* cubeMapComp;



};