#include "CarDoor.h"


// in_leftRight = �E���̃h�A�������̃h�A���ňʒu�␳�x�N�g����ς���
// in_leftRight = true �� ����     false �� �E��
CarDoor::CarDoor(PlayerCar* in_owner, const std::string& in_meshPath, bool in_leftRight)
	:m_owner(in_owner)
{



	if (in_leftRight)
	{
		m_adjustPos = Vector3(20.0f, -100.0f, 100.0f);
	}
	else
	{
		m_adjustPos = Vector3(20.0f, 100.0f, 100.0f);
	}

	//// �g�嗦���I�[�i�[�ɍ��킹��
	m_scale = m_owner->GetScale();
	// �I�[�i�[�̍��W���Z�b�g
	m_position = m_owner->GetPosition() + m_adjustPos;

	m_rotation = m_owner->GetRotation();

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
	// �I�[�i�[�̊g�嗦�E���W�E��]�s��ɍ��킹�Ă���
	m_scale = m_owner->GetScale();
	m_position = m_owner->GetPosition() + m_adjustPos;
	m_rotation = m_owner->GetRotation();

	// �I�[�i�[�ɍ��킹�邽�߃��[���h���W���擾�������� (�␳�x�N�g���ňʒu��␳)
	m_worldTransform = Matrix4::CreateTranslation(m_adjustPos) * m_owner->GetWorldTransform();

}
