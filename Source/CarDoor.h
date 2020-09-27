#pragma once
#include "Actor.h"
#include "PlayerCar.h"
#include <string>

class CarDoor : public Actor
{
public:

	CarDoor(class PlayerCar* in_owner, const std::string& in_meshPath);
	~CarDoor();

	void UpdateActor(float in_deltaTime) override;

	class MeshComponent* GetMeshComponent() { return m_meshComp; }     // メッシュへのポインタゲッター


private:

	class PlayerCar* m_owner;

	class MeshComponent* m_meshComp;

	

	static const Vector3 ADJUST_DOOR_POSITION;     // ドアの位置を補正する位置ベクトル
};