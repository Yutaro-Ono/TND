#include "SpotLight.h"
#include "SpotLightComponent.h"

SpotLight::SpotLight(const Vector3& in_pos, LIGHT_VOLUME in_vol)
	:m_lighting(true)
	,m_target(nullptr)
	, m_lightVolume(in_vol)
	, m_diffuse(Vector3(10.0f, 10.0f, 10.0f))
	, m_ambient(m_diffuse * 0.2f)
	, m_specular(Vector3(1.0f, 1.0f, 1.0f))
	, m_luminance(50.0f)
	, m_flash(false)
	, m_flashOffset(60.0f)
	, m_lightComp(nullptr)
{
	SetPosition(in_pos);
	m_direction = Vector3::NegUnitZ;

	m_lightComp = new SpotLightComponent(this);
}

SpotLight::~SpotLight()
{
}

void SpotLight::SetLightColor(const Vector3& in_diffuse, const Vector3& in_specular)
{
}

void SpotLight::UpdateActor(float in_deltaTime)
{
	if (m_target != nullptr)
	{
		//SetPosition(m_target->GetPosition() + m_target->GetForward() * Vector3(50.0f, 20.0f, 0.0f));
		//SetRotation(m_target->GetRotation());
		//SetRotation(Quaternion(m_target->GetRotation().x, rot.y, m_target->GetRotation().z, m_target->GetRotation().w));
		//m_worldTransform = Matrix4::CreateScale(m_scale) * Matrix4::CreateFromQuaternion(m_target->GetRotation()) * Matrix4::CreateTranslation(Vector3::Transform(Vector3(45.0f, 20.0f, 0.0f), m_target->GetRotation())) * Matrix4::CreateTranslation(m_target->GetPosition() + Vector3(0.0f, 0.0f, 30.0f));
		SetWorldTransform(Matrix4::CreateScale(m_scale) * Matrix4::CreateFromQuaternion(m_target->GetRotation()) * Matrix4::CreateTranslation(Vector3::Transform(Vector3(80.0f, 35.0f, 0.0f), m_target->GetRotation())) * Matrix4::CreateTranslation(m_target->GetPosition() + Vector3(0.0f, 0.0f, 35.0f)));
	}
}

void SpotLight::SwitchLighting()
{
}
