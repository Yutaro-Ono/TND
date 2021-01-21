#include "CubeMapComponent.h"
#include "GameMain.h"
#include "Renderer.h"
#include "Texture.h"
#include "Shader.h"
#include "Math.h"
#include "SkyBox.h"
#include "VertexArray.h"
#include <vector>


CubeMapComponent::CubeMapComponent(Actor* in_owner)
	:Component(in_owner)
	,m_texture(nullptr)
	,m_luminance(1.0f)
	,m_isVisible(false)
{
}

CubeMapComponent::~CubeMapComponent()
{
	delete m_texture;
}

// �e�N�X�`���̐���
void CubeMapComponent::CreateTexture(const std::string& in_filePath)
{
	m_texture = new Texture();
	m_texture->LoadCubeMap(in_filePath);
}

// �L���[�u�}�b�v�̕`�揈��
void CubeMapComponent::Draw(Shader* in_shader)
{
	// �����ɂ��Ă��Ȃ����
	if (m_isVisible)
	{
		// �[�x�ݒ�
		glDepthFunc(GL_LEQUAL);
		// �P�x�����V�F�[�_�ɓn��
		in_shader->SetFloat("u_skyLuminance", m_luminance);
		// �e�N�X�`���o�C���h
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture->GetTextureID());
		// �L���[�u�}�b�v�p���_�z��̃A�N�e�B�u��
		RENDERER->GetCubeMapVerts()->SetActive();
		// �`��
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// �O�̂��߃o�C���h����
		glBindVertexArray(0);

		glDepthFunc(GL_LESS);
	}

}

