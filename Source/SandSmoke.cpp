#include "SandSmoke.h"
#include "GameMain.h"
#include "Renderer.h"
#include "Texture.h"
#include "CarWheel.h"
#include "PlayerCar.h"
#include "Particle.h"
#include "ParticleManager.h"
#include "LevelTerrain.h"

// コンストラクタ
SandSmoke::SandSmoke(class CarWheel* in_target)
	:m_state(DISABLE)
	,m_target(in_target)
	,m_color(Vector3(1.0f, 1.0f, 1.0f))
{
	// テクスチャ
	m_texture = RENDERER->GetTexture("Data/Shaders/fire.png");
	
	// スケールを最適化
	SetScale(20.0f);

}

// デストラクタ
SandSmoke::~SandSmoke()
{
}

// 更新処理
void SandSmoke::UpdateActor(float in_deltaTime)
{


	// 車がアクセル状態・ブレーキ状態のときにアクティブ
	if (m_target->GetCarPtr()->GetDriveState() != PlayerCar::DRIVE_IDLE && m_target->GetSpin() == true)
	{
		// パーティクル発生をアクティブに
		m_state = ACTIVE;
		
		// 地形に応じて煙の色を変更
		if (m_target->GetCarPtr()->GetOnTerrainType() == LevelTerrain::TYPE_STREET)
		{
			m_color = Vector3(0.2f, 0.2f, 0.2f);
		}
		if (m_target->GetCarPtr()->GetOnTerrainType() == LevelTerrain::TYPE_GLASS)
		{
			m_color = Vector3(0.2f, 0.15f, 0.12f);
		}

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
		m_position = m_target->GetWorldTransform().GetTranslation();
		m_position.z -= 10.0f;


		// ランダムな値を設定
		Vector3 randV((rand() % 100) / 10.0f, (rand() % 100) / 10.0f, (rand() % 100) / 10.0f);
		Vector3 Velocity = randV * 0.1f;
		Velocity.x += -0.0f;
		Velocity.y = 0.0f;
		Velocity.z += 0.5f;

		// パーティクル発生
		// 3フレームに1回　パーティクル発生
		if (GAME_INSTANCE.GetFrame() % 5 == 0)
		{
			for (int i = 0; i < 15; i++)
			{
				// 発生位置を設定
				Vector3 pos = m_position;
				// ランダムな値を足す
				pos = pos + randV;

				// パーティクルを生成
				Particle* p = new Particle(pos,
					Velocity,
					m_scale, 0.1f,
					randV.x * 0.08f);

				p->SetTextureID(m_texture->GetTextureID());
				p->SetColor(m_color);
				p->SetBlendMode(Particle::BLEND_ALPHA);
				RENDERER->GetParticleManager()->SetParticle(p);
			}


			break;
		}

	}

}
