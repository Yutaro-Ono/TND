#include "CarDoor.h"

const Vector3 CarDoor::ADJUST_DOOR_POSITION = Vector3(0.0f, 0.0f, 0.0f);


CarDoor::CarDoor(PlayerCar* in_owner, const std::string& in_meshPath)
	:m_owner(in_owner)
{
	// �I�[�i�[�̍��W���Z�b�g
	m_position = m_owner->GetPosition();

	// ���b�V���̃Z�b�g
	Mesh* mesh = GAME_INSTANCE.GetRenderer()->GetMesh(in_meshPath);
	m_meshComp = new MeshComponent(this);
	m_meshComp->SetMesh(mesh);
}

CarDoor::~CarDoor()
{
}

void CarDoor::UpdateActor(float in_deltaTime)
{
	// �I�[�i�[�ɍ��킹�邽�߃��[���h���W���擾��������
	m_worldTransform = m_owner->GetWorldTransform();

	// �ʒu��␳
	m_position += ADJUST_DOOR_POSITION;


}
