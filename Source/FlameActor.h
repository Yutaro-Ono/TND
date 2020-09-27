//-----------------------------------------------------------------------+
// ともし火(フィールドオブジェクト) クラス
// 
// 
// copyright (C) 2019 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#pragma once
#include "Actor.h"

class Player;

class FlameActor : public Actor
{


public:

	FlameActor();         // コンストラクタ
	~FlameActor();        // デストラクタ

	void Delete();

	void UpdateActor(float in_deltaTime) override;

	void SetSphereCollider(Vector3 in_pos, float in_radius);

	void SetFlamePosition(Vector3 in_pos);

	void SetFlameLife(const float& in_life) { m_life = in_life; }

	void CollisionCheck(Player* in_player);

	void ComputeWorldTransform2(Matrix4 in_mat);

private:

	class Particle* m_particle;
	
	class Texture* m_texture;

	class BoxCollider* m_hitBox;          // 当たり判定ボックス

	struct Sphere* m_hitSphere;                   // 当たり判定(球)

	Vector3 m_velocity;                   // 炎が舞い上がる際に用いる加速度

	float m_life;                         // 炎の発生から消失までの時間倍率

	// 行列関連
	Matrix4 m_view;
	Matrix4 m_projection;
	Matrix4 m_billboard;

};