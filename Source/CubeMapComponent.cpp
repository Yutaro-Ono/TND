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
	m_texture->LoadSkyBox(in_filePath);
}

// �L���[�u�}�b�v�̕`�揈��
void CubeMapComponent::Draw(Shader* in_shader)
{
	// �����ɂ��Ă��Ȃ����
	if (m_isVisible)
	{
		// �r���[�s��A�v���W�F�N�V�����s����擾
		Matrix4 view, projection;
		view = RENDERER->GetViewMatrix();
		projection = RENDERER->GetProjectionMatrix();
		view.Invert();

		// Uniform�ɍs����Z�b�g
		in_shader->SetActive();
		in_shader->SetMatrixUniform("u_View", view);
		in_shader->SetMatrixUniform("u_Proj", projection);
		// �[�x�ݒ�
		glDepthFunc(GL_LEQUAL);
		// �e�N�X�`���o�C���h
		in_shader->SetInt("skybox", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture->GetTextureID());

		// �`��
		RENDERER->GetCubeMapVerts()->SetActive();
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// �O�̂��߃o�C���h����
		glBindVertexArray(0);

		glDepthFunc(GL_LESS);
	}

}

