// ポイントカメラ (設定したポイントアクタを注視し続ける)
#pragma once
#include "CameraComponent.h"

class PointCameraComponent : public CameraComponent
{


public:

	// カメラがターゲットアクタを追従するかしないか
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
	void SetOffset(const Vector3& in_offset, const Vector3& in_upVec);
	void SetCameraChaseType(CAMERA_CHASE in_chase) { m_chaseType = in_chase; }
	void SetMoveVec(const Vector3& in_moveVec) { m_moveVec = in_moveVec; }
	void SetTargetOffset(const Vector3& in_offset) { m_targetOffset = in_offset; }

private:

	CAMERA_CHASE m_chaseType;

	Matrix4 m_view;              // ビュー行列

	Vector3 m_offset;            // 三人称視点時ターゲットからの距離オフセット
	Vector3 m_targetOffset;      // ターゲット座標の調整用ベクトル
	Vector3 m_upVec;             // 上方向ベクトル

	Vector3 m_moveVec;           // 毎フレーム更新する移動ベクトル

};