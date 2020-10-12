// �ԑ���p�J����

#pragma once
#include "CameraComponent.h"


class ThirdPersonCarCamera : public CameraComponent
{

public:


	ThirdPersonCarCamera(class PlayerCar* in_target);
	~ThirdPersonCarCamera();


	void Update(float in_deltaTime) override;

	void ProcessInput(float in_deltaTime) override;


	//--------------------------------------------+
	// Getter / Setter
	//--------------------------------------------+
	float GetPitch() const { return m_pitch; }
	void SetPitch(float in_pitch) { m_pitch = in_pitch; }

	float GetYaw() const { return m_yaw; }
	void SetYaw(float in_yaw) { m_yaw = in_yaw; }



private:


	const Vector3& ComputeCameraPos() const;

	class PlayerCar* m_playerCar;      // �v���C���[�ւ̃|�C���^

	Vector3 m_offset;          // �^�[�Q�b�g����̋����I�t�Z�b�g

	Vector3 m_upVec;           // �J�����̏���x�N�g��

	// �J���������x
	Vector3 m_velocity;

	// �s�b�`���x(/�b)
	float m_pitch;

	// ���[���x(/�b)
	float m_yaw;

	// �^�[�Q�b�g�܂ł̋���
	float m_distance;

	// �}�E�X
	Vector2 m_mousePos;                    // �}�E�X�̍��W
	Vector2 m_frameMousePos;               // �}�E�X�̑O�t���[���̍��W

	static const float CAMERA_SENSITIVITY;
	static const Vector3 DEFAULT_DISTANCE_OFFSET;
	static const float MIN_TARGET_DISTANCE;        // �^�[�Q�b�g�܂ł̍ŒZ����
	static const float MAX_TARGET_DISTANCE;        // �^�[�Q�b�g�܂ł̍ő勗��
};