#include "FrameBuffer.h"
#include <SDL_image.h>
#include <SDL.h>
#include <glad/glad.h>
#include "GameMain.h"
#include "Shader.h"
#include "VertexArray.h"

// �R���X�g���N�^
FrameBuffer::FrameBuffer()
{
	// �S�|�X�g�G�t�F�N�g���̃V�F�[�_�[�𐶐��E���[�h
	for (int i = 0; i < POST_EFFECT_TYPE::ALL_NUM; i++)
	{
		m_postEffectShaders.push_back(new Shader());
	}

	// �|�X�g�G�t�F�N�g�V�F�[�_�̃��[�h
	m_postEffectShaders[NONE]->Load("Data/Shaders/FB/FrameBufferScreen.vert", "Data/Shaders/FB/FrameBufferScreen.frag");
	m_postEffectShaders[INVERT_COLOR]->Load("Data/Shaders/FB/FrameBufferScreen.vert", "Data/Shaders/FB/InvertColorScreen.frag");
	m_postEffectShaders[GREY_SCALE]->Load("Data/Shaders/FB/FrameBufferScreen.vert", "Data/Shaders/FB/GreyScaleScreen.frag");
	m_postEffectShaders[CERNEL]->Load("Data/Shaders/FB/FrameBufferScreen.vert", "Data/Shaders/FB/CernelScreen.frag");
	m_postEffectShaders[BLUR]->Load("Data/Shaders/FB/FrameBufferScreen.vert", "Data/Shaders/FB/BlurScreen.frag");
	m_postEffectShaders[SHARP_EDGE]->Load("Data/Shaders/FB/FrameBufferScreen.vert", "Data/Shaders/FB/SharpEdgeScreen.frag");

	m_activeShader = m_postEffectShaders[NONE];

	m_shaderNum = 0;

}

// �f�X�g���N�^
FrameBuffer::~FrameBuffer()
{
	glDeleteFramebuffers(1, &m_FBO);
	glDeleteRenderbuffers(1, &m_RBO);

	for (auto shader : m_postEffectShaders)
	{
		shader->Delete();
	}
	m_postEffectShaders.clear();

}

bool FrameBuffer::CreateFrameBuffer()
{

	//----------------------------------------------------------------------+
    //
    // �|�X�g�G�t�F�N�g�p�t���[���o�b�t�@��`
    //
    //----------------------------------------------------------------------+
    // �t���[���o�b�t�@�I�u�W�F�N�g���`���č쐬(Gen)
	glGenFramebuffers(1, &m_FBO);
	// active�ȃt���[���o�b�t�@�Ƃ��ăo�C���h����ƁA�`���ɂȂ�
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

	// �t���[���o�b�t�@������Ɋ����������ǂ������`�F�b�N
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
	{
		return false;

	}

	//-----------------------------------------------------------------------+
	//
	// �e�N�X�`���J���[�o�b�t�@�̍쐬
	//
	//-----------------------------------------------------------------------+
	glGenTextures(1, &m_colorBuffer);
	glBindTexture(GL_TEXTURE_2D, m_colorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, GAME_CONFIG->GetScreenWidth(), GAME_CONFIG->GetScreenHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// ���݂̃t���[���o�b�t�@�I�u�W�F�N�g�ɃA�^�b�`����
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorBuffer, 0);

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

// �t���[���o�b�t�@�ւ̏������ݏ��� (���̒���Ƀ��b�V���Ȃǂ̒ʏ�`�揈�����s��)
void FrameBuffer::WriteFrameBuffer()
{
	if (m_shaderNum != 0)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
	}
}

// �������܂ꂽ�t���[����ҏW���`�悷��
void FrameBuffer::DrawFrameBuffer()
{
	if (m_shaderNum != 0)
	{
		// �u�����h�̗L����
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		// 2.�t���[���o�b�t�@���e���X�N���[���ɕ`��
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		// �[�x�e�X�g�I�t
		glDisable(GL_DEPTH_TEST);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		// �V�F�[�_�̗L����
		m_activeShader->SetActive();
		m_activeShader->SetInt("screenTexture", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_colorBuffer);
		// VAO�o�C���h
		RENDERER->GetScreenVAO()->SetActive();
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glEnable(GL_DEPTH_TEST);
	}

	// Imgui�f�o�b�O
	DebugFrameBuffer();
}

// �|�X�g�G�t�F�N�g�ύX (�f�o�b�O�p)
void FrameBuffer::DebugFrameBuffer()
{
#ifdef  _DEBUG

	// ImGui�X�V
	//ImGui::Begin("Debug Console : PostProcess");
	ImGui::SliderInt("PostProcessShader", &m_shaderNum, 0, ALL_NUM - 1);
	// �w�肵���|�X�g�G�t�F�N�g���A�N�e�B�u�V�F�[�_�Ƃ��čX�V
	m_activeShader = m_postEffectShaders[m_shaderNum];

#endif //  _DEBUG

}
