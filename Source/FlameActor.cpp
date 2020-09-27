//-----------------------------------------------------------------------+
// ともし火(フィールドオブジェクト) クラス
// 
// 
// copyright (C) 2019 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#include "FlameActor.h"
#include "GameMain.h"
#include "Renderer.h"
#include "Particle.h"
#include "ParticleManager.h"
#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"
#include "Collision.h"
#include "BoxCollider.h"
#include "Player.h"
#include "GetEffect.h"
#include "AudioManager.h"

FlameActor::FlameActor()
	:m_velocity(Vector3(1.0f, 1.0f, 1.0f))
	,m_life(1.0f)
	,m_hitSphere(nullptr)
{
	m_texture = RENDERER->GetTexture("Data/Shaders/fire.png");

	// パーティクルを生成
	m_particle = new Particle(m_position, m_velocity, 100.0f, 0.6f, 10.0f);
	m_particle->SetTextureID(m_texture->GetTextureID());
	m_particle->SetColor(Vector3(1.0f, 0.5f, 0.2f));
	m_particle->SetBlendMode(Particle::BLEND_ADD);

	SetScale(100.0f);

	// 効果音
	AUDIO->GetSound("Data/Music/SE/FC/Actors/Player/GetFlame/small_explosion2.wav");

}

FlameActor::~FlameActor()
{
	delete m_hitSphere;
}

void FlameActor::Delete()
{

}

void FlameActor::UpdateActor(float in_deltaTime)
{


	// パーティクルのセット　これはあとでパーティクルエミッタクラス作りたい。

	Vector3 randV((rand() % 100) / 10.0f, (rand() % 100) / 10.0f, (rand() % 100) / 10.0f);
	Vector3 Velocity = randV * 0.1f;
	Velocity.x += -0.5f;
	Velocity.y += -0.5f;
	Velocity.z += 2.5f;

	// 後にパーティクル発生用クラス作成する
	// 3フレームに1回　パーティクル発生
	if (GAME_INSTANCE.GetFrame() % 5 == 0)
	{
		for (int i = 0; i < 2; i++)
		{
			Vector3 pos;
			if (i % 2 == 0)
			{
				pos = m_position;
			}
			else
			{
				pos = m_position;
			}
			pos = pos + randV;

			Particle* p = new Particle(pos,
				Velocity,
				m_scale, 1.0f,
				randV.x * 1.5f * m_life);

			p->SetTextureID(m_texture->GetTextureID());
			p->SetColor(Vector3(0.4f, 0.1f, 0.0f));
			p->SetBlendMode(Particle::BLEND_ADD);
			RENDERER->GetParticleManager()->SetParticle(p);
		}
	}

}

void FlameActor::SetSphereCollider(Vector3 in_pos, float in_radius)
{
	m_hitSphere = new Sphere(in_pos, in_radius);
}

void FlameActor::SetFlamePosition(Vector3 in_pos)
{
	m_position = in_pos;
	m_particle->SetPosition(m_position);
}

void FlameActor::CollisionCheck(Player* in_player)
{
	// プレイヤーと衝突したら
	if (m_state != State::STATE_DEAD && m_hitSphere != nullptr && m_hitSphere->Contains(in_player->GetPosition()))
	{
		// 効果音
		AUDIO->PlaySound("Data/Music/SE/FC/Actors/Player/GetFlame/small_explosion2.wav");

		// プレイヤーのチェイン数を更新
		in_player->AddFlameChain();
		// プレイヤーのアクセル限界値を更新
		in_player->SetAccelLimit(2.5f);
		// エフェクトを生成
		GetEffect* effect = new GetEffect(in_player);
		// 死亡状態に
		SetState(State::STATE_DEAD);
	}
}

void FlameActor::ComputeWorldTransform2(Matrix4 in_mat)
{
	// スケーリング→回転→平行移動となるように変換行列を作成
	// m_worldTransform = Matrix4::CreateScale(m_scale);


	// Y軸回転
	// m_worldTransform *= Matrix4::CreateRotationY(in_radY);
	// Z軸回転
	// m_worldTransform *= Matrix4::CreateRotationX(in_radZ);


	// ポジション
	// m_worldTransform *= Matrix4::CreateTranslation(m_position);

	m_worldTransform = in_mat;

	// アクターが持っている全コンポーネントの変換を行う
	for (auto comp : m_components)
	{
		comp->OnUpdateWorldTransform();
	}
}
