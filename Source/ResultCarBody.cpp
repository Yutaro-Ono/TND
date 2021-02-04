#include "ResultCarBody.h"
#include "PhysicsWorld.h"
#include "BoxCollider.h"
#include "Collision.h"
#include "EnvironmentMapComponent.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "CarMeshComponent.h"
#include "LightGlassComponent.h"
// メッシュパス
const std::string ResultCarBody::CAR_BODY_MESH_PATH = "Data/Meshes/TND/Actors/Car/Player/Body/OnlyFrame/BodyOnlyFrameLessMirror_Internal.OBJ";
const std::string ResultCarBody::CAR_GLASS_MESH_PATH = "Data/Meshes/TND/Actors/Car/Player/Body/Glass/Glass/BodyGlass.OBJ";
const std::string ResultCarBody::CAR_FRONTLIGHT_MESH_PATH = "Data/Meshes/TND/Actors/Car/Player/Body/Glass/FrontLight/FrontLight_Internal.OBJ";
const std::string ResultCarBody::CAR_BACKLIGHT_MESH_PATH = "Data/Meshes/TND/Actors/Car/Player/Body/Glass/BackLight/BackLight_Internal.OBJ";
const std::string ResultCarBody::CAR_INTERIOR_MESH_PATH = "Data/Meshes/TND/Actors/Car/Player/Body/Interior/InteriorGlassLess_Internal.OBJ";

// コンストラクタ
ResultCarBody::ResultCarBody(Actor* in_owner)
	:m_owner(in_owner)
{
	// オーナーの拡大率・座標・回転行列に合わせておく
	m_scale = m_owner->GetScale();
	m_position = m_owner->GetPosition();
	m_rotation = m_owner->GetRotation();
	// メッシュのセット
	// ボディフレーム・ガラス・内装
	Mesh* bodyFrameMesh = RENDERER->GetMesh(CAR_BODY_MESH_PATH);
	CarMeshComponent* bodyFrame = new CarMeshComponent(this);
	//MeshComponent* bodyFrame = new MeshComponent(this);
	bodyFrame->SetMesh(bodyFrameMesh);
	// 窓ガラス (環境マッピング)
	Mesh* glassMesh = RENDERER->GetMesh(CAR_GLASS_MESH_PATH);
	//MeshComponent* glass = new MeshComponent(this);
	EnvironmentMapComponent* glass = new EnvironmentMapComponent(this);
	glass->SetMesh(glassMesh);
	glass->SetLuminance(0.04f);

	// フロントライト
	glassMesh = RENDERER->GetMesh(CAR_FRONTLIGHT_MESH_PATH);
	LightGlassComponent* lightGlass = new LightGlassComponent(this);
	lightGlass->SetMesh(glassMesh);
	lightGlass->SetLightColor(Vector3(1.0f, 0.6f, 0.4f));
	lightGlass->SetLightLuminance(0.5f);
	// バックライト
	glassMesh = RENDERER->GetMesh(CAR_BACKLIGHT_MESH_PATH);
	lightGlass = new LightGlassComponent(this);
	lightGlass->SetMesh(glassMesh);
	lightGlass->SetLightColor(Vector3(1.0f, 0.01f, 0.35f));
	lightGlass->SetLightLuminance(0.5f);

	// 内装メッシュ
	Mesh* interiorMesh = RENDERER->GetMesh(CAR_INTERIOR_MESH_PATH);
	MeshComponent* interior = new MeshComponent(this);
	interior->SetMesh(interiorMesh);
	interior->SetMapColor(Vector3(0.45f, 0.8f, 0.75f));

}

// デストラクタ
ResultCarBody::~ResultCarBody()
{
}

void ResultCarBody::UpdateActor(float in_deltaTime)
{
	// オーナーの拡大率・座標・回転行列に合わせておく
	m_scale = m_owner->GetScale();
	m_position = m_owner->GetPosition();
	m_rotation = m_owner->GetRotation();

	// オーナーに合わせるためワールド座標を取得し続ける
	m_worldTransform = m_owner->GetWorldTransform();

}
