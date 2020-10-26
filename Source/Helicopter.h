#pragma once
#include "Actor.h"
#include <string>
#include "Collision.h"

class Helicopter : public Actor
{

public:

	Helicopter(class GameWorld* in_world, const Vector3& in_pos);              // コンストラクタ
	~Helicopter();             // デストラクタ

	void UpdateActor(float in_deltaTime) override;


	//---------------------------------------------------+
	// Getter / Setter
	//---------------------------------------------------+
	class GameWorld* GetWorld() { return m_world; }


private:

	// ワールドへのポインタ
	class GameWorld* m_world;

	// 巡回コンポーネント
	class PatrolComponent* m_patrolComp;

	// 機体の部品クラス
	class HeliBody* m_body;                // ボディ
	class HeliRotorMain* m_mainRotor;      // メインローター
	class HeliRotorBack* m_backRotor;      // バックローター
	class HeliSpotLight* m_spotLight;      // ライト

	Sphere m_searchSphere;                 // 索敵範囲球
};