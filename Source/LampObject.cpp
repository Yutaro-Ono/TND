#include "LampObject.h"
#include "GameMain.h"
#include "Renderer.h"
#include "Mesh.h"
#include "MeshComponent.h"
#include "PointLight.h"

LampObject::LampObject(const Vector3& in_pos)
	:m_headLight(nullptr)
	,m_groundLight(nullptr)
{
	SetPosition(in_pos);
	SetScale(0.8f);

	// メッシュを取得し、コンポーネントにセット
	m_meshComp = new MeshComponent(this);
	m_meshComp->SetMesh(RENDERER->GetMesh("Data/Meshes/TND/Objects/Lamp/SM_Lamp.OBJ"));
	m_meshComp->SetIntensityVal(4.5f);

	// 電球の光源
	m_headLight = new PointLight(PointLight::LIGHT_VOLUME::VL_BIG);
	m_headLight->SetPosition(m_position + Vector3(0.0f, 0.0f, -50.0f));
	m_headLight->SetLightColor(Vector3(0.8f, 1.0f, 1.0f), Vector3(1.0f, 1.0f, 1.0f));

	// 地面の光源
	m_groundLight = new PointLight(PointLight::LIGHT_VOLUME::VL_BIG);
	m_groundLight->SetPosition(m_position + Vector3(0.0f, 0.0f, 90.0f));
	m_groundLight->SetLightColor(Vector3(0.31f, 0.55f, 0.5f), Vector3(0.31f, 0.55f, 0.5f));
	m_groundLight->SetLuminance(3.0f);
}

LampObject::LampObject(const Vector3& in_pos, class Mesh* in_mesh)
	:m_headLight(nullptr)
	,m_groundLight(nullptr)
{
	SetPosition(in_pos);

	// メッシュを取得し、コンポーネントにセット
	m_meshComp = new MeshComponent(this);
	m_meshComp->SetMesh(in_mesh);
	m_meshComp->SetIntensityVal(3.5f);

	// 電球の光源
	m_headLight = new PointLight(PointLight::LIGHT_VOLUME::VL_BIG);
	m_headLight->SetPosition(m_position + Vector3(0.0f, 0.0f, -50.0f));
	m_headLight->SetLightColor(Vector3(0.2f, 0.5f, 0.7f), Vector3(0.2f, 0.5f, 0.7f));
	m_headLight->SetLuminance(2.25f);

	// 地面の光源
	m_groundLight = new PointLight(PointLight::LIGHT_VOLUME::VL_BIG);
	m_groundLight->SetPosition(m_position + Vector3(0.0f, 0.0f, 90.0f));
	m_groundLight->SetLightColor(Vector3(0.2f, 0.5f, 0.7f), Vector3(0.2f, 0.5f, 0.7f));
	m_groundLight->SetLuminance(2.25f);
}

LampObject::~LampObject()
{
}
