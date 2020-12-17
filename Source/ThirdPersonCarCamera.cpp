#include "ThirdPersonCarCamera.h"
#include "Actor.h"
#include "PlayerCar.h"
#include "Input.h"
#include "InputController.h"
#include "Mouse.h"

const float ThirdPersonCarCamera::CAMERA_SENSITIVITY = 65.0f;
const Vector3 ThirdPersonCarCamera::DEFAULT_DISTANCE_OFFSET = Vector3(-150.0f, 0.0f, 70.0f);
const float ThirdPersonCarCamera::MIN_TARGET_DISTANCE = -80.0f;
const float ThirdPersonCarCamera::MAX_TARGET_DISTANCE = 300.0f;

ThirdPersonCarCamera::ThirdPersonCarCamera(PlayerCar* in_target)
	:CameraComponent(in_target)
	,m_playerCar(in_target)
	, m_offset(Vector3(150.0f, -30.0f, 50.0f))
	, m_upVec(Vector3::UnitZ)
	,m_velocity(Vector3::Zero)
	,m_pitch(0.0f)
	,m_yaw(0.0f)
	,m_distance(0.0f)
	,m_mousePos(MOUSE_INSTANCE.GetPosition())
	,m_frameMousePos(MOUSE_INSTANCE.GetPosition())
	,m_chaseOwnerForward(false)
{

}

ThirdPersonCarCamera::~ThirdPersonCarCamera()
{
}

void ThirdPersonCarCamera::Update(float in_deltaTime)
{

	// 入力処理
	ProcessInput(in_deltaTime);

	// オーナーアクタの座標を取得
	Vector3 targetPos = m_owner->GetPosition();
	// ビュー行列
	Matrix4 view;

	float height = -35.0f;      // カメラの高さ


	// カメラが背後を追従するかしないか
	if (CONTROLLER_INSTANCE.GetAxisValue(SDL_CONTROLLER_AXIS_TRIGGERRIGHT) != 0.0f)
	{
		m_chaseOwnerForward = true;
	}


	//----------------------------------------------------+
	// カメラ追従あり
	//----------------------------------------------------+
	if (m_chaseOwnerForward)
	{
		// ばね定数
		const float springConstant = 128.0f;

		// ばね定数から減衰を計算
		float dampening = 3.0f * Math::Sqrt(springConstant);

		// 理想の位置を計算
		Vector3 idealPos = ComputeCameraPos();

		// 現在地と理想の差を計算
		Vector3 diff = m_position - idealPos;


		// ばねによる加速度
		Vector3 accel = -springConstant * diff - dampening * m_velocity;

		// 速度の更新
		m_velocity += accel * in_deltaTime;
		// 実際のカメラ位置を更新
		m_position += m_velocity * in_deltaTime;

		m_offset = idealPos - m_owner->GetPosition();
		m_upVec = Vector3::UnitZ;
		m_yaw = m_pitch = 0.0f;


		view = Matrix4::CreateLookAt(m_position, targetPos, m_upVec);
	}


	//---------------------------------------------------------+
	// カメラ追従無し
	//---------------------------------------------------------+
	else
	{
		// ヨーのクォータニオンをワールド変換行列の上方ベクトルから生成
		Quaternion yaw(Vector3::UnitZ, m_yaw * in_deltaTime);

		// オフセットにヨーの値を入れて更新
		m_offset = Vector3::Transform(m_offset, yaw);
		// カメラの上方ベクトルもヨーから更新
		m_upVec = Vector3::Transform(m_upVec, yaw);

		// カメラの前進ベクトルをオフセットから算出
		m_forwardVec = -1.0f * m_offset;
		m_forwardVec.Normalize();

		// 右方向ベクトルを上方ベクトルと前進ベクトルから算出
		Vector3 rightVec = Vector3::Cross(m_upVec, m_forwardVec);
		rightVec.Normalize();


		// カメラの右方向ベクトルからピッチのクォータニオンを生成
		Quaternion pitch(rightVec, m_pitch * in_deltaTime);

		// カメラの情報ベクトルもピッチから更新
		m_upVec = Vector3::Transform(m_upVec, pitch);

		// オフセットにピッチの値を入れて更新
		m_offset = Vector3::Transform(m_offset, pitch);


		// ワールド座標をターゲットの座標とオフセットから算出
		//m_position = Vector3::Lerp(m_position, targetPos + m_offset, 2.0f * in_deltaTime);

		Vector3 pos = targetPos + m_offset;
		//m_position = Vector3::Lerp(m_position, pos, attenRate * in_deltaTime);
		m_position = targetPos + m_offset;
		//printf("position.x : %f, position.y : %f, position.z : %f\n", m_position.x, m_position.y, m_position.z);
		// ビュー行列を更新
		view = Matrix4::CreateLookAt(m_position, targetPos, m_upVec);

		// 距離ベクトル
		Vector3 dist = Vector3(0.0f, 0.0, m_distance);
		// 距離分を加算
		view = view * Matrix4::CreateTranslation(dist);

	}

	// レンダラーのビュー行列更新
	SetViewMatrix(view);

}

// カメラ入力処理
void ThirdPersonCarCamera::ProcessInput(float in_deltaTime)
{

	const float maxOrbitSpeed = Math::Pi * 8;
	float attenRate = 3.0f;    // 減衰比率

	// コントローラ接続時と未接続時で処理を分岐
	if (CONTROLLER_INSTANCE.IsAvailable())
	{
		// 右スティックの入力値を格納する変数
		Vector2 axisR;
		axisR = CONTROLLER_INSTANCE.GetRAxisVec();

		
		if (axisR.x >= 0.9f || axisR.x <= -0.9f || axisR.y >= 0.9f || axisR.y <= -0.9f)
		{
			m_chaseOwnerForward = false;
		}

		// ヨー速度、ピッチ速度
		float yawSpeed, pitchSpeed;
		yawSpeed = pitchSpeed = 0.0f;

		// ピッチの最大角度・最小角度
		const float pitchMaxDegree = 80.0f; // カメラピッチ最高角度(degree)
		const float pitchMinDegree = 20.0f; // カメラピッチ最低角度(degree)

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
		// ピッチ計算
		pitchSpeed = axisR.y / CAMERA_SENSITIVITY;
		pitchSpeed *= maxOrbitSpeed;
		m_pitch = Math::Lerp(m_pitch, (axisR.y / CAMERA_SENSITIVITY * maxOrbitSpeed), attenRate * in_deltaTime);
		// ヨー計算
		m_yaw = Math::Lerp(m_yaw, (axisR.x / CAMERA_SENSITIVITY * maxOrbitSpeed), attenRate * in_deltaTime);


		// ピッチの最大・最小角度を調整
		if (axisR.y > 0.0f && m_offset.z + m_pitch > pitchMaxDegree)
		{
			m_pitch = 0.0f;
		}
		if (axisR.y < 0.0f && m_offset.z + m_pitch < pitchMinDegree)
		{
			m_pitch = 0.0f;
		}
			//printf("distance = %f\n", m_offset.x);



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
		//SetYaw(yawSpeed);

		// ピッチ計算
		pitchSpeed -= yoffset;
		//pitchSpeed *= maxOrbitSpeed;
		// メンバにセット
		//SetPitch(pitchSpeed);

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


void ThirdPersonCarCamera::SetDistance(float in_dist)
{
}

// オーナーの後方から一定距離にカメラを合わせる
const Vector3& ThirdPersonCarCamera::ComputeCameraPos() const
{
	// オーナーアクターへの座標を取得
	Vector3 cameraPos = m_playerCar->GetPosition();

	// 前進ベクトルの取得
	Vector3 forwardVec = m_playerCar->GetForward();

	// 前進ベクトルの反対側に、一定の距離・高さを保った状態に更新
	forwardVec = DEFAULT_DISTANCE_OFFSET.x * forwardVec;
	forwardVec.z = DEFAULT_DISTANCE_OFFSET.z;

	// 座標の更新
	cameraPos += forwardVec;

	return cameraPos;
}
