#include "CarBody.h"
#include "PlayerCar.h"
#include "PhysicsWorld.h"
#include "BoxCollider.h"
#include "Collision.h"
#include "EnvironmentMapComponent.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "CarMeshComponent.h"
#include "LightGlassComponent.h"
// メッシュパス
const std::string CarBody::CAR_BODY_MESH_PATH = "Data/Meshes/TND/Actors/Car/Player/Body/OnlyFrame/BodyOnlyFrameLessMirror_Internal.OBJ";
const std::string CarBody::CAR_GLASS_MESH_PATH = "Data/Meshes/TND/Actors/Car/Player/Body/Glass/Glass/BodyGlass.OBJ";
const std::string CarBody::CAR_FRONTLIGHT_MESH_PATH = "Data/Meshes/TND/Actors/Car/Player/Body/Glass/FrontLight/FrontLight_Internal.OBJ";
const std::string CarBody::CAR_BACKLIGHT_MESH_PATH = "Data/Meshes/TND/Actors/Car/Player/Body/Glass/BackLight/BackLight_Internal.OBJ";
const std::string CarBody::CAR_INTERIOR_MESH_PATH = "Data/Meshes/TND/Actors/Car/Player/Body/Interior/InteriorGlassLess_Internal.OBJ";

// コンストラクタ
CarBody::CarBody(PlayerCar* in_owner)
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


	// フロントライト
	glassMesh = RENDERER->GetMesh(CAR_FRONTLIGHT_MESH_PATH);
	LightGlassComponent* lightGlass = new LightGlassComponent(this);
	lightGlass->SetMesh(glassMesh);
	lightGlass->SetLightColor(Vector3(1.0f, 0.6f, 0.4f));
	lightGlass->SetLightLuminance(2.47f);
	// バックライト
	glassMesh = RENDERER->GetMesh(CAR_BACKLIGHT_MESH_PATH);
	lightGlass = new LightGlassComponent(this);
	lightGlass->SetMesh(glassMesh);
	lightGlass->SetLightColor(Vector3(1.0f, 0.01f, 0.35f));
	lightGlass->SetLightLuminance(3.21f);

	// 内装メッシュ
	Mesh* interiorMesh = RENDERER->GetMesh(CAR_INTERIOR_MESH_PATH);
	MeshComponent* interior = new MeshComponent(this);
	interior->SetMesh(interiorMesh);
	interior->SetDrawMap(false);
	interior->SetMapColor(Vector3(0.45f, 0.8f, 0.75f));

	// ボディのメッシュを基準としてオーナーの当たり判定ボックスをセット
	// 当たり判定ボックスのセット
	AABB playerBox = bodyFrameMesh->GetCollisionBox();
	m_owner->SetBoxCollider(new BoxCollider(in_owner, PhysicsWorld::TYPE_PLAYER_CAR));
	//playerBox.m_min.x *= 0.6f;
	//playerBox.m_min.y *= 0.6f;
	//playerBox.m_max.x *= 0.6f;
	//playerBox.m_max.y *= 0.6f;
	playerBox.m_min.z *= 0.05f;

	playerBox.m_max.z *= 1.0f;
	m_owner->GetBoxCollider()->SetObjectBox(playerBox);
	m_owner->GetBoxCollider()->SetArrowRotate(true);


	//// フロントライト・バックライトのセット
	//for (int i = 0; i < 2; i++)
	//{
	//	m_frontLight[i] = new PointLight(PointLight::VL_MEDIUM);
	//	m_frontLight[i]->SetPosition(Vector3(m_owner->GetPosition().x - 30.0f, m_owner->GetPosition().y + (10.0f * (i + 1)), m_owner->GetPosition().z));
	//	m_frontLight[i]->SetLightColor(Vector3(1.0f, 0.6f, 0.4f), Vector3(1.0f, 0.6f, 0.4f));
	//	m_frontLight[i]->SetLuminance(2.5f);

	//	m_backLight[i] = new PointLight(PointLight::VL_MEDIUM);
	//	m_backLight[i]->SetLightColor(Vector3(1.0f, 0.0f, 0.4f), Vector3(1.0f, 0.0f, 0.4f));
	//	m_backLight[i]->SetLuminance(3.0f);
	//}

	//m_frontLight[0]->SetPosition(Vector3(m_owner->GetPosition().x + 100.0f, m_owner->GetPosition().y + 25.0f, m_owner->GetPosition().z + 50.0f));
	//m_frontLight[1]->SetPosition(Vector3(m_owner->GetPosition().x + 100.0f, m_owner->GetPosition().y - 25.0f, m_owner->GetPosition().z + 50.0f));

	//m_backLight[0]->SetPosition(Vector3(m_owner->GetPosition().x - 100.0f, m_owner->GetPosition().y + 45.0f, m_owner->GetPosition().z + 50.0f));
	//m_backLight[1]->SetPosition(Vector3(m_owner->GetPosition().x - 100.0f, m_owner->GetPosition().y - 45.0f, m_owner->GetPosition().z + 50.0f));


	//m_spotLight = new SpotLight(m_position, SpotLight::VL_VERY_SMALL);
	//m_spotLight->SetTargetActor(this);
}

// デストラクタ
CarBody::~CarBody()
{
}

void CarBody::UpdateActor(float in_deltaTime)
{
	// オーナーの拡大率・座標・回転行列に合わせておく
	m_scale = m_owner->GetScale();
	m_position = m_owner->GetPosition();
	m_rotation = m_owner->GetRotation();

	// オーナーに合わせるためワールド座標を取得し続ける
	m_worldTransform = m_owner->GetWorldTransform();

	//// フロントライト・バックライトの座標調整
	//m_frontLight[0]->SetWorldTransform(Matrix4::CreateScale(m_frontLight[0]->GetScale()) * Matrix4::CreateFromQuaternion(GetRotation()) * Matrix4::CreateTranslation(Vector3::Transform(Vector3(m_owner->GetPosition().x + 15.0f, m_owner->GetPosition().y, m_owner->GetPosition().z), GetRotation())) * Matrix4::CreateTranslation(GetPosition() + Vector3(0.0f, 0.0f, 30.0f)));
	//m_frontLight[1]->SetWorldTransform(Matrix4::CreateScale(m_frontLight[1]->GetScale()) * Matrix4::CreateFromQuaternion(GetRotation()) * Matrix4::CreateTranslation(Vector3::Transform(Vector3(m_owner->GetPosition().x + 15.0f, m_owner->GetPosition().y, m_owner->GetPosition().z), GetRotation())) * Matrix4::CreateTranslation(GetPosition() + Vector3(0.0f, 0.0f, 30.0f)));

	//m_backLight[0]->SetWorldTransform(Matrix4::CreateScale(m_backLight[0]->GetScale()) * Matrix4::CreateFromQuaternion(GetRotation()) * Matrix4::CreateTranslation(Vector3::Transform(Vector3(m_owner->GetPosition().x - 15.0f, m_owner->GetPosition().y, m_owner->GetPosition().z), GetRotation())) * Matrix4::CreateTranslation(GetPosition() + Vector3(0.0f, 0.0f, 30.0f)));
	//m_backLight[1]->SetWorldTransform(Matrix4::CreateScale(m_backLight[1]->GetScale()) * Matrix4::CreateFromQuaternion(GetRotation()) * Matrix4::CreateTranslation(Vector3::Transform(Vector3(m_owner->GetPosition().x - 15.0f, m_owner->GetPosition().y, m_owner->GetPosition().z), GetRotation())) * Matrix4::CreateTranslation(GetPosition() + Vector3(0.0f, 0.0f, 30.0f)));

}
