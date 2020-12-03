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

	bool Initialize();                // ���������� (�e��t���[���o�b�t�@���쐬����)

	void DrawGBuffer();               // GBuffer�ւ̏������ݏ���
	void DrawLightPass();             // GBuffer���烉�C�e�B���O�v�Z���s��
	void Draw();                      // GBuffer�ɏ������܂ꂽ���̕`��

private:

	bool CreateGBuffer();             // GBuffer�̍쐬
	bool CreateLightBuffer();         // ���C�g�o�b�t�@�̍쐬



	//-----------------------------+
	// �����o�ϐ�
	//-----------------------------+
	class Renderer* m_renderer;           // �����_���[�N���X�ւ̃|�C���^

	class Shader* m_gBufferShader;        // GBuffer�p�V�F�[�_
	class Shader* m_gBufferSkinShader;    // GBuffer�p�X�L���V�F�[�_
	class Shader* m_screenShader;         // GBuffer�`��p�X�N���[���V�F�[�_

	class Shader* m_pointLightShader;
	class Shader* m_lightSphereShader;
	class Shader* m_directionalLightShader;
	class Shader* m_spotLightShader;

	// GBuffer
	unsigned int m_gBuffer;               // G-Buffer (3�v�f�����t���[���o�b�t�@)
    // G-Buffer�Ɋ֘A�t����e��o��
	unsigned int m_gPos;                  // 3D��ԍ��W
	unsigned int m_gNormal;               // �@���x�N�g��
	unsigned int m_gAlbedoSpec;           // �A���x�h(RGB)���X�y�L����(A)
	unsigned int m_gRBO;                  // G-Buffer�ɕR�t����`��o�b�t�@
	unsigned int m_attachments[3];

	// ���C�g�o�b�t�@ (���������p)
	unsigned int m_lightFBO;              // ���C�g�o�b�t�@
	// ���C�g�o�b�t�@�Ɋ֘A�t����o��
	unsigned int m_lightHDR;              // ���������p��HDR�Ή��o�b�t�@ (���������_)
	unsigned int m_lightRBO;              // ���C�g�p�����_�[�o�b�t�@
};