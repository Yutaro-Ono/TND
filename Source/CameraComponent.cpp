#include "CameraComponent.h"
#include "Renderer.h"
#include "PhysicsWorld.h"
#include "BoxCollider.h"
#include "Collision.h"

CameraComponent::CameraComponent(Actor * in_target, int updateOrder)
	:Component(in_target, updateOrder)
	,m_position(Vector3::Zero)
{

	// �v���W�F�N�V�����s��Z�b�g
	Matrix4 projection = Matrix4::CreatePerspectiveFOV(Math::ToRadians(75.0f),
		static_cast<float>(GAME_CONFIG->GetScreenWidth()),
		static_cast<float>(GAME_CONFIG->GetScreenHeight()),
		1.0f, 40000.0f);
	RENDERER->SetProjectionMatrix(projection);

	//AABB cullingBox;
	//cullingBox.m_min.x = 1.0f;
	//cullingBox.m_min.y = 0.0f;
	//cullingBox.m_min.z = 0.0f;
	//cullingBox.m_max.x = 100000.0f;
	//cullingBox.m_max.y = (float)GAME_CONFIG->GetScreenWidth();
	//cullingBox.m_max.z = (float)GAME_CONFIG->GetScreenHeight();

	// �Q�[�����ɂ��̃J�������A�N�e�B�u�ȃJ�����Ƃ��ēo�^
	SetActive();
}

CameraComponent::~CameraComponent()
{
	// �Q�[�����炱�̃J�������폜
	GAME_INSTANCE.InActiveCamera(this);
}

// �v���W�F�N�V�����s��̕ύX�Ɠo�^������
void CameraComponent::ChangeProjectionMat(float in_fov, float in_near, float in_far)
{
	// �v���W�F�N�V�����s��Z�b�g
	Matrix4 projection = Matrix4::CreatePerspectiveFOV(Math::ToRadians(in_fov),
		static_cast<float>(GAME_CONFIG->GetScreenWidth()),
		static_cast<float>(GAME_CONFIG->GetScreenHeight()),
		in_near, in_far);
	RENDERER->SetProjectionMatrix(projection);
}

void CameraComponent::SetViewMatrix(const Matrix4 & in_view)
{
	RENDERER->SetViewMatrix(in_view);
}
