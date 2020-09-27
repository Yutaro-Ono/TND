#include "GetEffect.h"
#include "Particle.h"
#include "ParticleManager.h"
#include "Player.h"
#include "Renderer.h"
#include "Texture.h"


#include "Math.h"



GetEffect::GetEffect(class Player* in_target)
	:m_targetPlayer(in_target)
	,m_speed(25.0f)
	,m_timer(0.0f)
{
	// �G�t�F�N�g�����ʒu���v���C���[���W����擾
	m_position = m_targetPlayer->GetPosition();

	m_effectRank = RANK_1;

	Texture* flameTexture = RENDERER->GetTexture("Data/Shaders/fire.png");

	// �����N�ɉ����ăG�t�F�N�g�𐶐�
	for (int i = 0; i < 90; i++)
	{
		// m_effectVal[i] = 1.0f / i;

		m_angle = (i + 1) * Math::TwoPi / 90.0f;

		m_effects.push_back(new Particle(m_position, Vector3(Math::Cos(m_angle) * m_speed, Math::Sin(m_angle) * m_speed, 1.5f), 120.0f, 0.6f, 1.0f));
		m_effects[i]->SetTextureID(flameTexture->GetTextureID());
		m_effects[i]->SetColor(Vector3(1.0f, 0.3f, 0.1f));
		m_effects[i]->SetBlendMode(Particle::BLEND_ADD);
		RENDERER->GetParticleManager()->SetParticle(m_effects[i]);
	}

	printf("�G�t�F�N�g����\n");
}

GetEffect::~GetEffect()
{
}

void GetEffect::UpdateActor(float in_deltaTime)
{

	m_timer += 1.0f * in_deltaTime;

	// 3�b�ŏ���
	if (m_timer >= 3.0f)
	{
		printf("�G�t�F�N�g����");
		SetState(STATE_DEAD);
	}

}
