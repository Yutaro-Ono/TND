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
	// �I�[�i�[�̊g�嗦�E���W�E��]�s��ɍ��킹�Ă���
	m_scale = m_owner->GetScale();
	m_position = m_owner->GetPosition();
	m_rotation = m_owner->GetRotation();

	// �I�[�i�[�ɍ��킹�邽�߃��[���h���W���擾��������
	m_worldTransform = m_owner->GetWorldTransform();

}
