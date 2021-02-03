#pragma once
#include "Actor.h"
#include "Actor.h"
#include <string>

class TitleCarWheel : public Actor
{

public:

	// 車体におけるタイヤの位置
	enum WHEEL_POSITION
	{
		FRONT_LEFT = 0,
		FRONT_RIGHT,
		BACK_LEFT,
		BACK_RIGHT,
		ALL_WHEEL_NUM
	};

	TitleCarWheel(class Actor* in_owner, WHEEL_POSITION in_enumPos);
	~TitleCarWheel();
	void UpdateActor(float in_deltaTime) override;



	//-------------------------------------------------+
	// Getter/Setter
	//-------------------------------------------------+
	class Actor* GetCarPtr() { return m_owner; }

	void SetSpin(bool in_spin) { m_spin = in_spin; }
	bool GetSpin() { return m_spin; }

private:

	WHEEL_POSITION m_wheelPosition;          // タイヤが車体のどの位置にあるか

	class Actor* m_owner;                // オーナークラス(追従するクラス)

	Vector3 m_adjustPos;                     // タイヤ用位置調整ベクトル

	bool m_spin;                             // 回転するかしないか

	static const std::string CAR_WHEEL_RUBBER_MESH_PATH;
	static const std::string CAR_WHEEL_STEEL_MESH_PATH;
};

