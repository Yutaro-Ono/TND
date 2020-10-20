#include "FrameBuffer.h"
#include <SDL_image.h>
#include <SDL.h>
#include <GL/glew.h>
#include "GameMain.h"

FrameBuffer::FrameBuffer()
{
}

FrameBuffer::~FrameBuffer()
{
}

bool FrameBuffer::CreateFrameBuffer()
{
	// �X�N���[���𕢂����߂̒��_�z��쐬
	float quadVertices[] = {
		// �|�W�V����   // �e�N�X�`�����W
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};
	// ���_�I�u�W�F�N�g�̍쐬
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	// �A�g���r���[�g�w��
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	//----------------------------------------------------------------------+
    //
    // �|�X�g�G�t�F�N�g�p�t���[���o�b�t�@��`
    //
    //----------------------------------------------------------------------+
    // �t���[���o�b�t�@�I�u�W�F�N�g���`���č쐬(Gen)
	unsigned int fbo;
	glGenFramebuffers(1, &fbo);
	// active�ȃt���[���o�b�t�@�Ƃ��ăo�C���h����ƁA�`���ɂȂ�
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	// �`�施�߂��s������Ƀo�C���h����������ƁA�`��悪�X�N���[���ɖ߂�
	// glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// �t���[���o�b�t�@������Ɋ����������ǂ������`�F�b�N
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
	{
		return false;

	}

	// �t���[���o�b�t�@�I�u�W�F�N�g�̓A�v���I���O�ɍ폜���邱�Ƃ�Y�ꂸ��
	// glDeleteFramebuffers(1, &fbo);

	//-----------------------------------------------------------------------+
	//
	// �e�N�X�`���J���[�o�b�t�@�̍쐬
	//
	//-----------------------------------------------------------------------+
	unsigned int texColorBuffer;
	glGenTextures(1, &texColorBuffer);
	glBindTexture(GL_TEXTURE_2D, texColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, GAME_CONFIG->GetScreenWidth(), GAME_CONFIG->GetScreenHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glBindTexture(GL_TEXTURE_2D, 0);
	// ���݂̃t���[���o�b�t�@�I�u�W�F�N�g�ɃA�^�b�`����
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0);

	//-------------------------------------------------------------+
	//
	// �����_�[�o�b�t�@�I�u�W�F�N�g�̍쐬
	//
	//-------------------------------------------------------------+
	// �����_�[�o�b�t�@�I�u�W�F�N�g�̍쐬
	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	// �����_�[�o�b�t�@���삪�I�u�W�F�N�g�ɉe����^����悤�Ƀo�C���h
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	// �����_�[�o�b�t�@�͏������ݐ�p
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, GAME_CONFIG->GetScreenWidth(), GAME_CONFIG->GetScreenHeight());
	//glBindRenderbuffer(GL_RENDERBUFFER, 0);
	// FBO��RBO���A�^�b�`
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	//���ɁA�t���[���o�b�t�@�����S�ł��邩�ǂ������`�F�b�N�������̂ŁA���S�łȂ��ꍇ�̓G���[���b�Z�[�W��\�����܂��B
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	}

	// �t���[���o�b�t�@����
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::DrawFrameBuffer()
{
}
