#pragma once
#include "Actor.h"
#include "PlayerCar.h"
#include <string>

class CarWheel : public Actor
{

public:

	CarWheel(class PlayerCar* in_owner, const std::string& in_meshPath);
	~CarWheel();
	void UpdateActor(float in_deltaTime) override;

	class MeshComponent* GetMeshComponent() { return m_meshComp; }     // メッシュへのポインタゲッター

private:

	class PlayerCar* m_owner;

	class MeshComponent* m_meshComp;

};

