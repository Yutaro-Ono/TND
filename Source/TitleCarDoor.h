#pragma once
#include "Actor.h"
#include <string>

class TitleCarDoor : public Actor
{
public:

	enum DOOR_POS
	{
		LEFT = 0,
		RIGHT
	};

	TitleCarDoor(class Actor* in_owner, DOOR_POS in_pos);
	~TitleCarDoor();

	void UpdateActor(float in_deltaTime) override;


private:

	class Actor* m_owner;

	Vector3 m_adjustPos;                    // ドアの位置を補正する位置ベクトル
  
	static const std::string CAR_DOOR_RIGHT_FRAME_MESH_PATH;
	static const std::string CAR_DOOR_RIGHT_GLASS_MESH_PATH;
	static const std::string CAR_DOOR_LEFT_FRAME_MESH_PATH;
	static const std::string CAR_DOOR_LEFT_GLASS_MESH_PATH;
};