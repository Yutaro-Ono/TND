#include "CarMeshComponent.h"
#include "GameMain.h"
#include "Renderer.h"
#include "Actor.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "ShadowMap.h"

CarMeshComponent::CarMeshComponent(Actor* in_owner)
	:Component(in_owner)
	, m_mesh(nullptr)
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
		in_shader->SetFloatUniform("u_specPower", 32);

		// �e��e�N�X�`�����V�F�[�_�ɃZ�b�g����
		// �e�N�X�`�����ǂݍ��܂�Ă��Ȃ��ꍇ�͖�������
		if (m_mesh->GetDiffuseMap() != nullptr)
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_mesh->GetDiffuseMap()->GetTextureID());

		}
		if (m_mesh->GetSpecularMap() != nullptr)
		{
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, m_mesh->GetSpecularMap()->GetTextureID());

		}
		if (m_mesh->GetNormalMap() != nullptr)
		{
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, m_mesh->GetNormalMap()->GetTextureID());

		}
		if (m_mesh->GetDepthMap() != nullptr)
		{
			glActiveTexture(GL_TEXTURE3);
			glBindTexture(GL_TEXTURE_2D, RENDERER->GetShadowMap()->GetDepthMap());
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
