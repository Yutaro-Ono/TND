#include "HeliRotorBack.h"
#include "GameMain.h"
#include "Renderer.h"
#include "Mesh.h"
#include "MeshComponent.h"
#include "Helicopter.h"

static float radian = 0.0f;
//const Vector3 ADJUST_POS = Vector3(-320.0f, -10.0f, 95.0f);

HeliRotorBack::HeliRotorBack(Helicopter* in_heli, const std::string& in_filePath)
{
	// �I�[�i�[�N���X���Z�b�g
	m_owner = in_heli;

	// ���b�V���̃��[�h�E�����_���[�ւ̓o�^
	Mesh* mesh = RENDERER->GetMesh(in_filePath);
	// ���b�V���R���|�[�l���g�Ƀ��[�h�������b�V�����Z�b�g
	m_meshComp = new MeshComponent(this);
	m_meshComp->SetMesh(mesh);
	m_meshComp->SetMapColor(Vector3(0.8f, 0.45f, 0.75f));
	// �I�[�i�[�ɍ��W�E�X�P�[���E��]�����킹��
	SetScale(m_owner->GetScale());
	SetPosition(m_owner->GetPosition());
	SetRotation(m_owner->GetRotation());

}

HeliRotorBack::~HeliRotorBack()
{
}

void HeliRotorBack::UpdateActor(float in_deltaTime)
{
	const Vector3 ADJUST_POS = Vector3(-320.0f, -10.0f, 95.0f);

	// �I�[�i�[�ɍ��W�E��]�E���[���h�s������킹��
	m_scale = m_owner->GetScale();
	m_position = m_owner->GetPosition() + ADJUST_POS;
	m_rotation = m_owner->GetRotation();

	// �v���y����]
	radian += 20.0f * in_deltaTime;
	if (radian >= 360.0f)
	{
		radian = 0.0f;
	}
	Quaternion rot = Quaternion::Quaternion(Vector3::UnitY, radian);

	// �ʒu�����x�N�g������]�ɉ����čs�i
	Vector3 offset = ADJUST_POS;
	offset = Vector3::Transform(offset, m_rotation);
	// ���[���h�ϊ��s��X�V
	m_worldTransform = Matrix4::CreateFromQuaternion(rot) * m_owner->GetWorldTransform() * Matrix4::CreateTranslation(offset);

}
