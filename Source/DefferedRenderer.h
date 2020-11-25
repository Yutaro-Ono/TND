//-------------------------------------------------------------------------------------------+
//
// �x�������_�����O�N���X (GBuffer�Ɋe�`�挋�ʂ�ۑ��EGBuffer�ɑ΂��Č����������s��)
//
//-------------------------------------------------------------------------------------------+
#pragma once

class DefferedRenderer
{

public:

	DefferedRenderer(class Renderer* in_renderer);
	~DefferedRenderer();

	bool Initialize();

	void Draw();                      // Renderer�N���X�ɓo�^����Ă���I�u�W�F�N�g�̕`��

private:

	bool CreateGBuffer();             // GBuffer�̍쐬
	bool CreateLightBuffer();         // ���C�g�o�b�t�@�̍쐬


	//-----------------------------+
	// �����o�ϐ�
	//-----------------------------+
	class Renderer* m_renderer;           // �����_���[�N���X�ւ̃|�C���^

	// GBuffer
	unsigned int m_gBuffer;               // G-Buffer (3�v�f�����t���[���o�b�t�@)
    // G-Buffer�Ɋ֘A�t����e��o��
	unsigned int m_gPos;                  // 3D��ԍ��W
	unsigned int m_gNormal;               // �@���x�N�g��
	unsigned int m_gAlbedoSpec;           // �A���x�h(RGB)���X�y�L����(A)
	unsigned int m_gRBO;                  // G-Buffer�ɕR�t����`��o�b�t�@

	// ���C�g�o�b�t�@ (���������p)
	unsigned int m_lightFBO;              // ���C�g�o�b�t�@
	// ���C�g�o�b�t�@�Ɋ֘A�t����o��
	unsigned int m_lightHDR;              // ���������p��HDR�Ή��o�b�t�@ (���������_)
	unsigned int m_lightRBO;              // ���C�g�p�����_�[�o�b�t�@
};