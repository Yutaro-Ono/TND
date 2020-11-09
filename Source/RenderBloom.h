//----------------------------------------------------------------------------------------+
//
// HDR(HighDynamicRange)�Ή��̃t���[���o�b�t�@�Ƃ���𗘗p����Bloom�������s��
//
//----------------------------------------------------------------------------------------+
#pragma once
#include <vector>

class RenderBloom
{
public:

	RenderBloom();        // �R���X�g���N�^
	~RenderBloom();       // �f�X�g���N�^


private:

	// �o�b�t�@�̐�������
	bool CreateHDRFBO();
	// �J���[�o�b�t�@�̐���
	void CreateColorBuffer(unsigned int in_colorBuffer, const unsigned int in_attachNum);



	unsigned int m_hdrFBO;              // HDR(HighDynamicRange)�Ή��̃t���[���o�b�t�@ (�F�l�𕂓������_�^�ɂ��邱�ƂŁA�F��𐸍׉�����)
	unsigned int m_rbo;                 // �`��o�b�t�@

	// �}���`�����_�[�^�[�Q�b�g
	unsigned int m_defaultBuffer;       // �ʏ�̃J���[�o�b�t�@(HDR�Ή�)
	unsigned int m_brightBuffer;        // ���P�x���ۑ�����o�b�t�@
	unsigned int m_attachments[2];      // �}���`�����_�[�^�[�Q�b�g�����p

	// �_�E���T���v�����O�E�ڂ����p��FBO�ƃe�N�X�`���z��
	std::vector<unsigned int> m_blurFBO;
	std::vector<unsigned int> m_blurBufferTex;


};