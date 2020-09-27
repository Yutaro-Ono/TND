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

	Vector3 m_actualPos;          // 実際のカメラ位置

	Vector3 m_velocity;           // actualPosの加速度

	float m_horizonDist;          // 水平方向距離
	float m_verticalDist;         // 垂直方向距離
	float m_targetDist;           // ターゲットまでの距離



	float m_springConstant;

};