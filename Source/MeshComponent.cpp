//-----------------------------------------------------------------------+
// メッシュコンポーネントクラス.
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

// コンストラクタ
MeshComponent::MeshComponent(Actor * in_owner, bool in_isSkeletal)
	:Component(in_owner)
	,m_mesh(nullptr)
	,m_textureIndex(0)
	,m_visible(true)
	,m_isSkeletal(in_isSkeletal)
{
	RENDERER->AddMeshComponent(this);
	//printf("new MeshComponent : [%5d] owner->( 0x%p )\n", GetID(), in_owner);
}

// デストラクタ
MeshComponent::~MeshComponent()
{
	//printf("Remove MeshComponent : [%5d] owner->( 0x%p )\n)", GetID(), m_owner);
	RENDERER->RemoveMeshComponent(this);
}

// 描画処理
void MeshComponent::Draw(Shader * in_shader)
{
	if (m_mesh != nullptr && m_visible)
	{
		// ワールド変換をセット
		in_shader->SetMatrixUniform("u_worldTransform", m_owner->GetWorldTransform());
		// スペキュラ強度セット
		in_shader->SetFloatUniform("u_specPower", 32);

		// 各種テクスチャをシェーダにセットする
		// テクスチャが読み込まれていない場合は無視する
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

		// 頂点配列をアクティブに
		VertexArray* va = m_mesh->GetVertexArray();
		va->SetActive();
		// 描画する
		glDrawElements(GL_TRIANGLES, va->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
	}
}

// シャドウ生成用の描画
void MeshComponent::DrawShadow(Shader* in_shader)
{
	if (m_mesh != nullptr && m_visible)
	{
		// ワールド変換をセット
		in_shader->SetMatrixUniform("u_worldTransform", m_owner->GetWorldTransform());

		// 頂点配列をアクティブに
		VertexArray* va = m_mesh->GetVertexArray();
		va->SetActive();
		// 描画する
		glDrawElements(GL_TRIANGLES, va->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
	}
}

