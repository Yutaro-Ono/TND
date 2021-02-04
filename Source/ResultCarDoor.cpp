#include "ResultCarDoor.h"
#include "EnvironmentMapComponent.h"
#include "CarMeshComponent.h"
#include "Mesh.h"

const std::string ResultCarDoor::CAR_DOOR_LEFT_FRAME_MESH_PATH = "Data/Meshes/TND/Actors/Car/Player/Door/DoorLeft/DoorFrameLeft_Internal.OBJ";
const std::string ResultCarDoor::CAR_DOOR_LEFT_GLASS_MESH_PATH = "Data/Meshes/TND/Actors/Car/Player/Door/DoorLeft/DoorGlassLeftTransparency_Internal.OBJ";
const std::string ResultCarDoor::CAR_DOOR_RIGHT_FRAME_MESH_PATH = "Data/Meshes/TND/Actors/Car/Player/Door/DoorRight/DoorFrameRight_Internal.OBJ";
const std::string ResultCarDoor::CAR_DOOR_RIGHT_GLASS_MESH_PATH = "Data/Meshes/TND/Actors/Car/Player/Door/DoorRight/DoorGlassRightTransparency_Internal.OBJ";

// in_leftRight = �E���̃h�A�������̃h�A���ňʒu�␳�x�N�g����ς���
// in_leftRight = true �� ����     false �� �E��
ResultCarDoor::ResultCarDoor(Actor* in_owner, DOOR_POS in_pos)
	:m_owner(in_owner)
{

	// ���W�I�t�Z�b�g�ƃ��b�V���̃Z�b�g (���E�ŕ���)
	if (in_pos == DOOR_POS::LEFT)
	{
		m_adjustPos = Vector3(80.0f, -100.0f, 92.8f);
		// ����
		Mesh* frame = RENDERER->GetMesh(CAR_DOOR_LEFT_FRAME_MESH_PATH);
		CarMeshComponent* frameMeshComp = new CarMeshComponent(this);
		frameMeshComp->SetMesh(frame);
		Mesh* glass = RENDERER->GetMesh(CAR_DOOR_LEFT_GLASS_MESH_PATH);
		//MeshComponent* glassMeshComp = new MeshComponent(this);
		EnvironmentMapComponent* glassMeshComp = new EnvironmentMapComponent(this);
		glassMeshComp->SetMesh(glass);
	}
	else if(in_pos == DOOR_POS::RIGHT)
	{
		m_adjustPos = Vector3(80.0f, 100.0f, 96.8f);
		// �E��
		Mesh* frame = RENDERER->GetMesh(CAR_DOOR_RIGHT_FRAME_MESH_PATH);
		CarMeshComponent* frameMeshComp = new CarMeshComponent(this);
		frameMeshComp->SetMesh(frame);
		Mesh* glass = RENDERER->GetMesh(CAR_DOOR_RIGHT_GLASS_MESH_PATH);
		//MeshComponent* glassMeshComp = new MeshComponent(this);
		EnvironmentMapComponent* glassMeshComp = new EnvironmentMapComponent(this);
		glassMeshComp->SetMesh(glass);
		glassMeshComp->SetMesh(glass);
	}

	//// �g�嗦���I�[�i�[�ɍ��킹��
	m_scale = m_owner->GetScale();
	// �I�[�i�[�̍��W���Z�b�g
	m_position = m_owner->GetPosition() + m_adjustPos;

	m_rotation = m_owner->GetRotation();


}

ResultCarDoor::~ResultCarDoor()
{
}

void ResultCarDoor::UpdateActor(float in_deltaTime)
{
	// �I�[�i�[�̊g�嗦�E���W�E��]�s��ɍ��킹�Ă���
	m_scale = m_owner->GetScale();
	m_position = m_owner->GetPosition() + m_adjustPos;
	m_rotation = m_owner->GetRotation();

	// �I�[�i�[�ɍ��킹�邽�߃��[���h���W���擾�������� (�␳�x�N�g���ňʒu��␳)
	m_worldTransform = Matrix4::CreateTranslation(m_adjustPos) * m_owner->GetWorldTransform();

}
