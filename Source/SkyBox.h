#pragma once
#include "Actor.h"
#include "CubeMapComponent.h"
#include "Environment.h"

class SkyBox : public Actor
{
public:

	SkyBox();
	SkyBox(class Environment* in_environment, Environment::GAME_TIME in_gameTime);
	~SkyBox();

	void UpdateActor(float in_deltaTime) override;

	// スカイボックスのセット
	void SetSkyBox(Environment::GAME_TIME in_gameTime);

	class CubeMapComponent* GetCubeMapComp() { return m_cubeMapComp; }

private:

	class CubeMapComponent* m_cubeMapComp;

	class Environment* m_environmnet;

};