#include "HeliSpotLight.h"
#include "GameMain.h"
#include "Renderer.h"
#include "Mesh.h"
#include "MeshComponent.h"
#include "Helicopter.h"

HeliSpotLight::HeliSpotLight(Helicopter* in_heli, const std::string& in_filePath)
{
	// �I�[�i�[�N���X���Z�b�g
	m_owner = in_heli;

	// ���b�V���̃��[�h�E�����_���[�ւ̓o�^
	Mesh* mesh = RENDERER->GetMesh(in_filePath);
	// ���b�V���R���|�[�l���g�Ƀ��[�h�������b�V�����Z�b�g
	m_meshComp = new MeshComponent(this);
	m_meshComp->SetMesh(mesh);

	// �I�[�i�[�ɍ��W�E�X�P�[���E��]�E���[���h�s������킹��
	SetScale(m_owner->GetScale());
	SetPosition(m_owner->GetPosition());
	SetRotation(m_owner->GetRotation());
	SetWorldTransform(m_owner->GetWorldTransform());
}

HeliSpotLight::~HeliSpotLight()
{
}

void HeliSpotLight::UpdateActor(float in_deltaTime)
{
	// �I�[�i�[�ɍ��W�E�X�P�[���E��]�E���[���h�s������킹��
	SetScale(m_owner->GetScale());
	SetPosition(m_owner->GetPosition());
	SetRotation(m_owner->GetRotation());
	SetWorldTransform(m_owner->GetWorldTransform());
}
