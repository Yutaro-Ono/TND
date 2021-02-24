#include "EnvironmentMapComponent.h"
#include "Renderer.h"
#include "GameMain.h"
#include "Actor.h"
#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"
#include "VertexArray.h"
#include "CubeMapComponent.h"

EnvironmentMapComponent::EnvironmentMapComponent(Actor* in_owner)
	:Component(in_owner)
	,m_isVisible(false)
	,m_luminance(0.5f)
	,m_alpha(1.0f)
{
	RENDERER->AddEnvironmentComponent(this);
}

EnvironmentMapComponent::~EnvironmentMapComponent()
{
	RENDERER->RemoveEnvironmentComponent(this);
}

// ���}�b�v�I�u�W�F�N�g�̕`�揈��
void EnvironmentMapComponent::DrawEnvironmentMap(Shader* in_envShader)
{

	if (!m_isVisible)
	{
		// GL�̃A���t�@�u�����h�E���e�X�g��ON
		glEnable(GL_BLEND);
		glEnable(GL_ALPHA_TEST);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


		in_envShader->SetMatrixUniform("u_worldTransform", m_owner->GetWorldTransform());
		in_envShader->SetFloat("u_luminance", m_luminance);
		in_envShader->SetFloat("u_alpha", m_alpha);
		// ���_�z��I�u�W�F�N�g���擾���A�o�C���h
		VertexArray* vao = m_mesh->GetVertexArray();
		vao->SetActive();
		// �A�N�e�B�u�ȃL���[�u�}�b�v���e�N�X�`���Ƃ��ăo�C���h
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, RENDERER->GetSkyBox()->GetSkyBoxTexture()->GetTextureID());
		// �`��
		glDrawElements(GL_TRIANGLES, vao->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
		// �A���t�@�e�X�g�̃I�t
		glDisable(GL_BLEND);
		glDisable(GL_ALPHA_TEST);


	}
}
