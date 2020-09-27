//-----------------------------------------------------------------------+
// 追尾カメラ.
// 
// 
// copyright (C) 2019 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#include "FollowCamera.h"
#include "GameMain.h"
#include "Actor.h"

// コンストラクタ
FollowCamera::FollowCamera(Actor * in_target)
	:CameraComponent(in_target)
	,m_horizonDist(200.0f)
	,m_verticalDist(95.0f)
	,m_targetDist(300.0f)
	,m_springConstant(64.0f)
{
}

// 更新処理
void FollowCamera::Update(float in_deltaTime)
{

	// ばね定数から減衰を計算
	float dampening = 4.0f * Math::Sqrt(m_springConstant);

	// 理想の位置を計算
	Vector3 idealPos = ComputeCameraPos();

	// 実際と理想の差を計算
	Vector3 diff = m_actualPos - idealPos;
	// ばねによる加速度
	Vector3 accel = -m_springConstant * diff - dampening * m_velocity;

	// 速度の更新
	m_velocity += accel * in_deltaTime;
	// 実際のカメラ位置を更新
	m_actualPos += m_velocity * in_deltaTime;
	// m_actualPos.x -= 20.0f;


	// ターゲットは所有アクターの前方に位置する離れた目標点
	Vector3 target = m_owner->GetPosition() + m_owner->GetForward() * m_targetDist;

	// 実際のポジションで行列をセット
	Matrix4 view = Matrix4::CreateLookAt(m_actualPos, target, Vector3::UnitZ);
	SetViewMatrix(view);


}

// 理想値にカメラをスナップ
void FollowCamera::SnapToIdeal()
{

	// 実際の位置に理想の位置をセット
	m_actualPos = ComputeCameraPos();
	// 速度はゼロに
	m_velocity = Vector3::Zero;
	
	// 注視点とビューを計算
	Vector3 target = m_owner->GetPosition() + m_owner->GetForward() * m_targetDist;

	Matrix4 view = Matrix4::CreateLookAt(m_actualPos, target, Vector3::UnitZ);

	SetViewMatrix(view);

}

// カメラの位置をセットして座標を返す
Vector3 FollowCamera::ComputeCameraPos() const
{

	// 所有アクターの上後方にする
	Vector3 cameraPos = m_owner->GetPosition();

	// 前進ベクトル
	Vector3 forwardVec;
	forwardVec = m_owner->GetForward();                // オーナーであるプレイヤーの前進ベクトルを取得

	forwardVec = -m_horizonDist * forwardVec;          // プレイヤーの前進ベクトルの反対側に移動
	forwardVec.z = m_verticalDist;                     // 高さを設定

	cameraPos += forwardVec;                           // 座標更新

	return cameraPos;

}
