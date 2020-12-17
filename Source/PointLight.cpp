#include "PointLight.h"
#include "PointLightComponent.h"
#include "MeshComponent.h"
#include "Mesh.h"

PointLight::PointLight(LIGHT_VOLUME in_vol)
	:m_lighting(true)
	,m_lightVolume(in_vol)
	,m_diffuse(Vector3(0.3f, 0.68f, 1.0f))
	,m_ambient(m_diffuse * 0.2f)
	,m_specular(Vector3(1.0f, 1.0f, 1.0f))
	,m_luminance(1.0f)
	,m_flash(false)
	,m_flashOffset(60.0f)
	,m_lightComp(nullptr)
{
	m_diffuse = Vector3(1.0f, 0.65f, 0.95f);
	m_ambient = m_diffuse * 0.2;
	m_specular = m_diffuse;
	m_luminance = 12.0f;
	m_lightComp = new PointLightComponent(this);
}

PointLight::~PointLight()
{
}

// ライトカラーのセット
void PointLight::SetLightColor(const Vector3& in_diffuse, const Vector3& in_specular)
{
	m_diffuse = in_diffuse;
	m_ambient = in_diffuse * 0.2f;
	m_specular = in_specular;
}

void PointLight::UpdateActor(float in_deltaTime)
{
	// 点滅処理
	if (m_flash)
	{
		static float anim = 0.01f;
		anim += 0.5f * in_deltaTime;
		m_luminance = 60.0f * Math::Sin(anim);
		if (m_luminance < 0.0f)
		{
			m_luminance *= -1.0f;
		}
	}

}


// 点灯状態の切り替え
void PointLight::SwitchLighting()
{
	m_lighting = !m_lighting;
}
