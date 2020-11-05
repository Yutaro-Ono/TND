#pragma once
#include "Actor.h"
#include <string>
#include "Collision.h"

class Helicopter : public Actor
{

public:

	Helicopter(class GameWorld* in_world, const Vector3& in_pos, int in_num);              // コンストラクタ
	~Helicopter();                                                             // デストラクタ

	void UpdateActor(float in_deltaTime) override;

	void SearchPlayer(class PlayerManager* in_player);

	//---------------------------------------------------+
	// Getter / Setter
	//---------------------------------------------------+
	class GameWorld* GetWorld()  { return m_world; }
	bool  GetFoundPlayer()       { return m_foundPlayer; }

	int GetNumber() { return m_number; }

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

	int m_number;                          // ワールド上でのヘリの番号

	bool m_foundPlayer;                    // プレイヤーを見つけたかどうか
	Sphere m_searchSphere;                 // 索敵範囲球
};