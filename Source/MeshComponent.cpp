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
	GAME_INSTANCE.GetRenderer()->AddMeshComponent(this);
	printf("new MeshComponent : [%5d] owner->( 0x%p )\n", GetID(), in_owner);
}

// デストラクタ
MeshComponent::~MeshComponent()
{
	printf("Remove MeshComponent : [%5d] owner->( 0x%p )\n)", GetID(), m_owner);
	GAME_INSTANCE.GetRenderer()->RemoveMeshComponent(this);
}

// 描画処理
void MeshComponent::Draw(Shader * in_shader)
{
	if (m_mesh)
	{
		// Set the world transform　ワールド変換をセット
		in_shader->SetMatrixUniform("uWorldTransform",
			m_owner->GetWorldTransform());
		// Set specular power　スペキュラ強度セット
		in_shader->SetFloatUniform("uSpecPower", 100);
		// Set the active texture　アクティブテクスチャセット
		Texture* t = m_mesh->GetTexture(m_textureIndex);
		if (t)
		{
			t->SetActive();
		}

		// 法線マップ情報などが格納されている場合は
		if (m_mesh->GetTextureNum() >= 2)
		{
			in_shader->SetInt("u_mat.diffuseMap", 0);
			in_shader->SetInt("u_mat.normalMap", 1);
			// テクスチャのアクティブ化
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_mesh->GetTexture(0)->GetTextureID());
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, m_mesh->GetTexture(1)->GetTextureID());
		}

		// Set the mesh's vertex array as active　頂点配列をアクティブに
		VertexArray* va = m_mesh->GetVertexArray();
		va->SetActive();
		// Draw　描画する
		glDrawElements(GL_TRIANGLES, va->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
	}
}

