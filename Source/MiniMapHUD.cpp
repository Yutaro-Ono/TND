#include "MiniMapHUD.h"
#include "GameMain.h"
#include "Renderer.h"
#include "Shader.h"
#include "GameWorld.h"
#include "PlayerManager.h"
#include "Texture.h"
#include "MeshComponent.h"
#include "LandMarkIndicator.h"

MiniMapHUD::MiniMapHUD(PlayerManager* in_target)
	:m_target(in_target)
	,m_miniMapFBO(0)
	,m_mapBuffer(0)
	,m_rbo(0)
	,m_width(1024)
	,m_height(512)
	,m_scale(0.7f)
	,m_screenPos(Vector2(-GAME_CONFIG->GetScreenWidth() / 2 + 35.0f, -GAME_CONFIG->GetScreenHeight() / 2 + m_height / 2 + 5.0f))
	,m_mapTex(nullptr)
{
	// �^�[�Q�b�g�A�N�^�̍��W���擾���A�}�b�v�`��̂��߂ɍ����֐ݒ�
	m_viewPos = m_target->GetPosition() + Vector3(0.0f, 0.0f, 4500.0f);

	// �v���W�F�N�V�����s����`
	m_projection = Matrix4::CreateOrtho(
		static_cast<float>(m_width) * 15.0f,
		static_cast<float>(m_height) * 15.0f,
		1.0f, 6000.0f);

	CreateFBO(m_miniMapFBO);
	RENDERER->SetMapHUD(this);

	// �}�b�v�e�N�X�`���𐶐�
	m_mapTex = new Texture();
	m_mapTex->Load("Data/Interface/HUD/Map/MapHUD.png");

	// HUD��̖��𐶐�
	m_landMark = new LandMarkIndicator(in_target);
}

MiniMapHUD::~MiniMapHUD()
{
	
}

// ���f�����}�b�v�o�b�t�@�ɏ�������
void MiniMapHUD::WriteBuffer(Shader* in_shader, std::vector<class MeshComponent*> in_mesh)
{

	// �t���[���o�b�t�@�̃o�C���h
	glBindFramebuffer(GL_FRAMEBUFFER, m_miniMapFBO);
	// �r���[�|�[�g��ύX
	glViewport(0, 0, m_width, m_height);
	glClearColor(0.0, 0.0, 0.0, 0.0);     // �J���[�̃N���A
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// �[�x�e�X�g��ON
	glEnable(GL_DEPTH_TEST);

	// �^�[�Q�b�g�A�N�^�̍��W���擾���A�}�b�v�`��̂��߂ɍ����֐ݒ�
	m_viewPos = m_target->GetPosition() + Vector3(0.0f, -3900.0f, 4500.0f);
	Matrix4 view = Matrix4::CreateLookAt(m_viewPos, m_target->GetPosition() + Vector3(0.0f, -3900.0f, 0.0f), Vector3::UnitX);
	// �r���[�E�v���W�F�N�V���������s����쐬
	Matrix4 viewProj = view * m_projection;
	
	in_shader->SetActive();
	in_shader->SetMatrixUniform("u_viewProj", viewProj);
	glClearColor(1.0f, 0.6f, 0.0f, 1.0f);

	// �o�b�t�@�ɏ�������
	for (auto mesh : in_mesh)
	{
		mesh->DrawMap(in_shader);
	}

	// �t���[���o�b�t�@�̃o�C���h����
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//// �r���[�|�[�g����ʃT�C�Y�ɖ߂�
	glViewport(0, 0, GAME_CONFIG->GetScreenWidth(), GAME_CONFIG->GetScreenHeight());

}

// �������܂ꂽ�o�b�t�@��`�悷��
void MiniMapHUD::Draw(Shader* in_shader)
{
	// �e�N�X�`���̏c���T�C�Y�ɃX�P�[�����|�����킹���l���X�P�[���s��Ƃ��Ē�`
	Matrix4 scaleMat = Matrix4::CreateScale(
		static_cast<float>(m_width) * m_scale,
		static_cast<float>(m_height) * m_scale,
		1.0f);
	// �w�肵����ʈʒu�ւ̃X�N���[���ϊ��s����쐬
	Matrix4 transMat = Matrix4::CreateTranslation(
		Vector3(m_screenPos.x, m_screenPos.y, 0.0f));
	// ��]�s��
	Matrix4 rotZ = Matrix4::CreateRotationZ(Math::ToRadians(180.0f));
	Matrix4 rotY = Matrix4::CreateRotationY(Math::ToRadians(180.0f));
	// �X�P�[���ƕϊ��s������[���h�s��֕ϊ�
	Matrix4 world = scaleMat * rotZ * rotY * transMat;

	in_shader->SetActive();
	in_shader->SetMatrixUniform("u_worldTransform", world);
	//in_shader->SetMatrixUniform("u_viewProj", viewProj);
	in_shader->SetInt("u_texture", 0);
	in_shader->SetInt("u_mapTex", 1);
	in_shader->SetFloat("u_intensity", 0.1f);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_mapBuffer);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_mapTex->GetTextureID());


	RENDERER->SetActiveSpriteVAO();
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

// �~�j�}�b�v�p�̃t���[���o�b�t�@�쐬
void MiniMapHUD::CreateFBO(unsigned int& in_fbo)
{
	glGenFramebuffers(1, &in_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, in_fbo);

	// �}�b�v��ʕ`��p�o�b�t�@
	glGenTextures(1, &m_mapBuffer);
	glBindTexture(GL_TEXTURE_2D, m_mapBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_mapBuffer, 0);

	// �o�b�t�@���}�b�vFBO�̕`���Ƃ���GL���ɖ�������
	unsigned int attachment = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, &attachment);

	// �����_�[�o�b�t�@�̍쐬 (�X�e���V���o�b�t�@�Ƃ��Ē�`)
	glGenRenderbuffers(1, &m_rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_width, m_height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_rbo);

	// �t���[���o�b�t�@�̐��������`�F�b�N
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR::GBUFFER::Create False" << std::endl;
	}

	// �t���[���o�b�t�@�̃o�C���h����
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}
