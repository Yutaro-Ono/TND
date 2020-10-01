#include "ParticleTestScene.h"
#include "Particle.h"
#include "ParticleManager.h"
#include "Shader.h"
#include "Grid.h"
#include "Texture.h"
#include "LevelBlock.h"
#include "GameMain.h"
#include <stdlib.h>

ParticleTestScene::ParticleTestScene()
{
	Matrix4 view, proj;

	// �e�N�X�`���̓ǂݍ���
	 mTexture = RENDERER->GetTexture("assets/fire.png");

	//�v���W�F�N�V�����s��Z�b�g
	mProjection = Matrix4::CreatePerspectiveFOV(Math::ToRadians(70.0f),
		static_cast<float>(GAME_INSTANCE.GetRenderer()->GetScreenWidth()),
		static_cast<float>(GAME_INSTANCE.GetRenderer()->GetScreenHeight()),
		1.0f, 10000.0f);

	RENDERER->SetProjectionMatrix(mProjection);

	// �f�o�b�O�p�O���b�h�Z�b�g
	Vector3 col(0, 1, 0);
	mGrid = new Grid(1000.0f, 10, col);

	// �w�i�u���b�N�Z�b�g
	//BGBlock *block;
	//const float blockSize = 200.0f;

	//for (int i = 0; i < 5; i++)
	//{
	//	block = new BGBlock;
	//	block->SetMesh(GAMEINSTANCE.GetRenderer()->GetMesh("assets/ForestGround01.gpmesh"));
	//	block->SetPosition(Vector3(i * blockSize - 400, 0, -100));
	//}

	// �f�B���N�V���i�����C�g�̐ݒ�
	GAME_INSTANCE.GetRenderer()->SetAmbientLight(Vector3(0.2f, 0.2f, 0.2f));
	DirectionalLight dir;
	dir.m_position = Vector3(0.0f, 0.0f, 1200.0f);
	dir.m_direction = Vector3(0.2f, -0.4f, -0.7f);
	dir.m_direction.Normalize();
	dir.m_diffuseColor = Vector3(0.78f, 0.88f, 1.0f);
	dir.m_specColor = Vector3(0.8f, 0.8f, 0.8f);
	GAME_INSTANCE.GetRenderer()->SetDirectionalLight(dir);

}

ParticleTestScene::~ParticleTestScene()
{
	delete mGrid;
}

SceneBase* ParticleTestScene::Update()
{
	const float len = 1000.0f;
	static float anim = 0.0f;
	anim += 0.01f;

	Vector3 vPos(len * cosf(anim), len * sinf(anim), 1000);
	Vector3 look(0, 0, 0);
	mView = Matrix4::CreateLookAt(vPos, look, Vector3::UnitZ);
	RENDERER->SetViewMatrix(mView);

	// �p�[�e�B�N���̃Z�b�g�@����͂��ƂŃp�[�e�B�N���G�~�b�^�N���X��肽���B

	Vector3 randV((rand() % 100) / 10.0f, (rand() % 100) / 10.0f, (rand() % 100) / 10.0f);
	Vector3 Velocity = randV * 0.1f;
	Velocity.x += -0.5f;
	Velocity.y += -0.5f;
	Velocity.z += 2.5f;

	// ��Ƀp�[�e�B�N�������p�N���X�쐬����
	// 3�t���[����1��@�p�[�e�B�N������
	static int frame = 0;
	frame++;
	if (frame % 5 == 0)
	{
		for (int i = 0; i < 2; i++)
		{
			Vector3 pos;
			if (i % 2 == 0)
			{
				pos = Vector3(0, -100, 0);
			}
			else
			{
				pos = Vector3(0, 100, 0);
			}
			pos = pos + randV;

			Particle* p = new Particle(pos,
				Velocity,
				200.0f, 1.0f,
				randV.x * 0.5f);

			p->SetTextureID(mTexture->GetTextureID());
			p->SetColor(Vector3(1.0f, 0.5f, 0.2f));
			p->SetBlendMode(Particle::BLEND_ADD);
			RENDERER->GetParticleManager()->SetParticle(p);
		}
	}

	return this;
}

void ParticleTestScene::Draw()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	//��ʃN���A
	RENDERER->WindowClear();
	mGrid->Draw();
	RENDERER->Draw();

	RENDERER->WindowFlip();
}
