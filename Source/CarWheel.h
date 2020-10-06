#pragma once
#include "Actor.h"
#include "PlayerCar.h"
#include <string>

class CarWheel : public Actor
{

public:

	// 車体におけるタイヤの位置
	typedef enum WHEEL_POSITION
	{
		FRONT_LEFT = 0,
		FRONT_RIGHT,
		BACK_LEFT,
		BACK_RIGHT,
		ALL_WHEEL_NUM
	};

	CarWheel(class PlayerCar* in_owner, const std::string& in_meshPath, WHEEL_POSITION in_enumPos);
	~CarWheel();
	void UpdateActor(float in_deltaTime) override;

	class MeshComponent* GetMeshComponent() { return m_meshComp; }     // メッシュへのポインタゲッター


	//-------------------------------------------------+
	// Getter/Setter
	//-------------------------------------------------+


private:

	WHEEL_POSITION m_wheelPosition;          // タイヤが車体のどの位置にあるか

	class PlayerCar* m_owner;                // オーナークラス(追従するクラス)

	class MeshComponent* m_meshComp;         // メッシュコンポーネント

	float m_playerRadian;                    // プレイヤーの弧度監視用

	Vector3 m_adjustPos;                     // タイヤ用位置調整ベクトル
};

