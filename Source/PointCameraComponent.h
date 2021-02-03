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
	void SetOffset(const Vector3& in_offset);
	void SetCameraChaseType(CAMERA_CHASE in_chase) { m_chaseType = in_chase; }

private:

	CAMERA_CHASE m_chaseType;

	Matrix4 m_view;              // ビュー行列

	Vector3 m_offset;            // 三人称視点時ターゲットからの距離オフセット

};