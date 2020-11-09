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
	void WriteBuffer(std::vector<class MeshComponent*> in_meshComp);
	void WriteBuffer(std::vector<class SkeletalMeshComponent*> in_skelComp);

	void DrawDownSampling();      // ���P�x�o�b�t�@���_�E���T���v�����O�v�Z���ĕ`�悷��



private:

	// �o�b�t�@�̐�������
	bool CreateHDRFBO();
	// �J���[�o�b�t�@�̐���
	void CreateColorBuffer(unsigned int in_colorBuffer, const unsigned int in_attachNum);
	// �_�E���T���v�����O�E�ڂ����pFBO�̐���
	void CreateBlurFBO();

	// �K�E�X�v�Z
	float GaussianDistribution(const Vector2& in_pos, float in_rho);    // �K�E�X�ڂ����̏d�݂��Z�o���� (in_rho = �΍�)
	void CalcGaussBlurParam(int in_w, int in_h, Vector2 in_dir, float in_deviation, Vector3* in_offset);


	//----------------------------+
	// �����o�ϐ�
	//----------------------------+
	unsigned int m_hdrFBO;              // HDR(HighDynamicRange)�Ή��̃t���[���o�b�t�@ (�F�l�𕂓������_�^�ɂ��邱�ƂŁA�F��𐸍׉�����)
	unsigned int m_rbo;                 // �`��o�b�t�@

	// ���_�z��p�I�u�W�F�N�g
	unsigned int m_vao;
	unsigned int m_vbo;

	// �}���`�����_�[�^�[�Q�b�g
	unsigned int m_defaultBuffer;       // �ʏ�̃J���[�o�b�t�@(HDR�Ή�)
	unsigned int m_brightBuffer;        // ���P�x���ۑ�����o�b�t�@
	unsigned int m_attachments[2];      // �}���`�����_�[�^�[�Q�b�g�����p

	// �_�E���T���v�����O�E�ڂ����p��FBO�ƃe�N�X�`���z��
	std::vector<unsigned int> m_blurFBO;
	std::vector<unsigned int> m_blurBufferTex;

	class Shader* m_hdrBloomShader;
	class Shader* m_downSamplingShader;
};