#include "PlayerCamera.h"

// コンストラクタ
PlayerCamera::PlayerCamera(Actor * in_targetActor)
	:Camera(in_targetActor)
	, m_targetDist(300.0f)
	, m_springConstant(64.0f)
{
}

// デストラクタ
PlayerCamera::~PlayerCamera()
{
}

// カメラを更新
void PlayerCamera::UpdateActor(float in_deltaTime)
{

	// ばね定数から減衰を計算
	float dampening = 3.0f * Math::Sqrt(m_springConstant);

	// 理想の位置を計算
	Vector3 idealPos = ComputeCameraPos();

	// 実際と理想の差を計算
	Vector3 diff = m_position - idealPos;
	// ばねによる加速度
	Vector3 accel = -m_springConstant * diff - dampening * m_velocity;

	// 速度の更新
	m_velocity += accel * in_deltaTime;
	// 実際のカメラ位置を更新
	m_position += m_velocity * in_deltaTime;
	// m_actualPos.x -= 20.0f;


	// ターゲットは所有アクターの前方に位置する離れた目標点
	m_viewTarget = m_targetActor->GetPosition() + m_targetActor->GetForward() * m_targetDist;

}

// カメラを理想値にスナップ
void PlayerCamera::SnapToIdeal()
{
	// 実際の位置に理想の位置をセット
	m_position = ComputeCameraPos();
	// 速度はゼロに
	m_velocity = Vector3::Zero;

	// 注視点とビューを計算
	m_viewTarget = m_targetActor->GetPosition() + m_targetActor->GetForward() * m_targetDist;

	m_viewMatrix = Matrix4::CreateLookAt(m_position, m_viewTarget, Vector3::UnitZ);

	SetViewMatrix(m_viewMatrix);
}

// カメラのシェイク(現在実装)
void PlayerCamera::ShakeCamera()
{
}

// カメラのターゲットまでの距離をセット
void PlayerCamera::SetBestCameraDist()
{
	m_horizonDist = 200.0f;
	//m_verticalDist = 95.0f;
	m_verticalDist = 105.0f;
	m_targetDist = 300.0f;
}

// カメラの座標を更新し、返す
Vector3 PlayerCamera::ComputeCameraPos() const
{
	// 所有アクターの上後方にする
	Vector3 cameraPos = m_targetActor->GetPosition();

	// 前進ベクトル
	Vector3 forwardVec;
	forwardVec = m_targetActor->GetForward();                // オーナーであるプレイヤーの前進ベクトルを取得

	forwardVec = -m_horizonDist * forwardVec;          // プレイヤーの前進ベクトルの反対側に移動
	forwardVec.z = m_verticalDist;                     // 高さを設定

	cameraPos += forwardVec;                           // 座標更新

	return cameraPos;
}
