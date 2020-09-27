//-----------------------------------------------------------------------+
// �Ƃ�����(�t�B�[���h�I�u�W�F�N�g) �N���X
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

	// �p�[�e�B�N���𐶐�
	m_particle = new Particle(m_position, m_velocity, 100.0f, 0.6f, 10.0f);
	m_particle->SetTextureID(m_texture->GetTextureID());
	m_particle->SetColor(Vector3(1.0f, 0.5f, 0.2f));
	m_particle->SetBlendMode(Particle::BLEND_ADD);

	SetScale(100.0f);

	// ���ʉ�
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


	// �p�[�e�B�N���̃Z�b�g�@����͂��ƂŃp�[�e�B�N���G�~�b�^�N���X��肽���B

	Vector3 randV((rand() % 100) / 10.0f, (rand() % 100) / 10.0f, (rand() % 100) / 10.0f);
	Vector3 Velocity = randV * 0.1f;
	Velocity.x += -0.5f;
	Velocity.y += -0.5f;
	Velocity.z += 2.5f;

	// ��Ƀp�[�e�B�N�������p�N���X�쐬����
	// 3�t���[����1��@�p�[�e�B�N������
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
	// �v���C���[�ƏՓ˂�����
	if (m_state != State::STATE_DEAD && m_hitSphere != nullptr && m_hitSphere->Contains(in_player->GetPosition()))
	{
		// ���ʉ�
		AUDIO->PlaySound("Data/Music/SE/FC/Actors/Player/GetFlame/small_explosion2.wav");

		// �v���C���[�̃`�F�C�������X�V
		in_player->AddFlameChain();
		// �v���C���[�̃A�N�Z�����E�l���X�V
		in_player->SetAccelLimit(2.5f);
		// �G�t�F�N�g�𐶐�
		GetEffect* effect = new GetEffect(in_player);
		// ���S��Ԃ�
		SetState(State::STATE_DEAD);
	}
}

void FlameActor::ComputeWorldTransform2(Matrix4 in_mat)
{
	// �X�P�[�����O����]�����s�ړ��ƂȂ�悤�ɕϊ��s����쐬
	// m_worldTransform = Matrix4::CreateScale(m_scale);


	// Y����]
	// m_worldTransform *= Matrix4::CreateRotationY(in_radY);
	// Z����]
	// m_worldTransform *= Matrix4::CreateRotationX(in_radZ);


	// �|�W�V����
	// m_worldTransform *= Matrix4::CreateTranslation(m_position);

	m_worldTransform = in_mat;

	// �A�N�^�[�������Ă���S�R���|�[�l���g�̕ϊ����s��
	for (auto comp : m_components)
	{
		comp->OnUpdateWorldTransform();
	}
}
