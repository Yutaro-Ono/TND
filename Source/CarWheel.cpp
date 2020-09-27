#include "CarWheel.h"

CarWheel::CarWheel(PlayerCar* in_owner, const std::string& in_meshPath)
	:m_owner(in_owner)
{
	m_position = m_owner->GetPosition();

	// ���b�V���̃Z�b�g
	Mesh* mesh = GAME_INSTANCE.GetRenderer()->GetMesh(in_meshPath);
	m_meshComp = new MeshComponent(this);
	m_meshComp->SetMesh(mesh);

}

CarWheel::~CarWheel()
{
}

void CarWheel::UpdateActor(float in_deltaTime)
{
	// �I�[�i�[�ɍ��킹�邽�߃��[���h���W���擾��������
	m_worldTransform = m_owner->GetWorldTransform();
}
