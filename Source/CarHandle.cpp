#include "CarHandle.h"

CarHandle::CarHandle(PlayerCar* in_owner, const std::string& in_meshPath)
	:m_owner(in_owner)
{
	SetScale(m_owner->GetScale());
	// ���b�V���̃Z�b�g
	Mesh* mesh = GAME_INSTANCE.GetRenderer()->GetMesh(in_meshPath);
	m_meshComp = new MeshComponent(this);
	m_meshComp->SetMesh(mesh);
}

CarHandle::~CarHandle()
{
}

void CarHandle::UpdateActor(float in_deltaTime)
{

	// �s��v�Z
	// �e�N���X�ƈʒu�����킹�������ŉ�]������K�v�����邽��
	// �����Ōv�Z����B(fix)
	Vector3 adjustPos = Vector3(18.0f, -15.0f, 30.0f);

	// �g�嗦�s��
	Matrix4 scale = Matrix4::CreateScale(m_owner->GetScale());
	// ��]�s��
	Matrix4 rot = Matrix4::CreateFromQuaternion(m_owner->GetRotation());
	// �����p�̍��W�s�� (��]�s��ɂ��ϊ�������)
	Matrix4 adjust = Matrix4::CreateTranslation(Vector3::Transform(adjustPos, rot));
	// ���[���h�ϊ��s��
	Matrix4 trns = Matrix4::CreateTranslation(m_owner->GetPosition());

	m_worldTransform = scale * rot * adjust * trns;

}
