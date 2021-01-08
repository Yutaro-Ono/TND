#include "CarBody.h"
#include "PlayerCar.h"
#include "PhysicsWorld.h"
#include "BoxCollider.h"
#include "Collision.h"
#include "EnvironmentMapComponent.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "CarMeshComponent.h"
// メッシュパス
const std::string CarBody::CAR_BODY_MESH_PATH = "Data/Meshes/TND/Actors/Car/Player/Body/OnlyFrame/BodyOnlyFrameLessMirror_Internal.OBJ";
const std::string CarBody::CAR_GLASS_MESH_PATH = "Data/Meshes/TND/Actors/Car/Player/Body/Glass/IncludeInterior/BodyGlassIncludeInterior_Internal.OBJ";
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
	Mesh* glassMesh = RENDERER->GetMesh("Data/Meshes/TND/Actors/Car/Player/Body/Glass/Glass/BodyGlass.OBJ");
	//MeshComponent* glass = new MeshComponent(this);
	EnvironmentMapComponent* glass = new EnvironmentMapComponent(this);
	glass->SetMesh(glassMesh);

	glassMesh = RENDERER->GetMesh("Data/Meshes/TND/Actors/Car/Player/Body/Glass/FrontLight/FrontLight_Internal.OBJ");
	glass = new EnvironmentMapComponent(this);
	glass->SetMesh(glassMesh);

	glassMesh = RENDERER->GetMesh("Data/Meshes/TND/Actors/Car/Player/Body/Glass/BackLight/BackLight_Internal.OBJ");
	glass = new EnvironmentMapComponent(this);
	glass->SetMesh(glassMesh);

	// 内装メッシュ
	Mesh* interiorMesh = RENDERER->GetMesh(CAR_INTERIOR_MESH_PATH);
	MeshComponent* interior = new MeshComponent(this);
	interior->SetMesh(interiorMesh);

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


	// フロントライト・バックライトのセット
	for (int i = 0; i < 2; i++)
	{
		m_frontLight[i] = new PointLight(PointLight::VL_BIG);
		m_frontLight[i]->SetPosition(m_owner->GetPosition());
		m_frontLight[i]->SetLightColor(Vector3(1.0f, 1.0f, 1.0f), Vector3(1.0f, 1.0f, 1.0f));

		m_backLight[i] = new PointLight(PointLight::VL_BIG);
		m_backLight[i]->SetLightColor(Vector3(1.0f, 0.1f, 0.1f), Vector3(1.0f, 1.0f, 1.0f));
		m_backLight[i]->SetPosition(Vector3(m_position.x - 50.0f, m_position.y + (30.0f * (i + 1)), m_position.z));
	}

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

	// フロントライト・バックライトの座標調整
	for (int i = 0; i < 2; i++)
	{
		m_frontLight[i]->SetWorldTransform(Matrix4::CreateScale(m_frontLight[i]->GetScale()) * Matrix4::CreateFromQuaternion(GetRotation()) * Matrix4::CreateTranslation(Vector3::Transform(Vector3(45.0f, 20.0f, 0.0f), GetRotation())) * Matrix4::CreateTranslation(GetPosition() + Vector3(0.0f, 0.0f, 30.0f)));
		m_backLight[i]->SetWorldTransform(Matrix4::CreateScale(m_backLight[i]->GetScale()) * Matrix4::CreateFromQuaternion(GetRotation()) * Matrix4::CreateTranslation(Vector3::Transform(Vector3(-45.0f, 20.0f, 0.0f), GetRotation())) * Matrix4::CreateTranslation(GetPosition() + Vector3(0.0f, 0.0f, 30.0f)));
	}
}
