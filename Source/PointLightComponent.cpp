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
	in_shader->SetFloat("u_pointLight.constant", m_light->GetConstant());
	in_shader->SetFloat("u_pointLight.linear", m_light->GetLinear());
	in_shader->SetFloat("u_pointLight.quadratic", m_light->GetQuadratic());
	// カラーのセット
	in_shader->SetVectorUniform("u_pointLight.diffuse", m_light->GetDiffuseColor());
	in_shader->SetVectorUniform("u_pointLight.ambient", m_light->GetAmbientColor());
	in_shader->SetVectorUniform("u_pointLight.specular", m_light->GetSpecularColor());

	// ライトボリューム半径から行列を更新
	Matrix4 lightVolume = Matrix4::CreateScale(m_light->GetLightRadius() * m_light->GetScale());
	lightVolume = lightVolume * Matrix4::CreateTranslation(m_light->GetPosition());
	// 座標のセット
	in_shader->SetVectorUniform("u_pointLight.position", m_light->GetPosition());
	in_shader->SetMatrixUniform("u_worldTransform", lightVolume);

	// 頂点配列をアクティブに
	VertexArray* va = m_mesh->GetVertexArray();
	va->SetActive();
	// 描画する
	glDrawElements(GL_TRIANGLES, va->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
}
