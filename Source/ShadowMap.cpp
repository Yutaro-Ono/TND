#include "ShadowMap.h"
#include "Shader.h"
#include "Renderer.h"
#include "GameMain.h"
#include "GameConfig.h"
#include "CameraComponent.h"
#include "MeshComponent.h"
#include "SkeletalMeshComponent.h"
#include "CarMeshComponent.h"

const int ShadowMap::SHADOW_WIDTH = 8192;
const int ShadowMap::SHADOW_HEIGHT = 8192;

// �R���X�g���N�^
ShadowMap::ShadowMap()
{
	glGenFramebuffers(1, &m_depthMapFBO);
	// �f�v�X�}�b�v���o�C���h
	glGenTextures(1, &m_depthMap);
	glBindTexture(GL_TEXTURE_2D, m_depthMap);
	// �[�x�l�݂̂��K�v�Ȃ̂ŁA�t�H�[�}�b�g��DEPTH_COMPONENT(24bit)��
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	// �e�N�X�`�����b�s���O�E�t�B���^�����O�ݒ�
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	// �t���[���o�b�t�@�Ƀf�v�X�}�b�v���A�^�b�`����
	glBindFramebuffer(GL_FRAMEBUFFER, m_depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthMap, 0);
	// ���̎��_����V�[���������_�����O����ۂ̐[�x���݂̂��K�v
	// ���̂��߃J���[�o�b�t�@���g�p���Ȃ����Ƃ𖾎�����
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	// �t���[���o�b�t�@�̃o�C���h����
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// �V�F�[�_�̍쐬
	// �f�v�X�}�b�v�V�F�[�_
	m_depthShader = new Shader();
	m_depthShader->Load("Data/Shaders/DepthMap.vert", "Data/Shaders/DepthMap.frag");
	//m_shadowShader = new Shader();
	//m_shadowShader->Load("Data/Shaders/PhongShadow.vert", "Data/Shaders/PhongShadow.frag");
	// �m�[�}���}�b�v + �e�V�F�[�_
	m_shadowShader = new Shader();
	m_shadowShader->Load("Data/Shaders/ShadowNormalMap.vert", "Data/Shaders/ShadowNormalMap.frag");
	
	m_depthSkinShader = new Shader();
	m_depthSkinShader->Load("Data/Shaders/SkinnedDepth.vert", "Data/Shaders/DepthMap.frag");
	m_skinShadowShader = new Shader();
	m_skinShadowShader->Load("Data/Shaders/SkinnedShadow.vert", "Data/Shaders/PhongShadow.frag");
	
	//m_depthSkinShader = new Shader();
	//m_depthSkinShader->Load("Data/Shaders/SkinnedDepthNormal.vert", "Data/Shaders/DepthMap.frag");
	//m_skinShadowShader = new Shader();
	//m_skinShadowShader->Load("Data/Shaders/ShadowSkinnedNormal.vert", "Data/Shaders/ShadowNormalMap.frag");

}

ShadowMap::~ShadowMap()
{

	glDeleteFramebuffers(1, &m_depthMapFBO);
	glDeleteTextures(1, &m_depthMap);
	delete m_depthShader;
	delete m_depthSkinShader;
	delete m_shadowShader;
	delete m_skinShadowShader;
}


/// <summary>
/// ���C�g���_����I�u�W�F�N�g��`�悵�A�[�x�}�b�v�𓾂�
/// </summary>
/// <param name="in_mesh"></param>     �V���h�E��K�p���郁�b�V���z��
/// <param name="in_skelMesh"></param> �V���h�E��K�p����X�L�����b�V���z��
void ShadowMap::RenderDepthMapFromLightView(const std::vector<class MeshComponent*>& in_mesh, const std::vector<class SkeletalMeshComponent*> in_skelMesh, const std::vector<class CarMeshComponent*> in_carMesh)
{
	// �[�x�e�X�g�L����
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// ���C�g���_�p�̃v���W�F�N�V�����s��ƃr���[�s���p�ӂ���
	// �f�B���N�V���i�����C�g(���s)�ł��邽�߁A�v���W�F�N�V�����s��ɂ͐��ˉe�s����g�p
	//m_lightProj = Matrix4::CreateOrtho(7000.0f, 7000.0f, 1.0f, 5000.0f);
	m_lightProj = Matrix4::CreateOrtho(7000.0f, 7000.0f, -10.0f, 5000.0f);

	m_lightView = Matrix4::CreateLookAt(RENDERER->GetDirectionalLight().position, RENDERER->GetDirectionalLight().target, Vector3::UnitZ);
	m_lightSpace = m_lightView * m_lightProj;

	// �V���h�E�}�b�v�̓����_�����O���̉𑜓x�Ƃ͈قȂ�A�V���h�E�}�b�v�̃T�C�Y�ɍ��킹��Viewport�p�����[�^��ύX����K�v������
	// ���̂���glViewport���Ăяo���B
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	// �t���[���o�b�t�@�̃o�C���h
	glBindFramebuffer(GL_FRAMEBUFFER, m_depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	m_depthShader->SetActive();
	m_depthShader->SetMatrixUniform("u_lightSpaceMatrix", m_lightSpace);

	// �f�v�X�o�b�t�@�𓾂邽�߂Ƀ��C�g���猩���V�[���������_�����O����
	//----------------------------------------------------------------------+
	for (auto mesh : in_mesh)
	{
		mesh->DrawShadow(m_depthShader);
	}
	for (auto mesh : in_carMesh)
	{
		mesh->DrawShadow(m_depthShader);
	}

	m_depthSkinShader->SetActive();
	m_depthSkinShader->SetMatrixUniform("u_lightSpaceMatrix", m_lightSpace);
	for (auto skel : in_skelMesh)
	{
		if (skel->GetVisible())
		{
			skel->DrawShadow(m_depthSkinShader);
		}

	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//// �r���[�|�[�g����ʃT�C�Y�ɖ߂�
	glViewport(0, 0, GAME_CONFIG->GetScreenWidth(), GAME_CONFIG->GetScreenHeight());
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}


// �V���h�E�ƃ��b�V���̕`�� (�X�L�����b�V���͑ΏۊO)
void ShadowMap::DrawShadowMesh(const std::vector<class MeshComponent*>& in_mesh)
{
	// �T���v�����O�p�e�N�X�`���Z�b�g
	m_shadowShader->SetInt("u_mat.diffuseMap", 0);
	m_shadowShader->SetInt("u_mat.specularMap", 1);
	m_shadowShader->SetInt("u_mat.normalMap", 2);
	m_shadowShader->SetInt("u_mat.depthMap", 3);
	
	// �V���h�E�V�F�[�_�ɂ�郁�b�V���`��
	for (auto mesh : in_mesh)
	{
		mesh->Draw(m_shadowShader);
	}

}

void ShadowMap::DrawShadowMesh(const std::vector<class MeshComponent*>& in_mesh, const std::vector<class SkeletalMeshComponent*>& in_skelMesh)
{
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	// �r���[�|�[�g����ʃT�C�Y�ɖ߂�
	glViewport(0, 0, GAME_CONFIG->GetScreenWidth(), GAME_CONFIG->GetScreenHeight());
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// �V���h�E�V�F�[�_�̃A�N�e�B�u���Euniform�ւ̃Z�b�g
	m_shadowShader->SetActive();
	m_shadowShader->SetVectorUniform("u_dirLight.direction", RENDERER->GetDirectionalLight().direction);
	m_shadowShader->SetVectorUniform("u_dirLight.ambient", RENDERER->GetDirectionalLight().ambient);
	m_shadowShader->SetVectorUniform("u_dirLight.diffuse", RENDERER->GetDirectionalLight().diffuse);
	m_shadowShader->SetVectorUniform("u_dirLight.specular", RENDERER->GetDirectionalLight().specular);

	m_shadowShader->SetMatrixUniform("u_view", RENDERER->GetViewMatrix());
	m_shadowShader->SetMatrixUniform("u_projection", RENDERER->GetProjectionMatrix());
	m_shadowShader->SetMatrixUniform("u_lightSpaceMatrix", m_lightSpace);
	m_shadowShader->SetVectorUniform("u_viewPos", RENDERER->GetViewMatrix().GetTranslation());
	m_shadowShader->SetVectorUniform("u_lightPos", RENDERER->GetDirectionalLight().position);

	// �T���v�����O�p�e�N�X�`���Z�b�g
	m_shadowShader->SetInt("u_mat.diffuseMap", 0);
	m_shadowShader->SetInt("u_mat.specularMap", 1);
	m_shadowShader->SetInt("u_mat.normalMap", 2);
	m_shadowShader->SetInt("u_mat.depthMap", 3);

	// �V���h�E�V�F�[�_�ɂ�郁�b�V���`��
	for (auto mesh : in_mesh)
	{
		mesh->Draw(m_shadowShader);
	}

	// �V���h�E�V�F�[�_�̃A�N�e�B�u���Euniform�ւ̃Z�b�g
	m_skinShadowShader->SetActive();
	m_skinShadowShader->SetVectorUniform("u_viewPos", RENDERER->GetViewMatrix().GetTranslation());
	m_skinShadowShader->SetVectorUniform("u_dirLight.direction", RENDERER->GetDirectionalLight().direction);
	m_skinShadowShader->SetVectorUniform("u_dirLight.ambient", RENDERER->GetDirectionalLight().ambient);
	m_skinShadowShader->SetVectorUniform("u_dirLight.diffuse", RENDERER->GetDirectionalLight().diffuse);
	m_skinShadowShader->SetVectorUniform("u_dirLight.specular", RENDERER->GetDirectionalLight().specular);

	m_skinShadowShader->SetMatrixUniform("u_view", RENDERER->GetViewMatrix());
	m_skinShadowShader->SetMatrixUniform("u_projection", RENDERER->GetProjectionMatrix());
	m_skinShadowShader->SetMatrixUniform("u_lightSpaceMatrix", m_lightSpace);
	m_skinShadowShader->SetVectorUniform("u_lightPos", RENDERER->GetDirectionalLight().position);

	// �T���v�����O�p�e�N�X�`���Z�b�g
	m_skinShadowShader->SetInt("u_mat.diffuseMap", 0);
	m_skinShadowShader->SetInt("u_mat.specularMap", 1);
	m_skinShadowShader->SetInt("u_mat.normalMap", 2);
	m_skinShadowShader->SetInt("u_mat.depthMap", 3);
	// �V���h�E�V�F�[�_�ɂ��X�L�����b�V���`��
	for (auto skel : in_skelMesh)
	{
		if (skel->GetVisible())
		{
			skel->Draw(m_skinShadowShader);
		}
	}

}

