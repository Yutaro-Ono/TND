#include "LightGlassComponent.h"
#include "Mesh.h"
#include "Shader.h"
#include "VertexArray.h"
#include "Actor.h"
#include "Renderer.h"

LightGlassComponent::LightGlassComponent(Actor* in_owner)
	:Component(in_owner)
	,m_mesh(nullptr)
	,m_lightColor(Vector3(0.5f, 0.5f, 0.5f))
	,m_luminance(1.0f)
	,m_isVisible(false)
{
	RENDERER->AddLightGlassComponent(this);
}

LightGlassComponent::~LightGlassComponent()
{
	RENDERER->RemoveLightGlassComponent(this);
}

void LightGlassComponent::Draw(Shader* in_shader)
{

	if (!m_isVisible)
	{
		// GLのアルファブレンド・αテストをON
		glEnable(GL_BLEND);
		glEnable(GL_ALPHA_TEST);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// uniformへのセット
		in_shader->SetMatrixUniform("u_worldTransform", m_owner->GetWorldTransform());
		in_shader->SetVectorUniform("u_lightColor", m_lightColor);
		in_shader->SetFloat("u_luminance", m_luminance);

		// 頂点配列のバインド
		VertexArray* vao = m_mesh->GetVertexArray();
		vao->SetActive();

		// 描画
		glDrawElements(GL_TRIANGLES, vao->GetNumIndices(), GL_UNSIGNED_INT, nullptr);

		// アルファテストのオフ
		glDisable(GL_BLEND);
		glDisable(GL_ALPHA_TEST);
	}

}

