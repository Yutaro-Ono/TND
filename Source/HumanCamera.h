// 人間操作用カメラ(一人称・三人称ハイブリッド)
#pragma once
#include "CameraComponent.h"

class HumanCamera : public CameraComponent
{


public:

	HumanCamera(class PlayerHuman* in_owner);
	~HumanCamera();

	void Update(float in_deltaTime) override;


	//-----------------------------------------------------------+
	// Getter/Setter
	//-----------------------------------------------------------+
	// ピッチ速度のセット
	void SetPitchSpeed(float in_speed) { m_pitchSpeed = in_speed; }
	// ピッチの最大速度のセット
	void SetMaxPitch(float in_maxSpeed) { m_maxPitch = in_maxSpeed; }

private:

	Vector3 m_offset;        // 三人称視点時ターゲットからの距離オフセット

	// ピッチ関連
	float m_pitch;
	float m_pitchSpeed;
	float m_maxPitch;
};