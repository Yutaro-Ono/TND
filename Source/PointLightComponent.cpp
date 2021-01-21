#include "PointLightComponent.h"
#include "GameMain.h"
#include "Renderer.h"
#include "Shader.h"
#include "Mesh.h"

PointLightComponent::PointLightComponent(PointLight* in_light)
	:Component(in_light)
	,m_light(in_light)
	,m_mesh(nullptr)
	,m_constant(1.0f)
{
	// レンダラーにこのポイントライトを追加
	RENDERER->AddPointLightComponent(this);

	// ライトパラメータのセット
	SetLightParam(m_light->GetLightVolume());

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
	in_shader->SetFloat("u_pl.constant", m_constant);
	in_shader->SetFloat("u_pl.linear", m_linear);
	in_shader->SetFloat("u_pl.quadratic", m_quadratic);
	// カラーのセット
	in_shader->SetVectorUniform("u_pl.diffuse", m_light->GetDiffuseColor());
	in_shader->SetVectorUniform("u_pl.ambient", m_light->GetAmbientColor());
	in_shader->SetVectorUniform("u_pl.specular", m_light->GetSpecularColor());
	in_shader->SetFloat("u_pl.luminance", m_light->GetLuminance());

	// 座標のセット
	in_shader->SetMatrixUniform("u_worldTransform", m_light->GetWorldTransform());

	// 頂点配列をアクティブに
	VertexArray* va = m_mesh->GetVertexArray();
	va->SetActive();
	// 描画する
	glDrawElements(GL_TRIANGLES, va->GetNumIndices(), GL_UNSIGNED_INT, 0);
}

/// <summary>
/// ライトパラメータのセット・ボリュームタイプに応じた値をセット
/// </summary>
/// <param name="in_vol"> ポイントライトアクタのボリュームタイプ </param>
void PointLightComponent::SetLightParam(PointLight::LIGHT_VOLUME in_vol)
{
	// 小サイズ
	if (in_vol == PointLight::LIGHT_VOLUME::VL_SMALL)
	{
		m_radius = 325.0f;
		m_linear = 0.014;
		m_quadratic = 0.0007;
	}
	else if (in_vol == PointLight::LIGHT_VOLUME::VL_VERY_SMALL)
	{
		m_radius = 50.0f;
		m_linear = 0.09;
		m_quadratic = 0.032;
	}
	// 中サイズ
	else if (in_vol == PointLight::LIGHT_VOLUME::VL_MEDIUM)
	{
		m_radius = 600.0f;
		m_linear = 0.007;
		m_quadratic = 0.0002;
	}
	// 大サイズ
	else if (in_vol == PointLight::LIGHT_VOLUME::VL_BIG)
	{
		m_radius = 3250.0f;
		m_linear = 0.0014;
		m_quadratic = 0.000007;
	}

	// ライトにスケールとして半径サイズを適用する
	m_light->SetScale(m_radius);
}