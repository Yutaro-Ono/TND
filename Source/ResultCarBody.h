#pragma once
#include "Actor.h"
#include "PlayerCar.h"
#include <string>

class ResultCarBody : public Actor
{

public:


	ResultCarBody(class Actor* in_owner);
	~ResultCarBody();

	void UpdateActor(float in_deltaTime) override;

	class MeshComponent* GetMeshComponent() { return m_meshComp; }     // メッシュへのポインタゲッター


private:

	class Actor* m_owner;

	static const std::string CAR_BODY_MESH_PATH;
	static const std::string CAR_GLASS_MESH_PATH;
	static const std::string CAR_FRONTLIGHT_MESH_PATH;
	static const std::string CAR_BACKLIGHT_MESH_PATH;
	static const std::string CAR_INTERIOR_MESH_PATH;
};