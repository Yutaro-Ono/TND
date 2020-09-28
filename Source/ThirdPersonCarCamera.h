#pragma once
#include "CameraComponent.h"

class ThirdPersonCarCamera : public CameraComponent
{

public:


	ThirdPersonCarCamera(class PlayerCar* in_target);
	~ThirdPersonCarCamera();


	void Update(float in_deltaTime) override;

	void ProcessInput() override;

	//--------------------------------------------+
	// Getter / Setter
	//--------------------------------------------+
	float GetPitch() const { return m_pitch; }
	void SetPitch(float in_pitch) { m_pitch = in_pitch; }

	float GetYaw() const { return m_yaw; }
	void SetYaw(float in_yaw) { m_yaw = in_yaw; }



private:

	Vector3 m_offset;          // ターゲットからの距離オフセット

	Vector3 m_upVec;           // カメラの上方ベクトル

	// ピッチ速度(/秒)
	float m_pitch;

	// ヨー速度(/秒)
	float m_yaw;

};