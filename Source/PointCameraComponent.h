// �|�C���g�J���� (�ݒ肵���|�C���g�A�N�^�𒍎���������)
#pragma once
#include "CameraComponent.h"

class PointCameraComponent : public CameraComponent
{


public:

	// �J�������^�[�Q�b�g�A�N�^��Ǐ]���邩���Ȃ���
	enum CAMERA_CHASE
	{
		ENABLE,
		DISABLE
	};


	PointCameraComponent(class Actor* in_owner, const Vector3& in_offset);
	~PointCameraComponent();

	void Update(float in_deltaTime) override;


	//-----------------------------------------------------------+
	// Getter/Setter
	//-----------------------------------------------------------+
	void SetOffset(const Vector3& in_offset, const Vector3& in_upVec);
	void SetCameraChaseType(CAMERA_CHASE in_chase) { m_chaseType = in_chase; }
	void SetMoveVec(const Vector3& in_moveVec) { m_moveVec = in_moveVec; }
	void SetTargetOffset(const Vector3& in_offset) { m_targetOffset = in_offset; }

private:

	CAMERA_CHASE m_chaseType;

	Matrix4 m_view;              // �r���[�s��

	Vector3 m_offset;            // �O�l�̎��_���^�[�Q�b�g����̋����I�t�Z�b�g
	Vector3 m_targetOffset;      // �^�[�Q�b�g���W�̒����p�x�N�g��
	Vector3 m_upVec;             // ������x�N�g��

	Vector3 m_moveVec;           // ���t���[���X�V����ړ��x�N�g��

};