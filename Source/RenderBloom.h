//----------------------------------------------------------------------------------------+
//
// HDR(HighDynamicRange)�Ή��̃t���[���o�b�t�@�Ƃ���𗘗p����Bloom�������s��
//
//----------------------------------------------------------------------------------------+
#pragma once
#include <vector>
#include "Math.h"

class RenderBloom
{
public:

	RenderBloom();        // �R���X�g���N�^
	~RenderBloom();       // �f�X�g���N�^

	// �J���[�o�b�t�@�E���P�x�o�b�t�@�ւ̏�������(��p�̃V�F�[�_�Ń��b�V���̑S�`����s��)
	void WriteBuffer(std::vector<class MeshComponent*> in_meshComp, std::vector<class SkeletalMeshComponent*> in_skelComp,
		class CubeMapComponent* in_cubeMapComp, std::vector<class EnvironmentMapComponent*> in_envComp);
	void WriteBuffer(class ParticleManager* in_particle);

	void DrawDownSampling(unsigned int in_brightBuffer);      // ���P�x�o�b�t�@���_�E���T���v�����O�v�Z���ĕ`�悷��
	void DrawGaussBlur();                                     // �K�E�X�ڂ������ʂ�K�p����

	void DrawBlendBloom(unsigned int in_colorBuffer);         // Bloom��K�p�����ŏI���ʂ�`�悷��


	// �K�E�X�v�Z�֐�
	float GaussianDistribution(const Vector2& in_pos, float in_rho);    // �K�E�X�ڂ����̏d�݂��Z�o���� (in_rho = �΍�)
	// �K�E�X�ڂ����v�Z
	void CalcGaussBlurParam(int in_w, int in_h, Vector2 in_dir, float in_deviation, Vector3* in_offset);

	// �o�b�t�@�̎擾
	unsigned int GetColorBuffer() { return m_defaultBuffer; }
	unsigned int GetBrightBuffer() { return m_brightBuffer; }

	// �t�F�[�h�C���E�t�F�[�h�A�E�g
	bool FadeOut(float in_val, float in_deltaTime);
	bool FadeIn(float in_val, float in_deltaTime);
	// �z���C�g�C���E�z���C�g�A�E�g
	bool WhiteOut(float in_val, float in_deltaTime);
	bool WhiteIn(float in_val, float in_deltaTime);


	void SetExposureVal(float in_exp) { m_exposure = in_exp; m_saveExposure = m_exposure; }     // �I�o
	void SetGamma(float in_gamma) { m_gamma = in_gamma; }          // �K���}�R���N�V����

private:

	// �o�b�t�@�̐�������
	bool CreateHDRFBO();
	// �J���[�o�b�t�@�̐���
	void CreateColorBuffer(unsigned int& in_colorBuffer, const unsigned int in_attachNum);
	// �_�E���T���v�����O�E�ڂ����pFBO�̐���
	void CreateBlurFBO();


	//----------------------------+
	// �����o�ϐ�
	//----------------------------+
	unsigned int m_hdrFBO;                             // HDR(HighDynamicRange)�Ή��̃t���[���o�b�t�@ (�F�l�𕂓������_�^�ɂ��邱�ƂŁA�F��𐸍׉�����)
	unsigned int m_rbo;                                // �`��o�b�t�@

	// �}���`�����_�[�^�[�Q�b�g
	unsigned int m_defaultBuffer;                      // �ʏ�̃J���[�o�b�t�@(HDR�Ή�)
	unsigned int m_brightBuffer;                       // ���P�x���ۑ�����o�b�t�@
	unsigned int m_attachments[2];                     // �}���`�����_�[�^�[�Q�b�g�����p

	// �_�E���T���v�����O�E�ڂ����p��FBO�ƃe�N�X�`���z��
	std::vector<unsigned int> m_blurFBO;
	std::vector<unsigned int> m_blurBufferTex;

	class Shader* m_multiRenderTargetShader;           // HDR�Ή��}���`�^�[�Q�b�g�V�F�[�_ (Mesh)
	class Shader* m_multiRenderSkinShader;             // HDR�Ή��}���`�^�[�Q�b�g�V�F�[�_ (SkinMesh)
	class Shader* m_multiRenderCubeMapShader;          // HDR�Ή��}���`�^�[�Q�b�g�V�F�[�_ (CubeMap)
	class Shader* m_multiRenderEnvironmentShader;      // HDR�Ή��}���`�^�[�Q�b�g�V�F�[�_ (EnvironmentMap)
	class Shader* m_hdrBloomShader;                    // Bloom�V�F�[�_
	class Shader* m_downSamplingShader;                // �k���o�b�t�@�p�V�F�[�_
	class Shader* m_gaussShader;                       // �K�E�X�ڂ����v�Z�p�V�F�[�_

	float m_exposure;                                  // �I�o��
	float m_saveExposure;                              // �t�F�[�h�A�E�g�������̘I�o�ۑ��p

	float m_gamma;                                     // �K���}�R���N�V����
};