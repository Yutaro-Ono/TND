// 人間操作用カメラ(一人称・三人称ハイブリッド)
#pragma once
#include "CameraComponent.h"

class HumanCamera : public CameraComponent
{


public:

	HumanCamera(class PlayerHuman* in_owner);
	~HumanCamera();

	void Update(float in_deltaTime) override;

	void ProcessInput(float in_deltaTime) override;



	//-----------------------------------------------------------+
	// Getter/Setter
	//-----------------------------------------------------------+
	// ピッチのセッター・ゲッター
	float GetPitch() const { return m_pitch; }
	void SetPitch(float in_pitch) { m_pitch = in_pitch; }
	// ピッチ速度のセット
	void SetPitchSpeed(float in_speed) { m_pitchSpeed = in_speed; }
	// ピッチの最大速度のセット
	void SetMaxPitch(float in_maxSpeed) { m_maxPitch = in_maxSpeed; }
	// ヨーのセッター・ゲッター
	float GetYaw() const { return m_yaw; }
	void SetYaw(float in_yaw) { m_yaw = in_yaw; }

private:

	class PlayerHuman* m_player;

	Vector3 m_position;       // カメラの座標
	Vector3 m_offset;        // 三人称視点時ターゲットからの距離オフセット
	Vector3 m_upVec;           // カメラの上方ベクトル

	// カメラ加速度
	Vector3 m_velocity;

	// オーナーアクタの移動時に使用する前進ベクトル(カメラ基準)
	Vector3 m_playerForward;

	// ピッチ関連
	float m_pitch;
	float m_pitchSpeed;
	float m_maxPitch;

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