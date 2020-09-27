//-----------------------------------------------------------------------+
// 追尾カメラ.
// 
// 
// copyright (C) 2019 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#pragma once
#include "CameraComponent.h"

class FollowCamera : public CameraComponent
{


public:

	FollowCamera(class Actor* in_target);                    // コンストラクタ

	void Update(float in_deltaTime) override;          // カメラ更新(オーバーライド)

	void SnapToIdeal();                                     // 理想の位置に現在位置を持っていく

	// Setter
	void SetHorizonDist(float in_dist) { m_horizonDist = in_dist; }
	void SetVerticalDist(float in_dist) { m_verticalDist = in_dist; }
	void SetTargetDist(float in_dist) { m_targetDist = in_dist; }
	void SetSpringConstant(float in_spring) { m_springConstant = in_spring; }


private:

	Vector3 ComputeCameraPos() const;


	Vector3 m_actualPos;          // 実際のカメラ位置

	Vector3 m_velocity;           // actualPosの加速度

	float m_horizonDist;          // 水平方向距離
	float m_verticalDist;         // 垂直方向距離
	float m_targetDist;           // ターゲットまでの距離

	float m_springConstant;       


};