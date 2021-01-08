#include "CarBody.h"
#include "PlayerCar.h"
#include "PhysicsWorld.h"
#include "BoxCollider.h"
#include "Collision.h"
#include "EnvironmentMapComponent.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "CarMeshComponent.h"
// ���b�V���p�X
const std::string CarBody::CAR_BODY_MESH_PATH = "Data/Meshes/TND/Actors/Car/Player/Body/OnlyFrame/BodyOnlyFrameLessMirror_Internal.OBJ";
const std::string CarBody::CAR_GLASS_MESH_PATH = "Data/Meshes/TND/Actors/Car/Player/Body/Glass/IncludeInterior/BodyGlassIncludeInterior_Internal.OBJ";
const std::string CarBody::CAR_INTERIOR_MESH_PATH = "Data/Meshes/TND/Actors/Car/Player/Body/Interior/InteriorGlassLess_Internal.OBJ";

// �R���X�g���N�^
CarBody::CarBody(PlayerCar* in_owner)
	:m_owner(in_owner)
{
	// �I�[�i�[�̊g�嗦�E���W�E��]�s��ɍ��킹�Ă���
	m_scale = m_owner->GetScale();
	m_position = m_owner->GetPosition();
	m_rotation = m_owner->GetRotation();
	// ���b�V���̃Z�b�g
	// �{�f�B�t���[���E�K���X�E����
	Mesh* bodyFrameMesh = RENDERER->GetMesh(CAR_BODY_MESH_PATH);
	CarMeshComponent* bodyFrame = new CarMeshComponent(this);
	//MeshComponent* bodyFrame = new MeshComponent(this);
	bodyFrame->SetMesh(bodyFrameMesh);
	// ���K���X (���}�b�s���O)
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

	// �������b�V��
	Mesh* interiorMesh = RENDERER->GetMesh(CAR_INTERIOR_MESH_PATH);
	MeshComponent* interior = new MeshComponent(this);
	interior->SetMesh(interiorMesh);

	// �{�f�B�̃��b�V������Ƃ��ăI�[�i�[�̓����蔻��{�b�N�X���Z�b�g
	// �����蔻��{�b�N�X�̃Z�b�g
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


	// �t�����g���C�g�E�o�b�N���C�g�̃Z�b�g
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

// �f�X�g���N�^
CarBody::~CarBody()
{
}

void CarBody::UpdateActor(float in_deltaTime)
{
	// �I�[�i�[�̊g�嗦�E���W�E��]�s��ɍ��킹�Ă���
	m_scale = m_owner->GetScale();
	m_position = m_owner->GetPosition();
	m_rotation = m_owner->GetRotation();

	// �I�[�i�[�ɍ��킹�邽�߃��[���h���W���擾��������
	m_worldTransform = m_owner->GetWorldTransform();

	// �t�����g���C�g�E�o�b�N���C�g�̍��W����
	for (int i = 0; i < 2; i++)
	{
		m_frontLight[i]->SetWorldTransform(Matrix4::CreateScale(m_frontLight[i]->GetScale()) * Matrix4::CreateFromQuaternion(GetRotation()) * Matrix4::CreateTranslation(Vector3::Transform(Vector3(45.0f, 20.0f, 0.0f), GetRotation())) * Matrix4::CreateTranslation(GetPosition() + Vector3(0.0f, 0.0f, 30.0f)));
		m_backLight[i]->SetWorldTransform(Matrix4::CreateScale(m_backLight[i]->GetScale()) * Matrix4::CreateFromQuaternion(GetRotation()) * Matrix4::CreateTranslation(Vector3::Transform(Vector3(-45.0f, 20.0f, 0.0f), GetRotation())) * Matrix4::CreateTranslation(GetPosition() + Vector3(0.0f, 0.0f, 30.0f)));
	}
}
