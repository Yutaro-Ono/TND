#pragma once

class FrameBuffer
{

public:

	FrameBuffer();
	~FrameBuffer();

	bool CreateFrameBuffer();     // �t���[���o�b�t�@�I�u�W�F�N�g�̍쐬

	void WriteFrameBuffer();
	void DrawFrameBuffer();


private:

	unsigned int m_FBO;             // �t���[���o�b�t�@�I�u�W�F�N�g

	unsigned int m_VAO, m_VBO;      // ���_�z��I�u�W�F�N�g�A���_�o�b�t�@�I�u�W�F�N�g

	unsigned int m_RBO;             // �����_�[�o�b�t�@�I�u�W�F�N�g

	unsigned int m_colorBuffer;     // �J���[�o�b�t�@

};