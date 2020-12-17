//-----------------------------------------------------------------------+
// ParticleManager�N���X
// 
// 
// copyright (C) 2019 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#include "ParticleManager.h"
#include "Particle.h"
#include "Shader.h"
#include "Renderer.h"
#include <algorithm>

// �R���X�g���N�^
ParticleManager::ParticleManager()
{
	m_shader = new Shader;

	if (!m_shader->Load("Data/Shaders/Phong.vert", "Data/Shaders/Particle.frag"))
	{
		printf("ParticleManager / Shader Load : Failed\n");
	}
}

// �f�X�g���N�^
ParticleManager::~ParticleManager()
{
	// �S�p�[�e�B�N���̍폜
	while (!m_particles.empty())
	{
		delete m_particles.back();
		m_particles.pop_back();
	}

	delete m_shader;
}

// �`�揈��
void ParticleManager::Draw()
{

	if (m_particles.size() == 0)
	{
		return;
	}
	// �u�����h���[�h������Ԏ擾
	Particle::PARTICLE_BLEND blendType, prevType;
	auto itr = m_particles.begin();
	blendType = prevType = (*itr)->GetBlendType();

	// �e�N�X�`��ID������Ԏ擾
	int nowTexture, prevTexture;
	nowTexture = prevTexture = (*itr)->GetTextureID();

	// �r���[�v���W�F�N�V�����s��
	Matrix4 viewProjectionMat;
	viewProjectionMat = m_viewMat * m_projectionMat;

	// �V�F�[�_�[ON
	m_shader->SetActive();
	m_shader->SetMatrixUniform("u_viewProj", viewProjectionMat);
	m_shader->SetInt("uTexture", 0);

	// �S�Ẵp�[�e�B�N���̃r���{�[�h�s����Z�b�g
	(*itr)->SetBillboardMat(GetBillboardMatrix());

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glDepthMask(GL_FALSE);

	// ���ׂẴp�[�e�B�N����`��
	ChangeBlendMode(blendType);
	ChangeTexture(nowTexture);


	// mParticle���̕`�惋�[�v
	for (auto particles : m_particles)
	{
		
		//�u�����h���[�h�ύX���K�v�Ȃ�ύX����
		blendType = particles->GetBlendType();

		// �u�����h�^�C�v���X�V���ꂽ��ύX
		if (blendType != prevType)
		{
			ChangeBlendMode(blendType);
		}


		// �e�N�X�`���ύX���K�v�Ȃ�ύX����
		nowTexture = particles->GetTextureID();
		if (nowTexture != prevTexture)
		{
			ChangeTexture(nowTexture);
		}

		// �e�N�X�`���̃A�N�e�B�u��
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, nowTexture);

		// �p�[�e�B�N���`��
		particles->Draw(m_shader);


		// �O��`���ԂƂ��ĕۑ�
		prevType = blendType;
		prevTexture = nowTexture;

	}


	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);
}

// �p�[�e�B�N�����x�N�g���z��ɃZ�b�g
void ParticleManager::SetParticle(Particle * in_particle)
{
	// �y���f�B���O�z��ɒǉ�
	m_pendingParticles.emplace_back(in_particle);
}

// �X�V����
void ParticleManager::Update(float in_deltaTime)
{
	// �S�Ẵp�[�e�B�N���̍X�V
	{
 		for (auto particles : m_particles)
		{
			particles->Update(in_deltaTime);
		}
	}

	// �y���f�B���O���̃p�[�e�B�N�����A�N�e�B�u�p�[�e�B�N���ɒǉ�
	{
		for (auto pending : m_pendingParticles)
		{
			m_particles.emplace_back(pending);
		}
	}

	// �y���f�B���O���N���A
	if (m_pendingParticles.size() != 0)
	{
		m_pendingParticles.clear();
	}


	// �A�N�e�B�u�ȃp�[�e�B�N�������݂��Ȃ��ꍇ
	if (m_particles.size() == 0)
	{
		return;
	}

	// �r���[�s��E�v���W�F�N�V�����s��E�r���{�[�h�s��
	m_viewMat = RENDERER->GetViewMatrix();
	m_projectionMat = RENDERER->GetProjectionMatrix();
	m_billboardMat = GetBillboardMatrix();

	// ���݂̃J�����ʒu���p�[�e�B�N���ɋ�����
	auto iter = m_particles.begin();

	// ����ł���p�[�e�B�N�����폜
	while (iter != m_particles.end())
	{
		// �p�[�e�B�N��������ł���Ȃ�폜
		if (!((*iter)->IsAlive()))
		{
			// erase(iter)�͗v�f�̍폜�ɉ����A���̗v�f���w��
			iter = m_particles.erase(iter);
		}
		else
		{
				iter++;
		}
	}


	// �A�N�e�B�u�ȃp�[�e�B�N�������݂��Ȃ��ꍇ��return
	if (m_particles.size() == 0)
	{
		return;
	}


	iter = m_particles.begin();
	// �r���{�[�h�ʒu
	(*iter)->m_staticBillboardMat = m_billboardMat;

	// �J�����ʒu
	(*iter)->m_staticCameraWorldPos = CalcCameraPos();


	// �p�[�e�B�N�����J���������ō~���\�[�g (�J�����ɂ��߂��p�[�e�B�N����O�ʂɏo��)
	std::sort(m_particles.begin(), m_particles.end(), std::greater<Particle *>());

}

void ParticleManager::AllDeadParticle()
{
	auto iter = m_particles.begin();

	while (iter != m_particles.end())
	{
		// erase(iter)�͗v�f�̍폜�ɉ����A���̗v�f���w��
		iter = m_particles.erase(iter);
	}
}

// �u�����h���[�h�ؑ�
void ParticleManager::ChangeBlendMode(Particle::PARTICLE_BLEND in_blendType)
{
	switch (in_blendType)
	{
	case Particle::BLEND_ADD:
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);  //���Z����
		break;
	case Particle::BLEND_ALPHA:
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // �A���t�@�u�����h
		break;
	case Particle::BLEND_MULT:
		glBlendFunc(GL_ZERO, GL_SRC_COLOR); //��Z����
		break;
	default:
		break;
	}
}


void ParticleManager::ChangeTexture(int in_changeTextureID)
{
	glBindTexture(GL_TEXTURE_2D, in_changeTextureID);
}


Vector3 ParticleManager::CalcCameraPos()
{
	// �r���[�s���胏�[���h�ł̃J�����ʒu�Z�o
	Matrix4 view = RENDERER->GetViewMatrix();
	Matrix4 transMat = view;

	// �r���[�s��̈ړ������������
	Vector3 v;
	v.x = -1.0f * view.mat[3][0];
	v.y = -1.0f * view.mat[3][1];
	v.z = -1.0f * view.mat[3][2];

	//�ړ���������菜�������Ɠ]�u���āA��]�����̋t�ϊ������
	transMat.mat[3][0] = transMat.mat[3][1] = transMat.mat[3][2] = 0.0f;
	transMat.Transpose();

	return Vector3(Vector3::Transform(v, transMat));
}
