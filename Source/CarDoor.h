#pragma once
#include "Actor.h"
#include "PlayerCar.h"
#include <string>

class CarDoor : public Actor
{
public:

	enum DOOR_POS
	{
		LEFT = 0,
		RIGHT
	};

	CarDoor(class PlayerCar* in_owner, DOOR_POS in_pos);
	~CarDoor();

	void UpdateActor(float in_deltaTime) override;


private:

	class PlayerCar* m_owner;

	Vector3 m_adjustPos;                    // �h�A�̈ʒu��␳����ʒu�x�N�g��
  
	static const std::string CAR_DOOR_RIGHT_FRAME_MESH_PATH;
	static const std::string CAR_DOOR_RIGHT_GLASS_MESH_PATH;
	static const std::string CAR_DOOR_LEFT_FRAME_MESH_PATH;
	static const std::string CAR_DOOR_LEFT_GLASS_MESH_PATH;
};