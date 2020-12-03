#include "PointLight.h"
#include "PointLightComponent.h"
#include "MeshComponent.h"
#include "Mesh.h"

PointLight::PointLight()
	:m_lighting(true)
	,m_constant(0.01f)
	,m_linear(0.05f)
	,m_quadratic(0.01f)
	,m_diffuse(Vector3(0.3f, 0.6f, 1.0f))
	,m_ambient(m_diffuse * 0.2f)
	,m_specular(Vector3(1.0f, 1.0f, 1.0f))
	,m_lightComp(nullptr)
{
	CalcLightRadius();
	m_lightComp = new PointLightComponent(this);
	//m_meshComp = new MeshComponent(this);
	//m_meshComp->SetMesh(RENDERER->GetMesh("Data/Meshes/TND/Light/PointLight/PointLight_Sphere.OBJ"));
}

PointLight::~PointLight()
{
}

// ライトパラメータのセット
void PointLight::SetLightParameter(float in_constant, float in_linear, float in_quadratic)
{
	m_constant = in_constant;
	m_linear = in_linear;
	m_quadratic = in_quadratic;

	// ライトボリューム半径を再計算
	CalcLightRadius();
}

// ライトカラーのセット
void PointLight::SetLightColor(const Vector3& in_diffuse, const Vector3& in_specular)
{
	m_diffuse = in_diffuse;
	m_ambient = in_diffuse * 0.2f;
	m_specular = in_specular;
}


// 点灯状態の切り替え
void PointLight::SwitchLighting()
{
	m_lighting = !m_lighting;
}

// ライトボリューム半径の計算
void PointLight::CalcLightRadius()
{
	float max = std::fmax(std::fmax(m_diffuse.x, m_diffuse.y), m_diffuse.z);
	float radius;
	radius = (-m_linear + std::sqrtf((m_linear * m_linear) - 4.0f * m_quadratic * (m_constant - (256.0f / 4.0f) * max))) / (2.0f * m_quadratic);
	m_lightRadius = radius;
}
