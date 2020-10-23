#pragma once
#include "CameraComponent.h"


class ThirdPersonCamera : public CameraComponent
{
public:

	ThirdPersonCamera(class Actor* in_owner);
	~ThirdPersonCamera();

	void Update(float in_deltaTime) override;           // 更新処理

	void ProcessInput(float in_deltaTime) override;     // 入力処理

	// 当たり判定処理
	void CollisionFix(class BoxCollider* in_hitCameraBox, class BoxCollider* in_hitBox);


	//-----------------------------------------------------------+
	// Getter/Setter
	//-----------------------------------------------------------+
	// ピッチのゲッター
	float GetPitch() const { return m_pitch; }
	// ヨーのゲッター
	float GetYaw() const { return m_yaw; }

	// ピッチ速度のセット
	void SetPitchSpeed(float in_speed) { m_pitchSpeed = in_speed; }
	// ピッチの最大速度のセット
	void SetMaxPitch(float in_maxSpeed) { m_maxPitch = in_maxSpeed; }

	// カメラ距離のセッター
	void SetDistance(float in_dist);

	// 前進ベクトルをオーナーと合わせるかどうかのセッター
	void SetAdjustForward(bool in_adjust) { m_adjustForward = in_adjust; }
	// カメラ追従処理を行うかどうかのセッター
	void SetChaseOwnerForward(bool in_chase) { m_chaseOwnerForward = in_chase; }

	// カメラ前進ベクトルのゲッター・セッター
	const Vector3& GetForward() { return m_forwardVec; }
	void SetCameraForward(const Vector3& in_forward) { m_forwardVec = in_forward; }

private:

	// オーナーの後方から一定距離にカメラ位置を調整し、その座標を返す
	const Vector3& ComputeCameraPos() const;

	Vector3 m_offset;        // 三人称視点時ターゲットからの距離オフセット

	Vector3 m_forwardVec;      // カメラの前進ベクトル
	Vector3 m_upVec;           // カメラの上方ベクトル

	// カメラ加速度
	Vector3 m_velocity;

	// オーナーアクタの移動時に使用する前進ベクトル(カメラ基準)
	Vector3 m_ownerForward;

	// ピッチ関連
	float m_pitch;
	float m_pitchSpeed;
	float m_maxPitch;

	// ヨー速度(/秒)
	float m_yaw;

	// ターゲットまでの距離
	float m_distance;

	// カメラの前進ベクトルとオーナーアクタの向きを合わせるか (デフォルト：ON)
	bool m_adjustForward;
	// カメラの前進ベクトルをオーナーアクタの前進ベクトルへ追従させるか (デフォルト：OFF)
	bool m_chaseOwnerForward;

	// 当たり判定
	class BoxCollider* m_hitBox;
	class Actor* m_cameraActor;

	// マウス入力
	Vector2 m_mousePos;                    // マウスの座標
	Vector2 m_frameMousePos;               // マウスの前フレームの座標

	static const float CAMERA_SENSITIVITY;
	static const Vector3 DEFAULT_DISTANCE_OFFSET;
	static const float MIN_TARGET_DISTANCE;        // ターゲットまでの最短距離
	static const float MAX_TARGET_DISTANCE;        // ターゲットまでの最大距離
};