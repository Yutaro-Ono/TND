// 車操作用カメラ

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

	class PlayerCar* m_playerCar;      // プレイヤーへのポインタ

	Vector3 m_offset;          // ターゲットからの距離オフセット

	Vector3 m_upVec;           // カメラの上方ベクトル

	// カメラ加速度
	Vector3 m_velocity;

	// ピッチ速度(/秒)
	float m_pitch;

	// ヨー速度(/秒)
	float m_yaw;

	// ターゲットまでの距離
	float m_distance;

	// マウス
	Vector2 m_mousePos;                    // マウスの座標
	Vector2 m_frameMousePos;               // マウスの前フレームの座標

	static const float CAMERA_SENSITIVITY;
	static const Vector3 DEFAULT_DISTANCE_OFFSET;
	static const float MIN_TARGET_DISTANCE;        // ターゲットまでの最短距離
	static const float MAX_TARGET_DISTANCE;        // ターゲットまでの最大距離
};