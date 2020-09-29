#include "OrbitCamera.h"
#include "Actor.h"
#include "Input.h"
#include "InputController.h"

OrbitCamera::OrbitCamera(Actor* in_target)
	:CameraComponent(in_target)
	,m_offset(-250.0f, 0.0f, 100.0f)
	,m_upVec(Vector3::UnitZ)
	,m_pitch(0.0f)
	,m_yaw(0.0f)
{
	
}

OrbitCamera::~OrbitCamera()
{
}

void OrbitCamera::Update(float in_deltaTime)
{
	ProcessInput(in_deltaTime);

	// ヨーのクォータニオンをワールド変換行列の上方ベクトルから生成
	Quaternion yaw(Vector3::UnitZ, m_yaw * in_deltaTime);

	// オフセットにヨーの値を入れて更新
	m_offset = Vector3::Transform(m_offset, yaw);
	// カメラの上方ベクトルもヨーから更新
	m_upVec = Vector3::Transform(m_upVec, yaw);

	// カメラの前進ベクトルをオフセットから算出
	Vector3 forwardVec = -1.0f * m_offset;
	forwardVec.Normalize();

	// 右方向ベクトルを上方ベクトルと前進ベクトルから算出
	Vector3 rightVec = Vector3::Cross(m_upVec, forwardVec);
	rightVec.Normalize();

	// カメラの右方向ベクトルからピッチのクォータニオンを生成
	Quaternion pitch(rightVec, m_pitch * in_deltaTime);

	// オフセットにピッチの値を入れて更新
	m_offset = Vector3::Transform(m_offset, pitch);
	// カメラの情報ベクトルもピッチから更新
	m_upVec = Vector3::Transform(m_upVec, pitch);

	// 実際のワールド座標をターゲットの座標とオフセットから算出
	Vector3 targetPos = m_owner->GetPosition();
	Vector3 cameraPos = targetPos + m_offset;

	// ビュー行列を更新
	Matrix4 view = Matrix4::CreateLookAt(cameraPos, targetPos, m_upVec);
	SetViewMatrix(view);
}

// カメラ入力処理
void OrbitCamera::ProcessInput(float in_deltaTime)
{
	// コントローラ接続時と未接続時で処理を分岐
	if (CONTROLLER_INSTANCE.IsAvailable())
	{
		// 右スティックの入力値を格納する変数
		Vector2 axisR;
		axisR = CONTROLLER_INSTANCE.GetRAxisVec();

		const int maxCameraSpeed = 50;
		const float maxOrbitSpeed = Math::Pi * 8;

		// ヨー速度、ピッチ速度
		float yawSpeed, pitchSpeed;
		yawSpeed = pitchSpeed = 0.0f;
		// ヨー計算
		yawSpeed = axisR.x / maxCameraSpeed;
		yawSpeed *= maxOrbitSpeed;
		// メンバにセット
		SetYaw(yawSpeed);

		// ピッチ計算
		pitchSpeed = axisR.y / maxCameraSpeed;
		pitchSpeed *= maxOrbitSpeed;
		// メンバにセット
		SetPitch(pitchSpeed);


	}
	else
	{




	}
}
