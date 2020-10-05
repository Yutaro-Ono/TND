#include "ThirdPersonCarCamera.h"
#include "Actor.h"
#include "PlayerCar.h"
#include "Input.h"
#include "InputController.h"
#include "Mouse.h"

const float ThirdPersonCarCamera::CAMERA_SENSITIVITY = 50.0f;
const Vector3 ThirdPersonCarCamera::DEFAULT_DISTANCE_OFFSET = Vector3(-250.0f, 0.0f, 100.0f);
const float ThirdPersonCarCamera::MIN_TARGET_DISTANCE = -80.0f;
const float ThirdPersonCarCamera::MAX_TARGET_DISTANCE = 300.0f;

ThirdPersonCarCamera::ThirdPersonCarCamera(PlayerCar* in_target)
	:CameraComponent(in_target)
	,m_playerCar(in_target)
	,m_position(Vector3::Zero)
	, m_offset(DEFAULT_DISTANCE_OFFSET)
	, m_upVec(Vector3::UnitZ)
	,m_velocity(Vector3::Zero)
	,m_pitch(0.0f)
	,m_yaw(0.0f)
	,m_distance(0.0f)
	,m_mousePos(MOUSE_INSTANCE.GetPosition())
	,m_frameMousePos(MOUSE_INSTANCE.GetPosition())
{

}

ThirdPersonCarCamera::~ThirdPersonCarCamera()
{
}

void ThirdPersonCarCamera::Update(float in_deltaTime)
{

	// 最終的にレンダラーにセットするビュー行列
	Matrix4 view;

	// 入力に関する処理
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

		// 距離ベクトル
		Vector3 dist = Vector3(0.0f, 0.0f, m_distance);

		// ワールド座標をターゲットの座標とオフセットから算出
		Vector3 targetPos = m_owner->GetPosition();
		m_position = targetPos + m_offset;


		// ビュー行列を更新
		view = Matrix4::CreateLookAt(m_position, targetPos, m_upVec);
		// 距離分を加算
		view = view * Matrix4::CreateTranslation(dist);

	// プレイヤーの車が走っていない時のみ、軌道カメラ操作を行う
	if (CONTROLLER_INSTANCE.GetAxisValue(SDL_CONTROLLER_AXIS_TRIGGERRIGHT)  < 0.1f && 
		CONTROLLER_INSTANCE.GetAxisValue(SDL_CONTROLLER_AXIS_TRIGGERRIGHT) > 0.1f ||
		INPUT_INSTANCE.IsKeyPressed(SDL_SCANCODE_W))
	{
		// ばね定数
		const float springConstant = 8.0f;

		// ばね定数から減衰を計算
		float dampening = 3.0f * Math::Sqrt(springConstant);

		// 理想の位置を計算
		Vector3 idealPos = ComputeCameraPos();

		// 実際と理想の差を計算
		Vector3 diff = m_position - idealPos;
		// ばねによる加速度
		Vector3 accel = -springConstant * diff - dampening * m_velocity;

		// 速度の更新
		m_velocity += accel * in_deltaTime;
		// 実際のカメラ位置を更新
		m_position += m_velocity * in_deltaTime;

		Vector3 viewTarget = m_playerCar->GetPosition() + m_playerCar->GetForward() * DEFAULT_DISTANCE_OFFSET.x;
		view = Matrix4::CreateLookAt(m_position, viewTarget, Vector3::UnitZ);

	}

	// ビュー行列のセット
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

		// ターゲットまでの距離を変更 (backボタンを押している時)
		if (CONTROLLER_INSTANCE.IsPressed(SDL_CONTROLLER_BUTTON_BACK))
		{
			// 距離を右スティックの入力値から算出
			m_distance += axisR.y * 500.0f * in_deltaTime;

			// 最小距離・最大距離の調整
			if (m_distance < MIN_TARGET_DISTANCE)
			{
				m_distance = MIN_TARGET_DISTANCE;
			}
			if (m_distance > MAX_TARGET_DISTANCE)
			{
				m_distance = MAX_TARGET_DISTANCE;
			}
		}

		// backボタンを押していない時
		if(!CONTROLLER_INSTANCE.IsPressed(SDL_CONTROLLER_BUTTON_BACK))
		{
			// カメラが引きすぎている場合に自動補正
			if (m_distance > MAX_TARGET_DISTANCE / 2)
			{
				m_distance = MAX_TARGET_DISTANCE / 2;
			}


			// ピッチ計算
			pitchSpeed = axisR.y / CAMERA_SENSITIVITY;
			pitchSpeed *= maxOrbitSpeed;

		}

			// ヨー計算
			yawSpeed = axisR.x / CAMERA_SENSITIVITY;
			yawSpeed *= maxOrbitSpeed;
			// ヨーをメンバにセット
			SetYaw(yawSpeed);
			// ピッチをメンバにセット
			SetPitch(pitchSpeed);

			printf("distance = %f\n", m_offset.x);



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


// オーナーの後方から一定距離にカメラを合わせる
const Vector3& ThirdPersonCarCamera::ComputeCameraPos() const
{
	// オーナーアクターへの座標を取得
	Vector3 cameraPos = m_playerCar->GetPosition();

	// 前進ベクトルの取得
	Vector3 forwardVec = m_playerCar->GetForward();

	// 前進ベクトルの反対側に、一定の距離・高さを保った状態に更新
	forwardVec = DEFAULT_DISTANCE_OFFSET * forwardVec;
	forwardVec.z = DEFAULT_DISTANCE_OFFSET.z;

	// 座標の更新
	cameraPos += forwardVec;

	return cameraPos;
}
