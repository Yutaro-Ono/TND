#pragma once
#include "Actor.h"
#include "PlayerCar.h"
#include <string>

class CarBody : public Actor
{

public:


	CarBody(class PlayerCar* in_owner, const std::string& in_meshPath);
	~CarBody();

	void UpdateActor(float in_deltaTime) override;

	class MeshComponent* GetMeshComponent() { return m_meshComp; }     // メッシュへのポインタゲッター


private:

	class PlayerCar* m_owner;

	class MeshComponent* m_meshComp;

};