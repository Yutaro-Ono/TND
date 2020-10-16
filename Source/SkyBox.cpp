#include "SkyBox.h"
#include "CubeMapComponent.h"
#include <string>

SkyBox::SkyBox()
{
	cubeMapComp = new CubeMapComponent(this);
	cubeMapComp->CreateTexture("Data/Textures/TND/Skybox/night/");
	m_position = Vector3(100, 100, 100);
}

SkyBox::~SkyBox()
{
}

void SkyBox::UpdateActor(float in_deltaTime)
{

}
