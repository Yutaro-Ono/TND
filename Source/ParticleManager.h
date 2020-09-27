//-----------------------------------------------------------------------+
// ParticleManager�N���X
// 
// 
// copyright (C) 2019 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#pragma once
#include "GameMain.h"
#include "Renderer.h"
#include "Shader.h"
#include "Particle.h"
#include <vector>
#include <map>


class ParticleManager
{
public:

	ParticleManager();                                                 // �R���X�g���N�^
	~ParticleManager();                                                // �f�X�g���N�^

	void Draw();                                                       // �`�揈��
	void SetParticle(class Particle* in_particle);                     // �p�[�e�B�N���̃Z�b�g
	void Update(float in_deltaTime);                                   // �X�V����

	void AllDeadParticle();                                            // �p�[�e�B�N���X�^�b�N�̍폜


private:

	void ChangeBlendMode(Particle::PARTICLE_BLEND in_blendType);
	void ChangeTexture(int in_changeTextureID);
	Vector3 CalcCameraPos();

	class Shader* m_shader;                                             // �V�F�[�_�[
	std::vector<class Particle*> m_particles;                           // �p�[�e�B�N���s��
	std::vector<class Particle*> m_pendingParticles;                    // �y���f�B���O�A�N�^�[�s��

	Matrix4 m_viewMat;                                                  // �r���[�s��
	Matrix4 m_projectionMat;                                            // �v���W�F�N�V�����s��
	Matrix4 m_billboardMat;                                             // �r���{�[�h�s��
};