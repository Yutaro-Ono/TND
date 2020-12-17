#include "SandSmoke.h"
#include "GameMain.h"
#include "Renderer.h"
#include "Texture.h"
#include "CarWheel.h"
#include "PlayerCar.h"
#include "Particle.h"
#include "ParticleManager.h"
#include "LevelTerrain.h"

// �R���X�g���N�^
SandSmoke::SandSmoke(class CarWheel* in_target)
	:m_state(DISABLE)
	,m_target(in_target)
	,m_color(Vector3(1.0f, 1.0f, 1.0f))
{
	// �e�N�X�`��
	m_texture = RENDERER->GetTexture("Data/Shaders/fire.png");
	
	// �X�P�[�����œK��
	SetScale(20.0f);

}

// �f�X�g���N�^
SandSmoke::~SandSmoke()
{
}

// �X�V����
void SandSmoke::UpdateActor(float in_deltaTime)
{


	// �Ԃ��A�N�Z����ԁE�u���[�L��Ԃ̂Ƃ��ɃA�N�e�B�u
	if (m_target->GetCarPtr()->GetDriveState() != PlayerCar::DRIVE_IDLE && m_target->GetSpin() == true)
	{
		// �p�[�e�B�N���������A�N�e�B�u��
		m_state = ACTIVE;
		
		// �n�`�ɉ����ĉ��̐F��ύX
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


	// �p�[�e�B�N�����������Ă���E�������Ă��Ȃ�
	switch (m_state)
	{

		// �������Ă��Ȃ�
	case DISABLE:

		break;

		// �������Ă���
	case ACTIVE:

		// �v���C���[�|�W�V�������擾
		m_position = m_target->GetWorldTransform().GetTranslation();
		m_position.z -= 10.0f;


		// �����_���Ȓl��ݒ�
		Vector3 randV((rand() % 100) / 10.0f, (rand() % 100) / 10.0f, (rand() % 100) / 10.0f);
		Vector3 Velocity = randV * 0.1f;
		Velocity.x += -0.0f;
		Velocity.y = 0.0f;
		Velocity.z += 0.5f;

		// �p�[�e�B�N������
		// 3�t���[����1��@�p�[�e�B�N������
		if (GAME_INSTANCE.GetFrame() % 5 == 0)
		{
			for (int i = 0; i < 15; i++)
			{
				// �����ʒu��ݒ�
				Vector3 pos = m_position;
				// �����_���Ȓl�𑫂�
				pos = pos + randV;

				// �p�[�e�B�N���𐶐�
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
