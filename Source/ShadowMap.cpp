#include "ShadowMap.h"
#include "Shader.h"
#include "Renderer.h"
#include "GameMain.h"
#include "GameConfig.h"
#include "MeshComponent.h"

const int ShadowMap::SHADOW_WIDTH = 4096;
const int ShadowMap::SHADOW_HEIGHT = 4096;

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

	// ��ʑS�̂𕢂����_���`
	unsigned int vbo;
	float quadVertices[] =
	{
		// x   // y   // z  // u  // v
		-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
		 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	};
	glGenVertexArrays(1, &m_screenVAO);
	glGenBuffers(1, &m_screenVAO);;
	glBindVertexArray(m_screenVAO);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	// �V�F�[�_�̍쐬
	m_depthShader = new Shader();
	m_depthShader->Load("Data/Shaders/depthMap.vert", "Data/Shaders/depthMap.frag");
	m_shadowShader = new Shader();
	m_shadowShader->Load("Data/Shaders/ShadowMap.vert", "Data/Shaders/ShadowMap.frag");
}

ShadowMap::~ShadowMap()
{

	glDeleteFramebuffers(1, &m_depthMapFBO);
	glDeleteTextures(1, &m_depthMap);
	glDeleteVertexArrays(1, &m_screenVAO);
	m_depthShader->Delete();
	m_shadowShader->Delete();
}

void ShadowMap::RenderDepthMapFromLightView(Renderer* in_renderer, const std::vector<class MeshComponent*>& in_mesh)
{
	// ���C�g���_�p�̃v���W�F�N�V�����s��ƃr���[�s���p�ӂ���
    // �f�B���N�V���i�����C�g(���s)�ł��邽�߁A�v���W�F�N�V�����s��ɂ͐��ˉe�s����g�p

	// ���C�g���_�̒����_ (�v���C���[�̍��W������\��)
	Vector3 lightViewTraget = Vector3(1800.0f, 2400.0f, 0.0f);

	m_lightProj = Matrix4::CreateOrtho(static_cast<float>(GAME_CONFIG->GetScreenWidth()), static_cast<float>(GAME_CONFIG->GetScreenHeight()),
		1.0f, 100000.0f);
	m_lightView = Matrix4::CreateLookAt(in_renderer->GetDirectionalLight().m_position, lightViewTraget, Vector3::UnitZ);
	m_lightSpace = m_lightView * m_lightProj;

	// �V���h�E�}�b�v�̓����_�����O���̉𑜓x�Ƃ͈قȂ�A�V���h�E�}�b�v�̃T�C�Y�ɍ��킹��Viewport�p�����[�^��ύX����K�v������
    // ���̂���glViewport���Ăяo���B
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	// �t���[���o�b�t�@�̃o�C���h
	glBindFramebuffer(GL_FRAMEBUFFER, m_depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	m_depthShader->SetActive();
	m_depthShader->SetMatrixUniform("lightSpace", m_lightSpace);

	// �f�v�X�o�b�t�@�𓾂邽�߂Ƀ��C�g���猩���V�[���������_�����O����
	//----------------------------------------------------------------------+
	for (auto mesh : in_mesh)
	{
		mesh->Draw(m_depthShader);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

// �V���h�E�ƃ��b�V���̕`��
void ShadowMap::Draw(const std::vector<class MeshComponent*>& in_mesh)
{
	// �r���[�|�[�g�����ɖ߂�
	glViewport(0, 0, GAME_CONFIG->GetScreenWidth(), GAME_CONFIG->GetScreenHeight());
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// �V���h�E�V�F�[�_�̃A�N�e�B�u���Euniform�ւ̃Z�b�g
	m_shadowShader->SetActive();
	m_shadowShader->SetVectorUniform("u_viewPos", RENDERER->GetViewMatrix().GetTranslation());
	m_shadowShader->SetVectorUniform("u_lightPos", RENDERER->GetDirectionalLight().m_position);
	m_shadowShader->SetMatrixUniform("u_lightSpace", m_lightSpace);
	// �T���v�����O�p�e�N�X�`���Z�b�g
	m_shadowShader->SetInt("u_mat.diffuseMap", 0);
	m_shadowShader->SetInt("u_mat.specularMao", 1);
	m_shadowShader->SetInt("u_mat.normalMap", 2);
	m_shadowShader->SetInt("u_mat.shadowMap", 3);
	// 3�ԖڂɃf�v�X�}�b�v���Z�b�g
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, m_depthMap);
	
	// ���b�V���`��
	for (auto mesh : in_mesh)
	{
		mesh->Draw(m_shadowShader);
	}

}
