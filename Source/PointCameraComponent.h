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
	void SetOffset(const Vector3& in_offset);
	void SetCameraChaseType(CAMERA_CHASE in_chase) { m_chaseType = in_chase; }

private:

	CAMERA_CHASE m_chaseType;

	Matrix4 m_view;              // �r���[�s��

	Vector3 m_offset;            // �O�l�̎��_���^�[�Q�b�g����̋����I�t�Z�b�g

};