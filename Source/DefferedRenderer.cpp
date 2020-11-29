//-------------------------------------------------------------------------------------------+
//
// �x�������_�����O�N���X (GBuffer�Ɋe�`�挋�ʂ�ۑ��EGBuffer�ɑ΂��Č����������s��)
//
//-------------------------------------------------------------------------------------------+
#include "DefferedRenderer.h"
#include "Shader.h"
#include "Renderer.h"
#include "GameMain.h"
#include "GameConfig.h"
#include "MeshComponent.h"
#include "SkeletalMeshComponent.h"
#include "VertexArray.h"
#include "PointLightComponent.h"
#include "SpriteComponent.h"
#include "WorldSpaceUI.h"
#include "UIScreen.h"
DefferedRenderer::DefferedRenderer(Renderer* in_renderer)
	:m_renderer(in_renderer)
	,m_gBufferShader(nullptr)
	,m_gBufferSkinShader(nullptr)
	,m_screenShader(nullptr)
	,m_pointLightShader(nullptr)
	,m_directionalLightShader(nullptr)
	,m_spotLightShader(nullptr)
{
	std::cout << "CREATE::DefferedRenderer::Instance" << std::endl;
}

DefferedRenderer::~DefferedRenderer()
{
	delete m_gBufferShader;
	delete m_gBufferSkinShader;
	delete m_screenShader;
	delete m_pointLightShader;
	delete m_directionalLightShader;
	delete m_spotLightShader;

}

// �������֐�
bool DefferedRenderer::Initialize()
{
	// G�o�b�t�@�ƃ��C�g�o�b�t�@�̍쐬
	CreateGBuffer();
	CreateLightBuffer();

	// GBuffer�p�V�F�[�_�̍쐬
	m_gBufferShader = new Shader();
	if (!m_gBufferShader->Load("Data/Shaders/GBuffer/gBuffer_Basic.vert", "Data/Shaders/GBuffer/gBuffer_Basic.frag"))
	{
		return false;
	}
	// GBuffer�p�X�L���V�F�[�_�̍쐬
	m_gBufferSkinShader = new Shader();
	if (!m_gBufferSkinShader->Load("Data/Shaders/Skinned.vert", "Data/Shaders/GBuffer/gBuffer_Basic.frag"))
	{
		return false;
	}
	// �X�N���[���V�F�[�_
	m_screenShader = new Shader();
	if (!m_screenShader->Load("Data/Shaders/FB/FrameBufferScreen.vert", "Data/Shaders/FB/FrameBufferScreen.frag"))
	{
		return false;
	}
	// �|�C���g���C�g�V�F�[�_
	m_pointLightShader = new Shader();
	if (!m_pointLightShader->Load("Data/Shaders/GBuffer/PointLight.vert", "Data/Shaders/GBuffer/PointLight.frag"))
	{
		return false;
	}
	// �f�B���N�V���i�����C�g�V�F�[�_
	m_directionalLightShader = new Shader();
	if (!m_directionalLightShader->Load("Data/Shaders/FB/FrameBufferScreen.vert", "Data/Shaders/FB/FrameBufferScreen.frag"))
	{
		return false;
	}
	// �X�|�b�g���C�g�V�F�[�_
	m_spotLightShader = new Shader();
	if (!m_spotLightShader->Load("Data/Shaders/FB/FrameBufferScreen.vert", "Data/Shaders/FB/FrameBufferScreen.frag"))
	{
		return false;
	}

	return true;
}

// GBuffer�ւ̏������ݏ���
void DefferedRenderer::DrawGBuffer()
{
	// �`����GBuffer�Ƃ��ăo�C���h����
	glBindFramebuffer(GL_FRAMEBUFFER, m_gBuffer);
	// �r���[�|�[�g���X�N���[���T�C�Y�ɃZ�b�g
	glViewport(0, 0, GAME_CONFIG->GetScreenWidth(), GAME_CONFIG->GetScreenHeight());

	glClearColor(0.0, 0.0, 0.0, 0.0);     // �J���[�̃N���A
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// �[�x�e�X�g��ON
	glEnable(GL_DEPTH_TEST);

	//-----------------------------------------------------------+
	// �ʏ탁�b�V��
	//-----------------------------------------------------------+
	// �V�F�[�_��uniform�Z�b�g
	m_gBufferShader->SetActive();
	m_gBufferShader->SetMatrixUniform("u_view", m_renderer->m_view);
	m_gBufferShader->SetMatrixUniform("u_projection", m_renderer->m_projection);
	m_gBufferShader->SetInt("u_mat.diffuseMap", 0);
	m_gBufferShader->SetInt("u_mat.specularMap", 1);
	// ���b�V���`�� (������GBuffer�̊e�v�f�ɏ�񂪏������܂��)
	for (auto mesh : m_renderer->m_meshComponents)
	{
		mesh->Draw(m_gBufferShader);
	}

	//------------------------------------------------------------+
	// �X�L�����b�V��
	//------------------------------------------------------------+
	// �V�F�[�_��uniform�Z�b�g
	m_gBufferSkinShader->SetActive();
	m_gBufferSkinShader->SetMatrixUniform("u_view", m_renderer->m_view);
	m_gBufferSkinShader->SetMatrixUniform("u_projection", m_renderer->m_projection);
	m_gBufferSkinShader->SetInt("u_mat.diffuseMap", 0);
	m_gBufferSkinShader->SetInt("u_mat.specularMap", 1);
	// ���b�V���`�� (������GBuffer�̊e�v�f�ɏ�񂪏������܂��)
	for (auto skel : m_renderer->m_skeletalMeshComponents)
	{
		skel->Draw(m_gBufferSkinShader);
	}


	// GBuffer�̃o�C���h����
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

// GBuffer�����Ƀ��C�e�B���O�v�Z���s��
void DefferedRenderer::DrawLightPass()
{
	// ���C�g�o�b�t�@���o�C���h
	glBindFramebuffer(GL_FRAMEBUFFER, m_lightFBO);
	glEnablei(GL_BLEND, 0);                                          // ���Z�������s�����߃u�����h��L����
	glBlendFuncSeparatei(0, GL_ONE, GL_ONE, GL_ONE, GL_ONE);         // ���Z�����̃u�����h���w��
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	// �[�x�e�X�g���I�t
	glDisable(GL_DEPTH_TEST);
	// �J�����O�ݒ�F���C�g�̓��b�V���̗����̂ݕ`�悷��
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glFrontFace(GL_CW);

	//------------------------------------------------------+
	// �|�C���g���C�g
	//------------------------------------------------------+
	// �|�C���g���C�g�V�F�[�_�ւ̃Z�b�g
	m_pointLightShader->SetActive();
	m_pointLightShader->SetMatrixUniform("u_view", m_renderer->GetViewMatrix());
	m_pointLightShader->SetMatrixUniform("u_projection", m_renderer->GetProjectionMatrix());
	m_pointLightShader->SetVectorUniform("u_viewPos", m_renderer->GetViewMatrix().GetTranslation());
	m_pointLightShader->SetInt("u_gPos", 0);
	m_pointLightShader->SetInt("u_gNormal", 1);
	m_pointLightShader->SetInt("u_gAlbedoSpec", 2);
	// gBuffer�̊e�e�N�X�`�����o�C���h���Ă���
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_gPos);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_gNormal);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_gAlbedoSpec);

	// �|�C���g���C�g�̕`��
	for (auto pl : m_renderer->m_pointLights)
	{
		pl->Draw(m_pointLightShader);
	}

	// �J�����O�ƃu�����h���~����
	glDisable(GL_CULL_FACE);
	glDisablei(GL_BLEND, 0);

	// gBuffer�̓��e�����C�g�o�b�t�@�փR�s�[���AgBuffer�̐[�x�l�Ɋ�Â����`����s��
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_gBuffer);       // gBuffer��ǂݎ��t���[���o�b�t�@�Ƃ��Ďw��
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_lightFBO);             // �������݃o�b�t�@�����C�g�o�b�t�@�Ɏw��
	// �[�x�����X�N���[�����̐[�x�o�b�t�@�փR�s�[����
	glBlitFramebuffer(0, 0, GAME_CONFIG->GetScreenWidth(), GAME_CONFIG->GetScreenHeight(), 0, 0, GAME_CONFIG->GetScreenWidth(), GAME_CONFIG->GetScreenHeight(), GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	// ���C�g�o�b�t�@�`��֖߂�
	glBindFramebuffer(GL_FRAMEBUFFER, m_lightFBO);
	
}

// �{�`�揈��
void DefferedRenderer::Draw()
{

	// GBuffer�ւ̏�������
	DrawGBuffer();

	// ���C�g�p�X
	DrawLightPass();

	// ���C�gFBO�ւ̏������݂��~�߂�
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	// GBuffer�ɏ������܂ꂽ�v�f���X�N���[���ɕ`��
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);

	m_screenShader->SetActive();
	m_screenShader->SetInt("u_screenTexture", 0);

	// GBuffer�e�N�X�`���Z�b�g
	glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, m_gAlbedoSpec);
	glBindTexture(GL_TEXTURE_2D, m_lightHDR);

	// �X�N���[���ɕ`��
	m_renderer->m_screenVerts->SetActive();
	glDrawArrays(GL_TRIANGLES, 0, 6);



	// ���[���h��ԏ�̃X�v���C�g�`��
	m_renderer->m_worldSpaceSpriteShader->SetActive();
	m_renderer->m_worldSpaceSpriteShader->SetMatrixUniform("u_View", m_renderer->m_view);
	m_renderer->m_worldSpaceSpriteShader->SetMatrixUniform("u_Projection", m_renderer->m_projection);
	for (auto spr : m_renderer->m_worldSprites)
	{
		spr->Draw(m_renderer->m_worldSpaceSpriteShader);
	}

	// Sprite�̕`��
	// �u�����h�̃A�N�e�B�u��
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// �[�x�e�X�g�̒�~
	glDisable(GL_DEPTH_TEST);
	glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

	// sprite�V�F�[�_�[�̃A�N�e�B�u��
	m_renderer->m_spriteVerts->SetActive();
	m_renderer->m_spriteShader->SetActive();

	for (auto sprite : m_renderer->m_spriteComponents)
	{
		if (sprite->GetVisible())
		{
			sprite->Draw(m_renderer->m_spriteShader);
		}
	}

	// �S�Ă�UI���X�V
	for (auto ui : GAME_INSTANCE.GetUIStack())
	{
		ui->Draw(m_renderer->m_spriteShader);
	}
}




// GBuffer���쐬����
// ���W�p�o�b�t�@�E�@���p�o�b�t�@�E�A���x�h���X�y�L�����o�b�t�@�E�����_�[�o�b�t�@
bool DefferedRenderer::CreateGBuffer()
{
	// GBuffer�̓o�^�E�o�C���h
	glGenFramebuffers(1, &m_gBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_gBuffer);

	//----------------------------------------------------------------+
	// �e�o�b�t�@��o�^���A2D�e�N�X�`���Ƃ���GBuffer�ɕR�t����
	//----------------------------------------------------------------+
	// 3D��ԍ��W�o�b�t�@ (���������_�o�b�t�@/�J���[0�ԖڂƂ��ēo�^)
	glGenTextures(1, &m_gPos);
	glBindTexture(GL_TEXTURE_2D, m_gPos);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, GAME_CONFIG->GetScreenWidth(), GAME_CONFIG->GetScreenHeight(), 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_gPos, 0);
	// �@���x�N�g���o�b�t�@ (���������_�o�b�t�@/�J���[1�ԖڂƂ��ēo�^)
	glGenFramebuffers(1, &m_gNormal);
	glBindTexture(GL_TEXTURE_2D, m_gNormal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, GAME_CONFIG->GetScreenWidth(), GAME_CONFIG->GetScreenHeight(), 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_gNormal, 0);
	// �A���x�h(RGB)���X�y�L����(A)�p�J���[�o�b�t�@ (A�����܂�8bit�o�b�t�@)
	glGenTextures(1, &m_gAlbedoSpec);
	glBindTexture(GL_TEXTURE_2D, m_gAlbedoSpec);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, GAME_CONFIG->GetScreenWidth(), GAME_CONFIG->GetScreenHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_gAlbedoSpec, 0);

	// �e�e�N�X�`����GBuffer�̕`���Ƃ���GL���ɖ�������
	unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, attachments);

	// �����_�[�o�b�t�@�̍쐬 (�X�e���V���o�b�t�@�Ƃ��Ē�`)
	glGenRenderbuffers(1, &m_gRBO);
	glBindRenderbuffer(GL_RENDERBUFFER, m_gRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, GAME_CONFIG->GetScreenWidth(), GAME_CONFIG->GetScreenHeight());
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_gRBO);

	// �t���[���o�b�t�@�̐��������`�F�b�N
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR::GBUFFER::Create False" << std::endl;
		return false;
	}
	// �t���[���o�b�t�@�̃o�C���h����
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return true;
}

// ���C�g�o�b�t�@���쐬����
// ���������p��HDR�o�b�t�@�E�����_�[�o�b�t�@
bool DefferedRenderer::CreateLightBuffer()
{
	glGenFramebuffers(1, &m_lightFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_lightFBO);

	// HDR�o�b�t�@�̍쐬
	glGenTextures(1, &m_lightHDR);
	glBindTexture(GL_TEXTURE_2D, m_lightHDR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, GAME_CONFIG->GetScreenWidth(), GAME_CONFIG->GetScreenHeight(), 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_lightHDR, 0);
	// HDR�e�N�X�`����`���0�Ԗڂɐݒ�
	glDrawBuffer(GL_COLOR_ATTACHMENT0);

	// �����_�[�o�b�t�@���쐬����
	glGenRenderbuffers(1, &m_lightRBO);
	glBindRenderbuffer(GL_RENDERBUFFER, m_lightRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, GAME_CONFIG->GetScreenWidth(), GAME_CONFIG->GetScreenHeight());
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_lightRBO);

	// �t���[���o�b�t�@�̐��������`�F�b�N
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR::GBUFFER::Create False" << std::endl;
		return false;
	}
	// �t���[���o�b�t�@�̃o�C���h����
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	return true;
}

