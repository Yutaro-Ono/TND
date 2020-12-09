#pragma once
#include "Component.h"
#include "Math.h"

class PatrolComponent : public Component
{

public:
	
	// 巡回状態
	enum PATROL_STATE
	{
		NONE,       // 何もしない
		PATROL,     // 通常巡回
		SEARCH,     // プレイヤーの索敵
		CHASE       // プレイヤーの追尾
	};


	PatrolComponent(class Helicopter* in_owner, class GameWorld* in_world);
	~PatrolComponent();

	void Update(float in_deltaTime) override;      // 更新処理

	void MoveToPatrolPos(float in_deltaTime);

	void ChasePlayer(float in_deltaTime);          // プレイヤー追跡

	void SetPatrolState(PatrolComponent::PATROL_STATE in_state) { m_patrolState = m_patrolState; }

private:


	PATROL_STATE m_patrolState;     // 巡回状態

	class Helicopter* m_heli;
	class GameWorld* m_world;
	
	class PatrolPoint* m_targetPoint;     // 巡回先のポイント
	Vector3 m_targetPos;                  // 巡回地点の座標


};