#include "SandSmoke.h"
#include "GameMain.h"
#include "Renderer.h"
#include "Texture.h"
#include "Player.h"
#include "Particle.h"
#include "ParticleManager.h"

// コンストラクタ
SandSmoke::SandSmoke(class Player* in_target)
	:m_state(DISABLE)
	,m_target(in_target)
{
	// テクスチャ
	m_texture = RENDERER->GetTexture("Data/Shaders/fire.png");
	
	// スケールを最適化
	SetScale(100.0f);

}

// デストラクタ
SandSmoke::~SandSmoke()
{
}

// 更新処理
void SandSmoke::UpdateActor(float in_deltaTime)
{


	// ターゲットしたプレイヤーのアクセル値がブレーキ値より20以上、ブレーキ値がアクセル値より10以上の時
	if ((m_target->GetAccel() - m_target->GetBrake()) >= 20.0f || (m_target->GetBrake() - m_target->GetAccel()) > 5.0f)
	{
		// パーティクル発生をアクティブに
		m_state = ACTIVE;
		
		// アクセルの値に応じてスケールを拡大
		m_scale = 100.0f * (m_target->GetAccel() / 100.0f) * 1.2f;

	}
	else
	{
		m_state = DISABLE;
	}


	// パーティクルが発生している・発生していない
	switch (m_state)
	{

		// 発生していない
	case DISABLE:

		break;

		// 発生している
	case ACTIVE:

		// プレイヤーポジションを取得
		m_position = m_target->GetPosition();
		m_position.z -= 70.0f;


		// ランダムな値を設定
		Vector3 randV((rand() % 100) / 10.0f, (rand() % 100) / 10.0f, (rand() % 100) / 10.0f);
		Vector3 Velocity = randV * 0.1f;
		Velocity.x += -0.5f;
		Velocity.y += -0.0f;
		Velocity.z += 2.5f;

		// パーティクル発生
		// 3フレームに1回　パーティクル発生
		if (GAME_INSTANCE.GetFrame() % 5 == 0)
		{
			for (int i = 0; i < 2; i++)
			{
				// 発生位置を設定
				Vector3 pos = m_position;
				// ランダムな値を足す
				pos = pos + randV;

				// パーティクルを生成
				Particle* p = new Particle(pos,
					Velocity,
					m_scale, 0.5f,
					randV.x * 0.09f);

				p->SetTextureID(m_texture->GetTextureID());
				p->SetColor(Vector3(1.0f, 0.98f, 0.85f));
				p->SetBlendMode(Particle::BLEND_ALPHA);
				RENDERER->GetParticleManager()->SetParticle(p);
			}


			break;
		}

	}



}
