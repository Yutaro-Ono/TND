#pragma once
#include "Actor.h"
#include "PlayerCar.h"
#include <string>

class CarDoor : public Actor
{
public:

	CarDoor(class PlayerCar* in_owner, const std::string& in_meshPath, bool in_leftRight);
	~CarDoor();

	void UpdateActor(float in_deltaTime) override;

	class MeshComponent* GetMeshComponent() { return m_meshComp; }     // ���b�V���ւ̃|�C���^�Q�b�^�[


private:

	class PlayerCar* m_owner;

	Vector3 m_adjustPos;                    // �h�A�̈ʒu��␳����ʒu�x�N�g��
  
};