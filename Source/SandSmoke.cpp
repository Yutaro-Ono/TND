#include "SandSmoke.h"
#include "GameMain.h"
#include "Renderer.h"
#include "Texture.h"
#include "Player.h"
#include "Particle.h"
#include "ParticleManager.h"

// �R���X�g���N�^
SandSmoke::SandSmoke(class Player* in_target)
	:m_state(DISABLE)
	,m_target(in_target)
{
	// �e�N�X�`��
	m_texture = RENDERER->GetTexture("Data/Shaders/fire.png");
	
	// �X�P�[�����œK��
	SetScale(100.0f);

}

// �f�X�g���N�^
SandSmoke::~SandSmoke()
{
}

// �X�V����
void SandSmoke::UpdateActor(float in_deltaTime)
{


	// �^�[�Q�b�g�����v���C���[�̃A�N�Z���l���u���[�L�l���20�ȏ�A�u���[�L�l���A�N�Z���l���10�ȏ�̎�
	if ((m_target->GetAccel() - m_target->GetBrake()) >= 20.0f || (m_target->GetBrake() - m_target->GetAccel()) > 5.0f)
	{
		// �p�[�e�B�N���������A�N�e�B�u��
		m_state = ACTIVE;
		
		// �A�N�Z���̒l�ɉ����ăX�P�[�����g��
		m_scale = 100.0f * (m_target->GetAccel() / 100.0f) * 1.2f;

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
		m_position.z -= 70.0f;


		// �����_���Ȓl��ݒ�
		Vector3 randV((rand() % 100) / 10.0f, (rand() % 100) / 10.0f, (rand() % 100) / 10.0f);
		Vector3 Velocity = randV * 0.1f;
		Velocity.x += -0.5f;
		Velocity.y += -0.0f;
		Velocity.z += 2.5f;

		// �p�[�e�B�N������
		// 3�t���[����1��@�p�[�e�B�N������
		if (GAME_INSTANCE.GetFrame() % 5 == 0)
		{
			for (int i = 0; i < 2; i++)
			{
				// �����ʒu��ݒ�
				Vector3 pos = m_position;
				// �����_���Ȓl�𑫂�
				pos = pos + randV;

				// �p�[�e�B�N���𐶐�
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
