#pragma once
#include "CameraComponent.h"

class OrbitCamera : public CameraComponent
{


public:

	OrbitCamera(class Actor* in_target);
	~OrbitCamera();

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

	Vector3 m_offset;          // �^�[�Q�b�g����̋����I�t�Z�b�g

	Vector3 m_upVec;           // �J�����̏���x�N�g��

	// �s�b�`���x(/�b)
	float m_pitch;

	// ���[���x(/�b)
	float m_yaw;
};