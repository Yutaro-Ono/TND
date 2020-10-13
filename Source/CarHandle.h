#pragma once
#include "Actor.h"
#include "PlayerCar.h"
#include <string>

class CarHandle : public Actor
{

public:

	CarHandle(class PlayerCar* in_owner, const std::string& in_meshPath);
	~CarHandle();
	void UpdateActor(float in_deltaTime) override;


	class MeshComponent* GetMeshComponent() { return m_meshComp; }     // ���b�V���ւ̃|�C���^�Q�b�^�[

private:


	class PlayerCar* m_owner;

};

