#include "HumanCamera.h"
#include "PlayerHuman.h"

HumanCamera::HumanCamera(PlayerHuman* in_owner)
	:CameraComponent(in_owner)
	,m_pitch(0.0f)
	,m_pitchSpeed(0.0f)
	,m_maxPitch(Math::Pi / 3.0f)
{

}

HumanCamera::~HumanCamera()
{
}

// 更新処理
void HumanCamera::Update(float in_deltaTime)
{
	// オーナーアクタの座標を取得
	Vector3 cameraPos = m_owner->GetPosition();

	// ピッチをオーナー内で更新された速度から更新
	m_pitch += m_pitchSpeed * in_deltaTime;
	// 最大・最小値にピッチをクランプ
	m_pitch = Math::Clamp(m_pitch, -m_maxPitch, m_maxPitch);
	// ピッチからQuaternionを作成
	Quaternion pitchRot = Quaternion::Quaternion(m_owner->GetRight(), m_pitch);

	// オーナーアクタの前進ベクトルとピッチのQuaternionから、カメラの前進ベクトルを算出
	Vector3 viewForward = Vector3::Transform(m_owner->GetForward(), pitchRot);

	// 
	Vector3 target = cameraPos + viewForward * 100.0f;
	// 上方向ベクトルを作成
	Vector3 upVec = Vector3::Transform(Vector3::UnitZ, pitchRot);

	// ビュー行列を作成し、レンダラーへセット
	Matrix4 view = Matrix4::CreateLookAt(cameraPos, target, upVec);
	SetViewMatrix(view);
}
