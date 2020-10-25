#include "HeliRotorBack.h"
#include "GameMain.h"
#include "Renderer.h"
#include "Mesh.h"
#include "MeshComponent.h"
#include "Helicopter.h"

static float radian = 0.0f;
const Vector3 ADJUST_POS = Vector3(-320.0f, -10.0f, 95.0f);

HeliRotorBack::HeliRotorBack(Helicopter* in_heli, const std::string& in_filePath)
{
	// �I�[�i�[�N���X���Z�b�g
	m_owner = in_heli;

	// ���b�V���̃��[�h�E�����_���[�ւ̓o�^
	Mesh* mesh = RENDERER->GetMesh(in_filePath);
	// ���b�V���R���|�[�l���g�Ƀ��[�h�������b�V�����Z�b�g
	m_meshComp = new MeshComponent(this);
	m_meshComp->SetMesh(mesh);

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
	// �I�[�i�[�ɍ��W�E�X�P�[���E��]�E���[���h�s������킹��
	SetScale(m_owner->GetScale());
	m_position = m_owner->GetPosition() + ADJUST_POS;
	SetRotation(m_owner->GetRotation());


	// �v���y����]
	radian += 20.0f * in_deltaTime;
	if (radian >= 360.0f)
	{
		radian = 0.0f;
	}


	//SetRotation(Quaternion::Quaternion(Vector3::UnitY, radian));

	//m_worldTransform = Matrix4::CreateFromQuaternion(m_rotation) * Matrix4::CreateTranslation(ADJUST_POS) * m_owner->GetWorldTransform();
	//SetWorldTransform(Matrix4::CreateTranslation(ADJUST_POS) * m_owner->GetWorldTransform());
}
