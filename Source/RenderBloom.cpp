#include "RenderBloom.h"
#include "GameMain.h"
#include "Renderer.h"
#include "GameConfig.h"
#include <iostream>

#define DOWN_SAMPLE_LEVEL 5

// �R���X�g���N�^
RenderBloom::RenderBloom()
{
	if (!CreateHDRFBO())
	{
		std::cout << "HDR�o�b�t�@�̐����Ɏ��s" << std::endl;
	}
}

// �f�X�g���N�^
RenderBloom::~RenderBloom()
{
}

// �o�b�t�@�̐���
bool RenderBloom::CreateHDRFBO()
{
	glGenFramebuffers(1, &m_hdrFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_hdrFBO);
	// �J���[�o�b�t�@�̐����ƃA�^�b�`
	CreateColorBuffer(m_defaultBuffer, 0);
	CreateColorBuffer(m_brightBuffer, 1);
	// �����_�[�o�b�t�@����
	glGenRenderbuffers(1, &m_rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, GAME_CONFIG->GetScreenWidth(), GAME_CONFIG->GetScreenHeight());
	// �t���[���o�b�t�@�ɃA�^�b�`
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo);

	// GL�Ƀ}���`�����_�����O���s�����Ƃ𖾎�
	m_attachments[0] = { GL_COLOR_ATTACHMENT0 };
	m_attachments[1] = { GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, m_attachments);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		return false;
	}

	// �o�C���h����
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return true;
}

// �J���[�o�b�t�@�̐���
void RenderBloom::CreateColorBuffer(unsigned int in_colorBuffer, const unsigned int in_attachNum)
{
	glGenTextures(1, &in_colorBuffer);
	glBindTexture(GL_TEXTURE_2D, in_colorBuffer);
	// �X�N���[���T�C�Y�̃e�N�X�`���𐶐� (���������_�^)
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, GAME_CONFIG->GetScreenWidth(), GAME_CONFIG->GetScreenHeight(), 0, GL_RGBA, GL_FLOAT, NULL);
	// �e�N�X�`���t�B���^�����O�E���b�s���O�ݒ�
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// �t���[���o�b�t�@�ɃA�^�b�`
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + in_attachNum, GL_TEXTURE_2D, in_colorBuffer, 0);

}
