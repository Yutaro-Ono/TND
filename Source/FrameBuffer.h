#pragma once
#include <vector>

class FrameBuffer
{

public:


	enum POST_EFFECT_TYPE
	{
		NONE = 0,
		INVERT_COLOR,
		GREY_SCALE,
		CERNEL,
		BLUR,
		SHARP_EDGE,
		ALL_NUM
	};


	FrameBuffer();
	~FrameBuffer();

	bool CreateFrameBuffer();     // �t���[���o�b�t�@�I�u�W�F�N�g�̍쐬

	void WriteFrameBuffer();
	void DrawFrameBuffer();

	void DebugFrameBuffer();

private:

	unsigned int m_FBO;             // �t���[���o�b�t�@�I�u�W�F�N�g

	unsigned int m_RBO;             // �����_�[�o�b�t�@�I�u�W�F�N�g

	unsigned int m_colorBuffer;     // �J���[�o�b�t�@


	int m_shaderNum;
	std::vector<class Shader*> m_postEffectShaders;      // �|�X�g�G�t�F�N�g�p�V�F�[�_
	class Shader* m_activeShader;                        // ���݃A�N�e�B�u�ȃV�F�[�_
};