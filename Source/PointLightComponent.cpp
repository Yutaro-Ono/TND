#include "PointLightComponent.h"
#include "PointLight.h"
#include "GameMain.h"
#include "Renderer.h"
#include "Shader.h"
#include "Mesh.h"

PointLightComponent::PointLightComponent(PointLight* in_light)
	:Component(in_light)
	,m_light(in_light)
	,m_mesh(nullptr)
{
	// レンダラーにこのポイントライトを追加
	RENDERER->AddPointLightComponent(this);
	// メッシュのロード
	m_mesh = RENDERER->GetMesh("Data/Meshes/TND/Light/PointLight/PointLight_Sphere.OBJ");
}

PointLightComponent::~PointLightComponent()
{
	RENDERER->RemovePointLightComponent(this);
}

// ライトの描画
void PointLightComponent::Draw(Shader* in_shader)
{
	// ポイントライトパラメータをセット
	in_shader->SetVectorUniform("u_pl.position", m_light->GetPosition());
	in_shader->SetFloat("u_pl.constant", m_light->GetConstant());
	in_shader->SetFloat("u_pl.linear", m_light->GetLinear());
	in_shader->SetFloat("u_pl.quadratic", m_light->GetQuadratic());
	// カラーのセット
	in_shader->SetVectorUniform("u_pl.diffuse", m_light->GetDiffuseColor());
	in_shader->SetVectorUniform("u_pl.ambient", m_light->GetAmbientColor());
	in_shader->SetVectorUniform("u_pl.specular", m_light->GetSpecularColor());

	Matrix4 mat = Matrix4::CreateScale(m_light->GetLightRadius());
	mat = mat * m_light->GetWorldTransform();
	// 座標のセット
	in_shader->SetMatrixUniform("u_worldTransform", mat);

	// 頂点配列をアクティブに
	VertexArray* va = m_mesh->GetVertexArray();
	va->SetActive();
	// 描画する
	glDrawElements(GL_TRIANGLES, va->GetNumIndices(), GL_UNSIGNED_INT, 0);
}
