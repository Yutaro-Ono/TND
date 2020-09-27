#include "FlameSpark.h"
#include "GameMain.h"
#include "Renderer.h"
#include "Texture.h"
#include "Player.h"
#include "Particle.h"
#include "ParticleManager.h"


FlameSpark::FlameSpark(Player * in_target)
	:m_state(FlameSpark::OccurreState::DISABLE)
	,m_target(in_target)
{
	// �e�N�X�`��
	m_texture = RENDERER->GetTexture("Data/Shaders/fire.png");

	// �X�P�[���̍œK��
	SetScale(4.3f);
}

FlameSpark::~FlameSpark()
{
}

void FlameSpark::UpdateActor(float in_deltaTime)
{
	// �^�[�Q�b�g�����v���C���[���A�N�Z����Ԃ������̓u���[�L��ԂɂȂ�����
	if (m_target->GetAccel() >= 40.0f || m_target->GetBrake() >= 10.0f)
	{
		m_state = ACTIVE;
	}
	else
	{
		m_state = DISABLE;
	}

	// �p�[�e�B�N�����������Ă���E�������Ă��Ȃ�
	switch (m_state)
	{

		// �������Ă��Ȃ�
	case DISABLE:

		break;

		// �������Ă���
	case ACTIVE:

		

		// �v���C���[�|�W�V�������擾
		m_position = m_target->GetPosition();
		m_position.z -= 80.0f;



		// �p�[�e�B�N���̃Z�b�g�@����͂��ƂŃp�[�e�B�N���G�~�b�^�N���X��肽���B
		Vector3 randV((rand() % 100) / 10.0f, (rand() % 100) / 10.0f, (rand() % 100) / 10.0f);
		Vector3 Velocity = randV * 0.1f;
		Velocity.x += -0.5f;
		Velocity.y += -0.0f;
		Velocity.z += 2.5f;

		// �p�[�e�B�N������
		for (int i = 0; i < 8; i++)
		{
			Vector3 pos;
			if (i % 2 == 0)
			{
				pos = m_position;
			}
			else
			{
				pos = m_position;
				pos.y *= -1.0f;
			}
			pos = pos + randV;

			// �p�[�e�B�N���𐶐�
			Particle* p = new Particle(pos,
				Velocity * 4.0f,
				m_scale, 1.0f,
				randV.x * 0.04f);

			p->SetTextureID(m_texture->GetTextureID());
			p->SetColor(Vector3(1.0f, 0.6f, 0.2f));
			p->SetBlendMode(Particle::BLEND_ADD);
			RENDERER->GetParticleManager()->SetParticle(p);
		}


		break;

	}
}

