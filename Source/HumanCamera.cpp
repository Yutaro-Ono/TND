#include "HumanCamera.h"
#include "PlayerHuman.h"

const float HumanCamera::CAMERA_SENSITIVITY = 20.0f;    // カメラ速度(低いほど高感度)
const Vector3 HumanCamera::DEFAULT_DISTANCE_OFFSET = Vector3(-50.0f, 0.0f, 20.0f);
const float HumanCamera::MIN_TARGET_DISTANCE = -10.0f;
const float HumanCamera::MAX_TARGET_DISTANCE = 150.0f;

HumanCamera::HumanCamera(PlayerHuman* in_owner)
	:CameraComponent(in_owner)
	,m_player(in_owner)
	, m_offset(DEFAULT_DISTANCE_OFFSET)
	, m_upVec(Vector3::UnitZ)
	,m_playerForward(Vector3::Zero)
	, m_yaw(0.0f)
	,m_pitch(0.0f)
	,m_pitchSpeed(0.0f)
	,m_maxPitch(Math::Pi / 3.0f)
	, m_distance(-30.0f)
	, m_mousePos(MOUSE_INSTANCE.GetPosition())
	, m_frameMousePos(MOUSE_INSTANCE.GetPosition())
{

}

HumanCamera::~HumanCamera()
{
}

// 更新処理
void HumanCamera::Update(float in_deltaTime)
{

	// 入力処理
	ProcessInput(in_deltaTime);

	// オーナーアクタの座標を取得
	Vector3 targetPos = m_owner->GetPosition();

	Matrix4 view;

	//----------------------------------------------------------------------------------+
    //
    // 一人称視点時の処理
    //
    //----------------------------------------------------------------------------------+
	if (m_player->GetPlayerPOV() == PlayerHuman::POV_FIRST_PERSON)
	{
		// ピッチをオーナー内で更新された速度から更新
		m_pitch += m_pitchSpeed * in_deltaTime;
		// 最大・最小値にピッチをクランプ
		m_pitch = Math::Clamp(m_pitch, -m_maxPitch, m_maxPitch);
		// ピッチからQuaternionを作成
		Quaternion pitchRot = Quaternion::Quaternion(m_owner->GetRight(), m_pitch);

		// オーナーアクタの前進ベクトルとピッチのQuaternionから、カメラの前進ベクトルを算出
		Vector3 viewForward = Vector3::Transform(m_owner->GetForward(), pitchRot);

		// 
		Vector3 target = targetPos + viewForward * 100.0f;
		// 上方向ベクトルを作成
		Vector3 upVec = Vector3::Transform(Vector3::UnitZ, pitchRot);

		// ビュー行列を作成し、レンダラーへセット
		view = Matrix4::CreateLookAt(targetPos, target, upVec);

		// プレイヤーの前進ベクトル更新
		m_player->RotateToNewForward(viewForward);

	}

	//----------------------------------------------------------------------------------+
	//
	// 三人称視点時の処理
	//
	//----------------------------------------------------------------------------------+
	if (m_player->GetPlayerPOV() == PlayerHuman::POV_THIRD_PERSON)
	{
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

		// カメラの情報ベクトルもピッチから更新
		m_upVec = Vector3::Transform(m_upVec, pitch);

		// オフセットにピッチの値を入れて更新
		m_offset = Vector3::Transform(m_offset, pitch);


		// ワールド座標をターゲットの座標とオフセットから算出
		//m_position = Vector3::Lerp(m_position, targetPos + m_offset, 2.0f * in_deltaTime);
		m_position = targetPos + m_offset;
		printf("position.x : %f, position.y : %f, position.z : %f\n", m_position.x, m_position.y, m_position.z);
		// ビュー行列を更新
		view = Matrix4::CreateLookAt(m_position, targetPos, m_upVec);

		// 距離ベクトル
		Vector3 dist = Vector3(-15.0f, -35.0f, m_distance);
		// 距離分を加算
		view = view * Matrix4::CreateTranslation(dist);



	}

	// プレイヤーの前進ベクトル更新
	m_player->RotateToNewForward(m_playerForward);

	// レンダラーのビュー行列更新
	SetViewMatrix(view);
}


// 入力処理
void HumanCamera::ProcessInput(float in_deltaTime)
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
		if (!CONTROLLER_INSTANCE.IsPressed(SDL_CONTROLLER_BUTTON_BACK))
		{
			// カメラが引きすぎている場合に自動補正
			if (m_distance > MAX_TARGET_DISTANCE / 2)
			{
				m_distance = MAX_TARGET_DISTANCE / 2;
			}


			// ピッチ計算
			pitchSpeed = axisR.y / CAMERA_SENSITIVITY;
			pitchSpeed *= maxOrbitSpeed;

			// プレイヤーの前進ベクトルを更新
			m_playerForward = m_player->GetPosition() - m_position;
			m_playerForward.z = 0.0f;
			m_playerForward.Normalize();

		}

		// ヨー計算
		yawSpeed = axisR.x / CAMERA_SENSITIVITY;
		yawSpeed *= maxOrbitSpeed;


		// ピッチの最大・最小角度を調整
		const float pitchMaxDegree = 40.0f; // カメラピッチ最高角度(degree)
		const float pitchMinDegree = -20.0f; // カメラピッチ最低角度(degree)
		if (axisR.y > 0.0f && m_offset.z + pitchSpeed > pitchMaxDegree)
		{
			pitchSpeed = 0.0f;
		}
		if (axisR.y < 0.0f && m_offset.z + pitchSpeed < pitchMinDegree)
		{
			pitchSpeed = 0.0f;		
		}


		// ヨーをメンバにセット
		SetYaw(yawSpeed);
		// ピッチをメンバにセット
		SetPitch(pitchSpeed);

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


