#pragma once
#include "Camera.h"

class PlayerCamera : public Camera
{


public:

	PlayerCamera(class Actor* in_targetActor);
	~PlayerCamera();

	void UpdateActor(float in_deltaTime) override;

	void SnapToIdeal();

	void ShakeCamera();


	// Setter
	void SetHorizonDist(float in_dist) { m_horizonDist = in_dist; }
	void SetVerticalDist(float in_dist) { m_verticalDist = in_dist; }
	void SetTargetDist(float in_dist) { m_targetDist = in_dist; }
	void SetSpringConstant(float in_spring) { m_springConstant = in_spring; }

	void SetBestCameraDist();

private:

	Vector3 ComputeCameraPos() const;

	Vector3 m_actualPos;          // ���ۂ̃J�����ʒu

	Vector3 m_velocity;           // actualPos�̉����x

	float m_horizonDist;          // ������������
	float m_verticalDist;         // ������������
	float m_targetDist;           // �^�[�Q�b�g�܂ł̋���



	float m_springConstant;

};