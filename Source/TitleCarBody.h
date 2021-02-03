#pragma once
#include "Actor.h"
#include "PlayerCar.h"
#include <string>

class TitleCarBody : public Actor
{

public:


	TitleCarBody(class Actor* in_owner);
	~TitleCarBody();

	void UpdateActor(float in_deltaTime) override;

	class MeshComponent* GetMeshComponent() { return m_meshComp; }     // メッシュへのポインタゲッター


private:

	class Actor* m_owner;

	class PointLight* m_frontLight[2];
	class PointLight* m_backLight[2];



	class SpotLight* m_spotLight;

	static const std::string CAR_BODY_MESH_PATH;
	static const std::string CAR_GLASS_MESH_PATH;
	static const std::string CAR_FRONTLIGHT_MESH_PATH;
	static const std::string CAR_BACKLIGHT_MESH_PATH;
	static const std::string CAR_INTERIOR_MESH_PATH;
};