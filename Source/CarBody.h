#pragma once
#include "Actor.h"
#include "PlayerCar.h"
#include <string>

class CarBody : public Actor
{

public:


	CarBody(class PlayerCar* in_owner);
	~CarBody();

	void UpdateActor(float in_deltaTime) override;

	class MeshComponent* GetMeshComponent() { return m_meshComp; }     // メッシュへのポインタゲッター


private:

	class PlayerCar* m_owner;

	static const std::string CAR_BODY_MESH_PATH;
	static const std::string CAR_GLASS_MESH_PATH;
	static const std::string CAR_INTERIOR_MESH_PATH;
};