#include "ResultCarBody.h"
#include "PhysicsWorld.h"
#include "BoxCollider.h"
#include "Collision.h"
#include "EnvironmentMapComponent.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "CarMeshComponent.h"
#include "LightGlassComponent.h"
// ���b�V���p�X
const std::string ResultCarBody::CAR_BODY_MESH_PATH = "Data/Meshes/TND/Actors/Car/Player/Body/OnlyFrame/BodyOnlyFrameLessMirror_Internal.OBJ";
const std::string ResultCarBody::CAR_GLASS_MESH_PATH = "Data/Meshes/TND/Actors/Car/Player/Body/Glass/Glass/BodyGlass.OBJ";
const std::string ResultCarBody::CAR_FRONTLIGHT_MESH_PATH = "Data/Meshes/TND/Actors/Car/Player/Body/Glass/FrontLight/FrontLight_Internal.OBJ";
const std::string ResultCarBody::CAR_BACKLIGHT_MESH_PATH = "Data/Meshes/TND/Actors/Car/Player/Body/Glass/BackLight/BackLight_Internal.OBJ";
const std::string ResultCarBody::CAR_INTERIOR_MESH_PATH = "Data/Meshes/TND/Actors/Car/Player/Body/Interior/InteriorGlassLess_Internal.OBJ";

// �R���X�g���N�^
ResultCarBody::ResultCarBody(Actor* in_owner)
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
	Mesh* glassMesh = RENDERER->GetMesh(CAR_GLASS_MESH_PATH);
	//MeshComponent* glass = new MeshComponent(this);
	EnvironmentMapComponent* glass = new EnvironmentMapComponent(this);
	glass->SetMesh(glassMesh);
	glass->SetLuminance(0.04f);

	// �t�����g���C�g
	glassMesh = RENDERER->GetMesh(CAR_FRONTLIGHT_MESH_PATH);
	LightGlassComponent* lightGlass = new LightGlassComponent(this);
	lightGlass->SetMesh(glassMesh);
	lightGlass->SetLightColor(Vector3(1.0f, 0.6f, 0.4f));
	lightGlass->SetLightLuminance(0.5f);
	// �o�b�N���C�g
	glassMesh = RENDERER->GetMesh(CAR_BACKLIGHT_MESH_PATH);
	lightGlass = new LightGlassComponent(this);
	lightGlass->SetMesh(glassMesh);
	lightGlass->SetLightColor(Vector3(1.0f, 0.01f, 0.35f));
	lightGlass->SetLightLuminance(0.5f);

	// �������b�V��
	Mesh* interiorMesh = RENDERER->GetMesh(CAR_INTERIOR_MESH_PATH);
	MeshComponent* interior = new MeshComponent(this);
	interior->SetMesh(interiorMesh);
	interior->SetMapColor(Vector3(0.45f, 0.8f, 0.75f));

}

// �f�X�g���N�^
ResultCarBody::~ResultCarBody()
{
}

void ResultCarBody::UpdateActor(float in_deltaTime)
{
	// �I�[�i�[�̊g�嗦�E���W�E��]�s��ɍ��킹�Ă���
	m_scale = m_owner->GetScale();
	m_position = m_owner->GetPosition();
	m_rotation = m_owner->GetRotation();

	// �I�[�i�[�ɍ��킹�邽�߃��[���h���W���擾��������
	m_worldTransform = m_owner->GetWorldTransform();

}
