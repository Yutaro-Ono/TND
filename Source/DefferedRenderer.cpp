//-------------------------------------------------------------------------------------------+
//
// �x�������_�����O�N���X (GBuffer�Ɋe�`�挋�ʂ�ۑ��EGBuffer�ɑ΂��Č����������s��)
//
//-------------------------------------------------------------------------------------------+
#include "DefferedRenderer.h"
#include <stdlib.h>
#include <iostream>
#include <glad/glad.h>
#include "Shader.h"
#include "Renderer.h"
#include "GameMain.h"
#include "GameConfig.h"
#include "MeshComponent.h"
#include "SkeletalMeshComponent.h"
#include "EnvironmentMapComponent.h"
#include "CubeMapComponent.h"
#include "VertexArray.h"
#include "PointLightComponent.h"
#include "SpotLightComponent.h"
#include "SpriteComponent.h"
#include "WorldSpaceUI.h"
#include "UIScreen.h"
#include "RenderBloom.h"
#include "FrameBuffer.h"
#include "ParticleManager.h"
#include "CarMeshComponent.h"
#include "CameraComponent.h"
#include "LightGlassComponent.h"
#include "MiniMapHUD.h"

// �R���X�g���N�^
DefferedRenderer::DefferedRenderer(Renderer* in_renderer)
	:m_renderer(in_renderer)
	,m_simpleMeshShader(nullptr)
	,m_meshShader(nullptr)
	,m_skinShader(nullptr)
	,m_skyBoxShader(nullptr)
    ,m_envShader(nullptr)
    ,m_carShader(nullptr)
    ,m_lightGlassShader(nullptr)
	,m_screenShader(nullptr)
	,m_pointLightShader(nullptr)
	,m_directionalLightShader(nullptr)
	,m_spotLightShader(nullptr)
	,m_gBuffer(0)
	,m_gPos(0)
	,m_gNormal(0)
	,m_gAlbedoSpec(0)
	,m_gEmissive(0)
	,m_lightHDR(0)
	,m_uboGBuffer(0)
{
	std::cout << "CREATE::DefferedRenderer::Instance" << std::endl;
}

// �f�X�g���N�^
DefferedRenderer::~DefferedRenderer()
{
	delete m_simpleMeshShader;
	delete m_meshShader;
	delete m_skinShader;
	delete m_envShader;
	delete m_carShader;
	delete m_skyBoxShader;
	delete m_screenShader;
	delete m_pointLightShader;
	delete m_directionalLightShader;
	delete m_spotLightShader;
}

// GBuffer�ւ̏������ݏ���
void DefferedRenderer::DrawGBuffer()
{
	// �}�b�vHUD�������ݏ���
	if (m_renderer->GetMapHUD() != nullptr)
	{
		m_renderer->GetMapHUD()->WriteBuffer(m_renderer->m_mapInputShader, m_renderer->m_meshComponents);
	}
	// �`����GBuffer�Ƃ��ăo�C���h����
	glBindFramebuffer(GL_FRAMEBUFFER, m_gBuffer);
	// �r���[�|�[�g���X�N���[���T�C�Y�ɃZ�b�g
	glViewport(0, 0, GAME_CONFIG->GetScreenWidth(), GAME_CONFIG->GetScreenHeight());
	glClearColor(0.0, 0.0, 0.0, 0.0);     // �J���[�̃N���A
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// �[�x�e�X�g��ON
	glEnable(GL_DEPTH_TEST);

	// ���C�g��Ԃ̊e�s����`
	Matrix4 lightSpace, lightView, lightProj;
	lightProj = Matrix4::CreateOrtho(7000.0f, 7000.0f, 1.0f, 5000.0f);
	lightView = Matrix4::CreateLookAt(RENDERER->GetDirectionalLight().position, RENDERER->GetDirectionalLight().target, Vector3::UnitZ);
	lightSpace = lightView * lightProj;

	//-----------------------------------------------------------+
	// �ʏ탁�b�V��
	//-----------------------------------------------------------+
	// �V�F�[�_��uniform�Z�b�g
	m_meshShader->SetActive();
	m_meshShader->SetVectorUniform("u_dirLight.direction", RENDERER->GetDirectionalLight().direction);
	m_meshShader->SetVectorUniform("u_dirLight.ambient", RENDERER->GetDirectionalLight().ambient);
	m_meshShader->SetVectorUniform("u_dirLight.diffuse", RENDERER->GetDirectionalLight().diffuse);
	m_meshShader->SetVectorUniform("u_dirLight.specular", RENDERER->GetDirectionalLight().specular);
	m_meshShader->SetMatrixUniform("u_lightSpaceMatrix", lightSpace);
	m_meshShader->SetVectorUniform("u_lightPos", RENDERER->GetDirectionalLight().position);
	m_meshShader->SetInt("u_mat.diffuseMap", 0);
	m_meshShader->SetInt("u_mat.specularMap", 1);
	m_meshShader->SetInt("u_mat.normalMap", 2);
	m_meshShader->SetInt("u_mat.emissiveMap", 3);
	m_meshShader->SetInt("u_mat.depthMap", 4);
	// ���b�V���`�� (������GBuffer�̊e�v�f�ɏ�񂪏������܂��)
	for (auto mesh : m_renderer->m_meshComponents)
	{
		mesh->Draw(m_meshShader);
	}

	
	//------------------------------------------------------------+
	// �X�L�����b�V��
	//------------------------------------------------------------+
	// �V�F�[�_��uniform�Z�b�g
	m_skinShader->SetActive();
	m_skinShader->SetVectorUniform("u_dirLight.direction", RENDERER->GetDirectionalLight().direction);
	m_skinShader->SetVectorUniform("u_dirLight.ambient", RENDERER->GetDirectionalLight().ambient);
	m_skinShader->SetVectorUniform("u_dirLight.diffuse", RENDERER->GetDirectionalLight().diffuse);
	m_skinShader->SetVectorUniform("u_dirLight.specular", RENDERER->GetDirectionalLight().specular);
	m_skinShader->SetVectorUniform("u_viewPos", m_renderer->m_view.GetTranslation());
	m_skinShader->SetMatrixUniform("u_lightSpaceMatrix", lightSpace);
	m_skinShader->SetVectorUniform("u_lightPos", RENDERER->GetDirectionalLight().position);
	m_skinShader->SetInt("u_mat.diffuseMap", 0);
	m_skinShader->SetInt("u_mat.specularMap", 1);
	m_skinShader->SetInt("u_mat.normalMap", 2);
	m_skinShader->SetInt("u_mat.emissiveMap", 3);
	m_skinShader->SetInt("u_mat.depthMap", 4);
	// ���b�V���`�� (������GBuffer�̊e�v�f�ɏ�񂪏������܂��)
	for (auto skel : m_renderer->m_skeletalMeshComponents)
	{
		skel->Draw(m_skinShader);
	}

	//------------------------------------------------------------+
	// ��
	//------------------------------------------------------------+
	m_carShader->SetActive();
	m_carShader->SetVectorUniform("u_dirLight.direction", RENDERER->GetDirectionalLight().direction);
	m_carShader->SetVectorUniform("u_dirLight.ambient", RENDERER->GetDirectionalLight().ambient);
	m_carShader->SetVectorUniform("u_dirLight.diffuse", RENDERER->GetDirectionalLight().diffuse);
	m_carShader->SetVectorUniform("u_dirLight.specular", RENDERER->GetDirectionalLight().specular);
	m_carShader->SetMatrixUniform("u_lightSpaceMatrix", lightSpace);
	m_carShader->SetVectorUniform("u_lightPos", RENDERER->GetDirectionalLight().position);
	m_carShader->SetInt("u_mat.diffuseMap", 0);
	m_carShader->SetInt("u_mat.specularMap", 1);
	m_carShader->SetInt("u_mat.depthMap", 2);
	// �ԃ��b�V���`��
	for (auto car : m_renderer->m_carMeshComponents)
	{
		car->Draw(m_carShader);
	}

	//------------------------------------------------------------+
    // SkyBox
    //------------------------------------------------------------+
	m_skyBoxShader->SetActive();
	// Uniform�ɋt�s����Z�b�g
	Matrix4 InvView = m_renderer->m_view;
	InvView.Invert();
	m_skyBoxShader->SetMatrixUniform("u_invView", InvView);
	m_skyBoxShader->SetMatrixUniform("u_projection", m_renderer->m_projection);
	m_skyBoxShader->SetInt("u_skybox", 0);
	m_renderer->GetSkyBox()->Draw(m_skyBoxShader);

	//------------------------------------------------------------+
	// EnvironmentMap
	//------------------------------------------------------------+
	// ���b�V�������̃J�����O
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	// uniform�Z�b�g
	m_envShader->SetActive();
	m_envShader->SetVectorUniform("u_viewPos", m_renderer->m_view.GetTranslation());
	m_envShader->SetInt("u_skybox", 0);
	for (auto env : m_renderer->m_envMeshComponents)
	{
		env->DrawEnvironmentMap(m_envShader);
	}
	// �J�����O�̃I�t
	glDisable(GL_CULL_FACE);

	//------------------------------------------------------------+
    // ���C�g�O���X
    //------------------------------------------------------------+
	m_lightGlassShader->SetActive();
	m_lightGlassShader->SetVectorUniform("u_viewPos", m_renderer->m_view.GetTranslation());
	m_lightGlassShader->SetInt("u_skybox", 0);
	for (auto light : m_renderer->m_lightGlassComponents)
	{
		light->Draw(m_lightGlassShader);
	}

	// GBuffer�̃o�C���h����
	glBindFramebuffer(GL_FRAMEBUFFER, 0);



}

// GBuffer�����Ƀ��C�e�B���O�v�Z���s��
void DefferedRenderer::DrawLightPass()
{
	// ���C�g�o�b�t�@���o�C���h
	glBindFramebuffer(GL_FRAMEBUFFER, m_lightFBO);

	// �u�����h�w��
	glEnablei(GL_BLEND, 0);                                          // ���Z�������s�����߃u�����h��L����
	glBlendFuncSeparatei(0, GL_ONE, GL_ONE, GL_ONE, GL_ONE);         // ���Z�����̃u�����h���w��
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// �[�x�e�X�g���I�t
	glDisable(GL_DEPTH_TEST);

	// gBuffer�̊e�e�N�X�`�����o�C���h
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_gPos);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_gNormal);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_gAlbedoSpec);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, m_gEmissive);

	// �J�����O�ݒ�F���C�g�̓��b�V���̗����̂ݕ`�悷��
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glFrontFace(GL_CCW);

	//------------------------------------------------------+
	// �|�C���g���C�g
	//------------------------------------------------------+
	// �|�C���g���C�g�V�F�[�_�ւ̃Z�b�g
	m_pointLightShader->SetActive();
	m_pointLightShader->SetVectorUniform("u_viewPos",    GAME_INSTANCE.GetViewVector());
	m_pointLightShader->SetInt("u_gBuffer.pos",     0);
	m_pointLightShader->SetInt("u_gBuffer.normal",       1);
	m_pointLightShader->SetInt("u_gBuffer.albedoSpec",   2);
	m_pointLightShader->SetInt("u_gBuffer.emissive", 3);
	// �|�C���g���C�g�̕`��
	for (auto pl : m_renderer->m_pointLights)
	{
		pl->Draw(m_pointLightShader);
	}


	//------------------------------------------------------+
	// �X�|�b�g���C�g
	//------------------------------------------------------+
	// �X�|�b�g���C�g�V�F�[�_�ւ̃Z�b�g
	m_spotLightShader->SetActive();
	m_spotLightShader->SetMatrixUniform("u_view", m_renderer->GetViewMatrix());
	m_spotLightShader->SetMatrixUniform("u_projection", m_renderer->GetProjectionMatrix());
	m_spotLightShader->SetVectorUniform("u_viewPos", m_renderer->GetViewMatrix().GetTranslation());
	m_spotLightShader->SetInt("u_gBuffer.pos", 0);
	m_spotLightShader->SetInt("u_gBuffer.normal", 1);
	m_spotLightShader->SetInt("u_gBuffer.albedoSpec", 2);
	m_spotLightShader->SetInt("u_gBuffer.emissive", 3);
	// �X�|�b�g���C�g�̕`��
	for (auto spotL : m_renderer->m_spotLights)
	{
		spotL->Draw(m_spotLightShader);
	}

	// �J�����O�̃I�t
	glDisable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	//glFrontFace(GL_CW);


	//-----------------------------------------------+
	// �f�B���N�V���i�����C�g�p�X
	//-----------------------------------------------+
	// �P�x��`
	float intensity = 1.65f;
	// �V�F�[�_�̃Z�b�g
	m_directionalLightShader->SetActive();
	m_directionalLightShader->SetVectorUniform("u_viewPos", GAME_INSTANCE.GetViewVector());
	m_directionalLightShader->SetVectorUniform("u_dirLight.direction",    m_renderer->m_directionalLight.direction);
	m_directionalLightShader->SetVectorUniform("u_dirLight.ambientColor", m_renderer->m_directionalLight.ambient);
	m_directionalLightShader->SetVectorUniform("u_dirLight.color",        m_renderer->m_directionalLight.diffuse);
	m_directionalLightShader->SetVectorUniform("u_dirLight.specular",     m_renderer->m_directionalLight.specular);
	m_directionalLightShader->SetFloat("u_dirLight.intensity", intensity);
	m_directionalLightShader->SetInt("u_gBuffer.pos", 0);
	m_directionalLightShader->SetInt("u_gBuffer.normal", 1);
	m_directionalLightShader->SetInt("u_gBuffer.albedoSpec", 2);
	m_directionalLightShader->SetInt("u_gBuffer.emissive", 3);
	// �X�N���[���S�̂ɕ`��
	m_renderer->GetScreenVAO()->SetActive();
	glDrawArrays(GL_TRIANGLES, 0, 6);

	// Sprite�̕`��
	// �u�����h�̃A�N�e�B�u��
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// �[�x�e�X�g�̒�~
	glDisable(GL_DEPTH_TEST);
	glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);


	// ���[���h��ԏ�̃X�v���C�g�`��
	Matrix4 view = m_renderer->GetViewMatrix();
	Matrix4 projection = m_renderer->GetProjectionMatrix();
	m_bloomWorldSpriteShader->SetActive();
	m_bloomWorldSpriteShader->SetMatrixUniform("u_view", view);
	m_bloomWorldSpriteShader->SetMatrixUniform("u_projection", projection);
	for (auto spr : m_renderer->m_worldSprites)
	{
		spr->Draw(m_bloomWorldSpriteShader);
	}

	// sprite�V�F�[�_�[�̃A�N�e�B�u��
	m_bloomSpriteShader->SetActive();
	m_bloomSpriteShader->SetFloat("u_intensity", 1.0f);
	RENDERER->SetActiveSpriteVAO();
	for (auto sprite : m_renderer->m_spriteComponents)
	{
		if (sprite->GetVisible())
		{
			sprite->Draw(m_bloomSpriteShader);
		}
	}
	// �S�Ă�UI���X�V
	for (auto ui : GAME_INSTANCE.GetUIStack())
	{
		ui->Draw(m_bloomSpriteShader);
	}

	// �}�b�vHUD
	if (m_renderer->GetMapHUD() != nullptr)
	{
		m_renderer->GetMapHUD()->Draw(m_renderer->m_mapOutputShader);
	}

	// �u�����h���I�t
	glDisable(GL_BLEND);

	// �u�����h���~����
	glDisablei(GL_BLEND, 0);

	// gBuffer�̐[�x�������C�g�o�b�t�@�փR�s�[����
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_gBuffer);              // gBuffer��ǂݎ��t���[���o�b�t�@�Ƃ��Ďw��
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_lightFBO);             // �������݃o�b�t�@�����C�g�o�b�t�@�Ɏw��
	glBlitFramebuffer(0, 0, GAME_CONFIG->GetScreenWidth(), GAME_CONFIG->GetScreenHeight(), 0, 0, GAME_CONFIG->GetScreenWidth(), GAME_CONFIG->GetScreenHeight(), GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	// ���C�g�o�b�t�@�`��֖߂�
	glBindFramebuffer(GL_FRAMEBUFFER, m_lightFBO);


	// �[�x�e�X�g���I��
	glEnable(GL_DEPTH_TEST);
	
	//----------------------------------------------------------------+
    // �p�[�e�B�N���`��
    //----------------------------------------------------------------+
	m_renderer->GetParticleManager()->Draw();


	// ���C�gFBO�ւ̏������݂��~�߂�
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

// �{�`�揈��
void DefferedRenderer::Draw()
{

	// GBuffer�ւ̏�������
	DrawGBuffer();

	// ���C�g�o�b�t�@�ւ̏�������
	DrawLightPass();

	// Bloom�������{�����`��
	RenderBloom* bloom = m_renderer->GetBloom();
	//bloom->SetExposureVal(4.5f);
	bloom->DrawDownSampling(m_lightHighBright);
	bloom->DrawGaussBlur();
	bloom->DrawBlendBloom(m_lightHDR);

	//----------------------------------------------------------------+
	// �ŏI�o�͌��ʂ�`��
	//----------------------------------------------------------------+
	// GBuffer�ɏ������܂ꂽ�v�f���X�N���[���ɕ`��
	//glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glDisable(GL_DEPTH_TEST);
	//// �X�N���[���V�F�[�_��uniform�Z�b�g
	//m_screenShader->SetActive();
	//m_screenShader->SetInt("u_screenTexture", 0);

	//// GBuffer�e�N�X�`���Z�b�g
	//glActiveTexture(GL_TEXTURE0);
	////glBindTexture(GL_TEXTURE_2D, m_lightHDR);
	////glBindTexture(GL_TEXTURE_2D, m_gAlbedoSpec);
	//glBindTexture(GL_TEXTURE_2D, m_gEmissive);
	////glBindTexture(GL_TEXTURE_2D, m_gBrightBuffer);

	//// �X�N���[���ɕ`��
	//m_renderer->m_screenVerts->SetActive();
	//glDrawArrays(GL_TRIANGLES, 0, 6);

}

void DefferedRenderer::LinkUniformBuffers()
{
	m_meshShader->LinkUniformBuffer();
	m_skinShader->LinkUniformBuffer();
	m_carShader->LinkUniformBuffer();
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
	glGenTextures(1, &m_gNormal);
	glBindTexture(GL_TEXTURE_2D, m_gNormal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, GAME_CONFIG->GetScreenWidth(), GAME_CONFIG->GetScreenHeight(), 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_gNormal, 0);
	// �A���x�h(RGB)���X�y�L����(A)�p�J���[�o�b�t�@ (A�����܂�8bit�J���[�o�b�t�@/2�ԖڂƂ��ēo�^)
	glGenTextures(1, &m_gAlbedoSpec);
	glBindTexture(GL_TEXTURE_2D, m_gAlbedoSpec);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, GAME_CONFIG->GetScreenWidth(), GAME_CONFIG->GetScreenHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_gAlbedoSpec, 0);
	// ���P�x�o�b�t�@�̍쐬 (�G�~�b�V�u�o�͗p�P�x�o�b�t�@/3�ԖڂƂ��ēo�^)
	glGenTextures(1, &m_gEmissive);
	glBindTexture(GL_TEXTURE_2D, m_gEmissive);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, GAME_CONFIG->GetScreenWidth(), GAME_CONFIG->GetScreenHeight(), 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, m_gEmissive, 0);


	// �e�e�N�X�`����GBuffer�̕`���Ƃ���GL���ɖ�������
	m_gAttachments[0] = { GL_COLOR_ATTACHMENT0 };
	m_gAttachments[1] = { GL_COLOR_ATTACHMENT1 };
	m_gAttachments[2] = { GL_COLOR_ATTACHMENT2 };
	m_gAttachments[3] = { GL_COLOR_ATTACHMENT3 };
	glDrawBuffers(4, m_gAttachments);

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
	// ���P�x�o�b�t�@�̍쐬
	glGenTextures(1, &m_lightHighBright);
	glBindTexture(GL_TEXTURE_2D, m_lightHighBright);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, GAME_CONFIG->GetScreenWidth(), GAME_CONFIG->GetScreenHeight(), 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_lightHighBright, 0);
	// �A�^�b�`�����g�ݒ�
	m_lightAttachments[0] = { GL_COLOR_ATTACHMENT0 };
	m_lightAttachments[1] = { GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, m_lightAttachments);

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


/// <summary> 
/// Deffered�����_�����O�ɗp����ϐ��̏������E�C���X�^���X�̐���
/// </summary>
/// <returns></returns>
bool DefferedRenderer::Initialize()
{
	// G�o�b�t�@�ƃ��C�g�o�b�t�@�̍쐬
	CreateGBuffer();
	CreateLightBuffer();

	//--------------------------------------------------------------------------------------------------------------------+
	// GBuffer�o�͗p�V�F�[�_
	//--------------------------------------------------------------------------------------------------------------------+
	// �V���v���ȃ��b�V���p�V�F�[�_
	m_simpleMeshShader = new Shader();
	if (!m_simpleMeshShader->Load("Data/Shaders/BasicMesh.vert", "Data/Shaders/BasicMesh.frag"))
	{
		return false;
	}
	// GBuffer�p���b�V���V�F�[�_�̍쐬
	m_meshShader = new Shader();
	if (!m_meshShader->Load("Data/Shaders/GBuffer/gBuffer_NormShadow.vert", "Data/Shaders/GBuffer/gBuffer_NormShadow.frag"))
	{
		return false;
	}

	// GBuffer�p�X�L���V�F�[�_�̍쐬
	m_skinShader = new Shader();
	if (!m_skinShader->Load("Data/Shaders/GBuffer/gBuffer_SkinNormShadow.vert", "Data/Shaders/GBuffer/gBuffer_Shadow.frag"))
	{
		return false;
	}

	// GBuffer�p�X�J�C�{�b�N�X�V�F�[�_
	m_skyBoxShader = new Shader();
	if (!m_skyBoxShader->Load("Data/Shaders/GBuffer/gBuffer_SkyBox.vert", "Data/Shaders/GBuffer/gBuffer_SkyBox.frag"))
	{
		return false;
	}
	// GBuffer�p���}�b�v�V�F�[�_
	m_envShader = new Shader();
	if (!m_envShader->Load("Data/Shaders/GBuffer/gBuffer_EnvironmentMap.vert", "Data/Shaders/GBuffer/gBuffer_EnvironmentMap.frag"))
	{
		return false;
	}
	// GBuffer�ԗp�V�F�[�_
	m_carShader = new Shader();
	if (!m_carShader->Load("Data/Shaders/GBuffer/gBuffer_CarShaderReflect.vert", "Data/Shaders/GBuffer/gBuffer_CarShaderReflect.frag"))
	{
		return false;
	}

	// GBuffer���C�g�O���X�V�F�[�_
	m_lightGlassShader = new Shader();
	if (!m_lightGlassShader->Load("Data/Shaders/GBuffer/GBuffer_LightGlass.vert", "Data/Shaders/GBuffer/GBuffer_LightGlass.frag"))
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
	if (!m_pointLightShader->Load("Data/Shaders/GBuffer/PointLight.vert", "Data/Shaders/GBuffer/PointLight_Bloom.frag"))
	{
		return false;
	}
	// �f�B���N�V���i�����C�g�V�F�[�_
	m_directionalLightShader = new Shader();
	if (!m_directionalLightShader->Load("Data/Shaders/GBuffer/DirectionalLight.vert", "Data/Shaders/GBuffer/DirectionalLight_Bloom.frag"))
	{
		return false;
	}
	// �X�|�b�g���C�g�V�F�[�_
	m_spotLightShader = new Shader();
	if (!m_spotLightShader->Load("Data/Shaders/GBuffer/SpotLight_Bloom.vert", "Data/Shaders/GBuffer/SpotLight_Bloom.frag"))
	{
		return false;
	}

	// �X�v���C�g�V�F�[�_
	m_bloomSpriteShader = new Shader();
	if (!m_bloomSpriteShader->Load("Data/Shaders/SpriteShader.vert", "Data/Shaders/GBuffer/SpriteShader_Bloom.frag"))
	{
		return false;
	}
	m_bloomSpriteShader->SetActive();
	// �X�N���[���p�̍s����쐬 (UI��X�v���C�g�͈ȍ~���̍s�����ɕ`��)
	Matrix4 viewProj = Matrix4::CreateSimpleViewProj(m_renderer->m_screenWidth, m_renderer->m_screenHeight);
	m_bloomSpriteShader->SetMatrixUniform("u_viewProj", viewProj);

	// ���[���h�X�v���C�g�V�F�[�_
	m_bloomWorldSpriteShader = new Shader();
	if (!m_bloomWorldSpriteShader->Load("Data/Shaders/GBuffer/Particle_Bloom.vert", "Data/Shaders/GBuffer/WorldSpaceSprite_Bloom.frag"))
	{
		return false;
	}


	return true;
}