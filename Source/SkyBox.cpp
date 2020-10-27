#include "SkyBox.h"
#include "CubeMapComponent.h"
#include <string>


SkyBox::SkyBox()
{
	m_cubeMapComp = new CubeMapComponent(this);
	m_cubeMapComp->CreateTexture("Data/Textures/TND/Skybox/night/");
}

SkyBox::SkyBox(Environment* in_environment, Environment::GAME_TIME in_gameTime)
	:m_environmnet(in_environment)
{
	SetSkyBox(in_gameTime);
	m_position = Vector3(100, 100, 100);

}

SkyBox::~SkyBox()
{
}

void SkyBox::UpdateActor(float in_deltaTime)
{

}

// 時間帯ごとにスカイボックスを生成
void SkyBox::SetSkyBox(Environment::GAME_TIME in_gameTime)
{
	if (in_gameTime == Environment::GAME_TIME::MORNING)
	{
		m_cubeMapComp = new CubeMapComponent(this);
		m_cubeMapComp->CreateTexture("Data/Textures/TND/Skybox/morning/");
		return;
	}
	if (in_gameTime == Environment::GAME_TIME::AFTERNOON)
	{
		m_cubeMapComp = new CubeMapComponent(this);
		m_cubeMapComp->CreateTexture("Data/Textures/TND/Skybox/morning/");
		return;
	}
	if (in_gameTime == Environment::GAME_TIME::EVENING)
	{
		m_cubeMapComp = new CubeMapComponent(this);
		m_cubeMapComp->CreateTexture("Data/Textures/TND/Skybox/sunset/");
		return;
	}
	if (in_gameTime == Environment::GAME_TIME::NIGHT)
	{
		m_cubeMapComp = new CubeMapComponent(this);
		m_cubeMapComp->CreateTexture("Data/Textures/TND/Skybox/night/");
		return;
	}
}
