#include "CarMeshComponent.h"
#include "GameMain.h"
#include "Renderer.h"
#include "Actor.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "ShadowMap.h"
#include "SkyBox.h"

CarMeshComponent::CarMeshComponent(Actor* in_owner)
	:Component(in_owner)
	, m_mesh(nullptr)
	, m_reflect(true)
	, m_textureIndex(0)
	, m_visible(true)
{
	RENDERER->AddCarMeshComponent(this);
}

CarMeshComponent::~CarMeshComponent()
{
	RENDERER->RemoveCarMeshComponent(this);
}

void CarMeshComponent::Draw(Shader* in_shader)
{
	if (m_mesh != nullptr && m_visible)
	{
		// ���[���h�ϊ����Z�b�g
		in_shader->SetMatrixUniform("u_worldTransform", m_owner->GetWorldTransform());
		// �X�y�L�������x�Z�b�g
		in_shader->SetFloatUniform("u_specPower", 32.0f);

		// �e��e�N�X�`�����V�F�[�_�ɃZ�b�g����
		// �e�N�X�`�����ǂݍ��܂�Ă��Ȃ��ꍇ�͖�������
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_mesh->GetTextureID(TEXTURE_TYPE::DIFFUSE_MAP));
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_mesh->GetTextureID(TEXTURE_TYPE::SPECULAR_MAP));
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, RENDERER->GetShadowMap()->GetDepthMap());

		// ���˗L�����A���}�b�v�g�p
		if (m_reflect)
		{
			in_shader->SetInt("u_skybox", 3);
			glActiveTexture(GL_TEXTURE3);
			glBindTexture(GL_TEXTURE_CUBE_MAP, RENDERER->GetSkyBox()->GetSkyBoxTexture()->GetTextureID());;
		}

		// ���_�z����A�N�e�B�u��
		VertexArray* va = m_mesh->GetVertexArray();
		va->SetActive();
		// �`�悷��
		glDrawElements(GL_TRIANGLES, va->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
	}
}

void CarMeshComponent::DrawShadow(Shader* in_shader)
{
	if (m_mesh != nullptr && m_visible)
	{
		// ���[���h�ϊ����Z�b�g
		in_shader->SetMatrixUniform("u_worldTransform", m_owner->GetWorldTransform());

		// ���_�z����A�N�e�B�u��
		VertexArray* va = m_mesh->GetVertexArray();
		va->SetActive();
		// �`�悷��
		glDrawElements(GL_TRIANGLES, va->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
	}
}
