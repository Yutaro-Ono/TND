#include "CarBody.h"
#include "PlayerCar.h"

// �R���X�g���N�^
CarBody::CarBody(PlayerCar* in_owner, const std::string& in_meshPath)
	:m_owner(in_owner)
{
	// �I�[�i�[�̍��W���Z�b�g
	m_position = m_owner->GetPosition();

	// ���b�V���̃Z�b�g
	Mesh* mesh = GAME_INSTANCE.GetRenderer()->GetMesh(in_meshPath);
	m_meshComp = new MeshComponent(this);
	m_meshComp->SetMesh(mesh);

	// �{�f�B�̃��b�V������Ƃ��ăI�[�i�[�̓����蔻��{�b�N�X���Z�b�g
	m_owner->CreateAABB(mesh);
}

// �f�X�g���N�^
CarBody::~CarBody()
{
}

void CarBody::UpdateActor(float in_deltaTime)
{
	// �I�[�i�[�ɍ��킹�邽�߃��[���h���W���擾��������
	m_worldTransform = m_owner->GetWorldTransform();
}
