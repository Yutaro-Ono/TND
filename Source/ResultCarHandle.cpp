#include "ResultCarHandle.h"
#include "Mesh.h"
#include "MeshComponent.h"

ResultCarHandle::ResultCarHandle(Actor* in_owner, const std::string& in_meshPath)
	:m_owner(in_owner)
{
	// �I�[�i�[�̍��W���Z�b�g
	m_position = m_owner->GetPosition();

	// ���b�V���̃Z�b�g
	Mesh* mesh = GAME_INSTANCE.GetRenderer()->GetMesh(in_meshPath);
	m_meshComp = new MeshComponent(this);
	m_meshComp->SetMesh(mesh);
}

ResultCarHandle::~ResultCarHandle()
{
}

void ResultCarHandle::UpdateActor(float in_deltaTime)
{
	// �I�[�i�[�̊g�嗦�E���W�E��]�s��ɍ��킹�Ă���
	m_scale = m_owner->GetScale();
	m_position = m_owner->GetPosition() + Vector3(20.0f, 0.0f, 30.0f);
	m_rotation = m_owner->GetRotation();

	Matrix4 handleMat = Matrix4::CreateScale(m_scale) * Matrix4::CreateFromQuaternion(m_rotation) * Matrix4::CreateTranslation(m_position);

	// �I�[�i�[�ɍ��킹�邽�߃��[���h���W���擾��������
	m_worldTransform = handleMat;

}
