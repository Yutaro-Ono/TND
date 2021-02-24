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
#include "ShadowMap.h"
#include "Texture.h"
#include "VertexArray.h"
#include <typeinfo>

// �R���X�g���N�^
MeshComponent::MeshComponent(Actor * in_owner, bool in_isSkeletal)
	:Component(in_owner)
	,m_mesh(nullptr)
	,m_textureIndex(0)
	,m_visible(true)
	,m_isDrawMap(true)
	,m_isSkeletal(in_isSkeletal)
	,m_intensity(1.0f)
	,m_mapColor(Vector3(0.5f, 0.5f, 0.5f))
{
	RENDERER->AddMeshComponent(this);
	//printf("new MeshComponent : [%5d] owner->( 0x%p )\n", GetID(), in_owner);
}

// �f�X�g���N�^
MeshComponent::~MeshComponent()
{
	//printf("Remove MeshComponent : [%5d] owner->( 0x%p )\n)", GetID(), m_owner);
	RENDERER->RemoveMeshComponent(this);
}

// �ʏ�`�揈��
void MeshComponent::Draw(Shader * in_shader)
{
	if (m_mesh != nullptr && m_visible)
	{
		// ���[���h�ϊ����Z�b�g
		in_shader->SetMatrixUniform("u_worldTransform", m_owner->GetWorldTransform());
		// �X�y�L�������x�Z�b�g
		in_shader->SetFloatUniform("u_specPower", 32);
		// �P�x���x�Z�b�g
		in_shader->SetFloatUniform("u_intensity", m_intensity);

		SetTexturesToUniform(in_shader);          // Uniform�Ɋe��e�N�X�`�����Z�b�g

		// ���_�z����A�N�e�B�u��
		VertexArray* va = m_mesh->GetVertexArray();
		va->SetActive();
		// �`�悷��
		glDrawElements(GL_TRIANGLES, va->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
	}
}

// �V���h�E�����p�̕`��
void MeshComponent::DrawShadow(Shader* in_shader)
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

void MeshComponent::DrawMap(Shader* in_shader)
{
	if (m_mesh != nullptr && m_isDrawMap)
	{
		// ���[���h�ϊ����Z�b�g
		in_shader->SetMatrixUniform("u_worldTransform", m_owner->GetWorldTransform());
		// �X�y�L�������x�Z�b�g
		in_shader->SetVectorUniform("u_mapColor", m_mapColor);

		// ���_�z����A�N�e�B�u��
		VertexArray* va = m_mesh->GetVertexArray();
		va->SetActive();
		// �`�悷��
		glDrawElements(GL_TRIANGLES, va->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
	}
}

/// <summary>
/// �V�F�[�_uniform�Ɋe��e�N�X�`�����Z�b�g����֐�
/// </summary>
/// <param name="in_shader"> �V�F�[�_�N���X�̃|�C���^ </param>
void MeshComponent::SetTexturesToUniform(Shader* in_shader)
{
	// �f�B�t���[�Y �� �X�y�L���� �� �m�[�}�� �� �G�~�b�V�u �� �V���h�E �̏��ŃZ�b�g
	// �w��^�C�v�̃e�N�X�`����Mesh���ێ����Ă��Ȃ������ꍇ�A�������� "0"���Z�b�g
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_mesh->GetTextureID(TEXTURE_TYPE::DIFFUSE_MAP));
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_mesh->GetTextureID(TEXTURE_TYPE::SPECULAR_MAP));
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_mesh->GetTextureID(TEXTURE_TYPE::NORMAL_MAP));
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, m_mesh->GetTextureID(TEXTURE_TYPE::EMISSIVE_MAP));
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, RENDERER->GetShadowMap()->GetDepthMap());
}

