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
	if (m_mesh)
	{
		// Set the world transform�@���[���h�ϊ����Z�b�g
		in_shader->SetMatrixUniform("uWorldTransform",
			m_owner->GetWorldTransform());
		// Set specular power�@�X�y�L�������x�Z�b�g
		in_shader->SetFloatUniform("uSpecPower", 100);
		// Set the active texture�@�A�N�e�B�u�e�N�X�`���Z�b�g
		Texture* t = m_mesh->GetTexture(m_textureIndex);
		if (t)
		{
			t->SetActive();
		}

		// �@���}�b�v���Ȃǂ��i�[����Ă���ꍇ��
		if (m_mesh->GetTextureNum() >= 2)
		{
			in_shader->SetInt("u_mat.diffuseMap", 0);
			in_shader->SetInt("u_mat.normalMap", 1);
			// �e�N�X�`���̃A�N�e�B�u��
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_mesh->GetTexture(0)->GetTextureID());
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, m_mesh->GetTexture(1)->GetTextureID());
		}

		// Set the mesh's vertex array as active�@���_�z����A�N�e�B�u��
		VertexArray* va = m_mesh->GetVertexArray();
		va->SetActive();
		// Draw�@�`�悷��
		glDrawElements(GL_TRIANGLES, va->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
	}
}

