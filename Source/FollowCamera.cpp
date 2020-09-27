//-----------------------------------------------------------------------+
// �ǔ��J����.
// 
// 
// copyright (C) 2019 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#include "FollowCamera.h"
#include "GameMain.h"
#include "Actor.h"

// �R���X�g���N�^
FollowCamera::FollowCamera(Actor * in_target)
	:CameraComponent(in_target)
	,m_horizonDist(200.0f)
	,m_verticalDist(95.0f)
	,m_targetDist(300.0f)
	,m_springConstant(64.0f)
{
}

// �X�V����
void FollowCamera::Update(float in_deltaTime)
{

	// �΂˒萔���猸�����v�Z
	float dampening = 4.0f * Math::Sqrt(m_springConstant);

	// ���z�̈ʒu���v�Z
	Vector3 idealPos = ComputeCameraPos();

	// ���ۂƗ��z�̍����v�Z
	Vector3 diff = m_actualPos - idealPos;
	// �΂˂ɂ������x
	Vector3 accel = -m_springConstant * diff - dampening * m_velocity;

	// ���x�̍X�V
	m_velocity += accel * in_deltaTime;
	// ���ۂ̃J�����ʒu���X�V
	m_actualPos += m_velocity * in_deltaTime;
	// m_actualPos.x -= 20.0f;


	// �^�[�Q�b�g�͏��L�A�N�^�[�̑O���Ɉʒu���闣�ꂽ�ڕW�_
	Vector3 target = m_owner->GetPosition() + m_owner->GetForward() * m_targetDist;

	// ���ۂ̃|�W�V�����ōs����Z�b�g
	Matrix4 view = Matrix4::CreateLookAt(m_actualPos, target, Vector3::UnitZ);
	SetViewMatrix(view);


}

// ���z�l�ɃJ�������X�i�b�v
void FollowCamera::SnapToIdeal()
{

	// ���ۂ̈ʒu�ɗ��z�̈ʒu���Z�b�g
	m_actualPos = ComputeCameraPos();
	// ���x�̓[����
	m_velocity = Vector3::Zero;
	
	// �����_�ƃr���[���v�Z
	Vector3 target = m_owner->GetPosition() + m_owner->GetForward() * m_targetDist;

	Matrix4 view = Matrix4::CreateLookAt(m_actualPos, target, Vector3::UnitZ);

	SetViewMatrix(view);

}

// �J�����̈ʒu���Z�b�g���č��W��Ԃ�
Vector3 FollowCamera::ComputeCameraPos() const
{

	// ���L�A�N�^�[�̏����ɂ���
	Vector3 cameraPos = m_owner->GetPosition();

	// �O�i�x�N�g��
	Vector3 forwardVec;
	forwardVec = m_owner->GetForward();                // �I�[�i�[�ł���v���C���[�̑O�i�x�N�g�����擾

	forwardVec = -m_horizonDist * forwardVec;          // �v���C���[�̑O�i�x�N�g���̔��Α��Ɉړ�
	forwardVec.z = m_verticalDist;                     // ������ݒ�

	cameraPos += forwardVec;                           // ���W�X�V

	return cameraPos;

}
