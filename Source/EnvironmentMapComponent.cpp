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
{
	RENDERER->AddEnvironmentComponent(this);
}

EnvironmentMapComponent::~EnvironmentMapComponent()
{
	RENDERER->RemoveEnvironmentComponent(this);
}

// 環境マップオブジェクトの描画処理
void EnvironmentMapComponent::DrawEnvironmentMap(Shader* in_envShader)
{

	if (!m_isVisible)
	{
		// GLのアルファブレンド・αテストをON
		glEnable(GL_BLEND);
		glEnable(GL_ALPHA_TEST);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		in_envShader->SetMatrixUniform("u_worldTransform", m_owner->GetWorldTransform());
		in_envShader->SetFloat("u_luminance", 0.5f);
		// 頂点配列オブジェクトを取得し、バインド
		VertexArray* vao = m_mesh->GetVertexArray();
		vao->SetActive();
		// アクティブなキューブマップをテクスチャとしてバインド
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, RENDERER->GetSkyBox()->GetSkyBoxTexture()->GetTextureID());
		// 描画
		glDrawElements(GL_TRIANGLES, vao->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
		// アルファテストのオフ
		glDisable(GL_BLEND);
		glDisable(GL_ALPHA_TEST);

	}
}
