//-----------------------------------------------------------------------+
// ���b�V���R���|�[�l���g�N���X.
// 
// 
// copyright (C) 2019 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#include "MeshComponent.h"
#include "Shader.h"
#include "Mesh.h"
#include "Actor.h"
#include "GameMain.h"
#include "Renderer.h"
#include "Texture.h"
#include "VertexArray.h"
#include <typeinfo>

// �R���X�g���N�^
MeshComponent::MeshComponent(Actor * in_owner, bool in_isSkeletal)
	:Component(in_owner)
	,m_mesh(nullptr)
	,m_textureIndex(0)
	,m_visible(true)
	,m_isSkeletal(in_isSkeletal)
{
	GAME_INSTANCE.GetRenderer()->AddMeshComponent(this);
	printf("new MeshComponent : [%5d] owner->( 0x%p )\n", GetID(), in_owner);
}

// �f�X�g���N�^
MeshComponent::~MeshComponent()
{
	printf("Remove MeshComponent : [%5d] owner->( 0x%p )\n)", GetID(), m_owner);
	GAME_INSTANCE.GetRenderer()->RemoveMeshComponent(this);
}

// �`�揈��
void MeshComponent::Draw(Shader * in_shader)
{
	if (m_mesh != nullptr && m_visible)
	{
		// Set the world transform�@���[���h�ϊ����Z�b�g
		in_shader->SetMatrixUniform("uWorldTransform",
			m_owner->GetWorldTransform());
		// Set specular power�@�X�y�L�������x�Z�b�g
		in_shader->SetFloatUniform("uSpecPower", 100);

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
			glBindTexture(GL_TEXTURE_2D, m_mesh->GetDepthMap()->GetTextureID());
		}

		

		// Set the mesh's vertex array as active�@���_�z����A�N�e�B�u��
		VertexArray* va = m_mesh->GetVertexArray();
		va->SetActive();
		// Draw�@�`�悷��
		glDrawElements(GL_TRIANGLES, va->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
	}
}

// �V���h�E�����p�̕`��
void MeshComponent::DrawShadow(Shader* in_shader)
{
	if (m_mesh != nullptr && m_visible)
	{
		// Set the world transform�@���[���h�ϊ����Z�b�g
		in_shader->SetMatrixUniform("uWorldTransform",
			m_owner->GetWorldTransform());
		// Set specular power�@�X�y�L�������x�Z�b�g
		in_shader->SetFloatUniform("uSpecPower", 100);

		// Set the mesh's vertex array as active�@���_�z����A�N�e�B�u��
		VertexArray* va = m_mesh->GetVertexArray();
		va->SetActive();
		// Draw�@�`�悷��
		glDrawElements(GL_TRIANGLES, va->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
	}
}

