#include "ThirdPersonCarCamera.h"
#include "Actor.h"
#include "PlayerCar.h"
#include "Input.h"
#include "InputController.h"
#include "Mouse.h"

const float ThirdPersonCarCamera::CAMERA_SENSITIVITY = 50.0f;

ThirdPersonCarCamera::ThirdPersonCarCamera(PlayerCar* in_target)
	:CameraComponent(in_target)
	, m_offset(-250.0f, 0.0f, 100.0f)
	, m_upVec(Vector3::UnitZ)
	, m_pitch(0.0f)
	, m_yaw(0.0f)
	,m_mousePos(MOUSE_INSTANCE.GetPosition())
	,m_frameMousePos(MOUSE_INSTANCE.GetPosition())
{

}

ThirdPersonCarCamera::~ThirdPersonCarCamera()
{
}

void ThirdPersonCarCamera::Update(float in_deltaTime)
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

	// ワールド座標をターゲットの座標とオフセットから算出
	Vector3 targetPos = m_owner->GetPosition();
	Vector3 cameraPos = targetPos + m_offset;

	// ビュー行列を更新
	Matrix4 view = Matrix4::CreateLookAt(cameraPos, targetPos, m_upVec);
	SetViewMatrix(view);
}

// カメラ入力処理
void ThirdPersonCarCamera::ProcessInput(float in_deltaTime)
{

	const float maxOrbitSpeed = Math::Pi * 8;

	// コントローラ接続時と未接続時で処理を分岐
	if (CONTROLLER_INSTANCE.IsAvailable())
	{
		// 右スティックの入力値を格納する変数
		Vector2 axisR;
		axisR = CONTROLLER_INSTANCE.GetRAxisVec();


		// ヨー速度、ピッチ速度
		float yawSpeed, pitchSpeed;
		yawSpeed = pitchSpeed = 0.0f;
		// ヨー計算
		yawSpeed = axisR.x / CAMERA_SENSITIVITY;
		yawSpeed *= maxOrbitSpeed;
		// メンバにセット
		SetYaw(yawSpeed);

		// ピッチ計算
		pitchSpeed = axisR.y / CAMERA_SENSITIVITY;
		pitchSpeed *= maxOrbitSpeed;
		// メンバにセット
		SetPitch(pitchSpeed);


	}
	else
	{

		// マウスの位置からカメラのヨーとピッチを取得
		m_mousePos = MOUSE_INSTANCE.GetPosition();
		//float xoffset = (m_frameMousePos.x - m_mousePos.x) / CAMERA_SENSITIVITY;
		float yoffset = (m_frameMousePos.y - m_mousePos.y) / CAMERA_SENSITIVITY;

		// x座標のオフセット
		float xoffset = 0.0f;
		// マウスのx座標が前フレームのx座標と比べて値が異なる場合、ヨーを更新
		// 画面中央
		if (m_mousePos.x > m_frameMousePos.x || m_mousePos.x > m_frameMousePos.x && (m_mousePos.x - GAME_CONFIG->GetScreenWidth()) > 0.0f)
		{
			xoffset = -1.0f * in_deltaTime;
			xoffset *= maxOrbitSpeed;
		}
		else if (m_mousePos.x < m_frameMousePos.x || m_mousePos.x < m_frameMousePos.x && m_mousePos.x < GAME_CONFIG->GetScreenWidth())
		{
			xoffset = 1.0f * in_deltaTime;
			xoffset *= maxOrbitSpeed;
		}


		// ヨー速度、ピッチ速度
		float yawSpeed, pitchSpeed;
		yawSpeed = pitchSpeed = 0.0f;

		// ヨー計算
		yawSpeed -= xoffset;
		//yawSpeed *= maxOrbitSpeed;
		// メンバにセット
		SetYaw(yawSpeed);

		// ピッチ計算
		pitchSpeed -= yoffset;
		//pitchSpeed *= maxOrbitSpeed;
		// メンバにセット
		SetPitch(pitchSpeed);

		if (m_mousePos.x != 1919.0f && m_mousePos.x > 0.0f)
		{
			m_frameMousePos.x = MOUSE_INSTANCE.GetPositionX();
		}
		if (m_mousePos.y != 1079.0f && m_mousePos.y != 0.0f)
		{
			m_frameMousePos.y = MOUSE_INSTANCE.GetPositionY();
		}
	}
}
