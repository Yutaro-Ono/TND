#include "ThirdPersonCamera.h"
#include "Actor.h"
#include "PhysicsWorld.h"
#include "BoxCollider.h"
#include "Collision.h"
#include "MeshComponent.h"

const float ThirdPersonCamera::CAMERA_SENSITIVITY = 20.0f;    // カメラ速度(低いほど高感度)
const Vector3 ThirdPersonCamera::DEFAULT_DISTANCE_OFFSET = Vector3(-50.0f, 0.0f, 20.0f);
const float ThirdPersonCamera::MIN_TARGET_DISTANCE = 0.0f;
const float ThirdPersonCamera::MAX_TARGET_DISTANCE = 200.0f;

ThirdPersonCamera::ThirdPersonCamera(Actor* in_owner)
	:CameraComponent(in_owner)
	, m_offset(Vector3(50.0f, -30.0f, 10.0f))
	, m_upVec(Vector3::UnitZ)
	, m_ownerForward(m_owner->GetForward())
	, m_yaw(0.0f)
	, m_pitch(0.0f)
	, m_pitchSpeed(0.0f)
	, m_maxPitch(Math::Pi / 3.0f)
	, m_distance(20.0f)
	, m_adjustForward(true)
	, m_chaseOwnerForward(false)
	, m_mousePos(MOUSE_INSTANCE.GetPosition())
	, m_frameMousePos(MOUSE_INSTANCE.GetPosition())
{
	m_cameraActor = new Actor();
	m_cameraActor->SetPosition(m_position);

	// 向きベクトル初期化
	m_forwardVec = -1.0f * m_offset;
	m_forwardVec.Normalize();


	// 当たり判定ボックスのセット
	AABB hitBox;
	hitBox.m_min.x = -5.0f;
	hitBox.m_min.y = -5.0f;
	hitBox.m_min.z = -5.0f;
	hitBox.m_max.x = 5.0f;
	hitBox.m_max.y = 5.0f;
	hitBox.m_max.z = 5.0f;
	m_hitBox = new BoxCollider(m_cameraActor, PhysicsWorld::TYPE_CAMERA);
	m_hitBox->SetObjectBox(hitBox);

}

ThirdPersonCamera::~ThirdPersonCamera()
{
}

void ThirdPersonCamera::Update(float in_deltaTime)
{

	// 入力処理
	ProcessInput(in_deltaTime);

	// オーナーアクタの座標を取得
	Vector3 targetPos = m_owner->GetPosition();
	// ビュー行列
	Matrix4 view;

	float height = -35.0f;      // カメラの高さ


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
		Vector3 dist = Vector3(-15.0f, height, m_distance);
		// 距離分を加算
		view = view * Matrix4::CreateTranslation(dist);

		
		m_cameraActor->SetPosition(m_position);
		m_cameraActor->ComputeWorldTransform();
		m_hitBox->OnUpdateWorldTransform();
	}

	//printf("Camera : x : %f, y : %f, z : %f\n", m_position.x, m_position.y, m_position.z);
	//printf("hitBox : x : %f, y : %f, z : %f\n", m_cameraActor->GetPosition().x, m_cameraActor->GetPosition().y, m_cameraActor->GetPosition().z);

	// レンダラーのビュー行列更新
	SetViewMatrix(view);
}

void ThirdPersonCamera::ProcessInput(float in_deltaTime)
{
	const float maxOrbitSpeed = Math::Pi * 8;

	// ヨー速度、ピッチ速度
	float yawSpeed, pitchSpeed;
	yawSpeed = pitchSpeed = 0.0f;

	float attenRate = 3.0f;    // 減衰比率

	// ピッチの最大角度・最小角度
	const float pitchMaxDegree = 45.0f; // カメラピッチ最高角度(degree)
	const float pitchMinDegree = -10.0f; // カメラピッチ最低角度(degree)

	//-----------------------------------------------------+
	//
	// コントローラ接続時の処理
	//
	//-----------------------------------------------------+
	if (CONTROLLER_INSTANCE.IsAvailable())
	{
		

		// 右スティックの入力値を格納する変数
		Vector2 axisR;
		axisR = CONTROLLER_INSTANCE.GetRAxisVec();

		// backボタンを押していない時
		if (!CONTROLLER_INSTANCE.IsPressed(SDL_CONTROLLER_BUTTON_BACK))
		{
			

			// カメラが引きすぎている場合に自動補正
			if (m_distance > MAX_TARGET_DISTANCE / 2)
			{
				m_distance = MAX_TARGET_DISTANCE / 2;
			}

			// 前進ベクトル更新を許可している時のみ
			if (m_adjustForward)
			{
				// プレイヤーの前進ベクトルを更新
				//m_ownerForward = m_owner->GetPosition() - m_position;
				//m_ownerForward.z = 0.0f;
				//m_ownerForward.Normalize();
				// プレイヤーの前進ベクトル更新
				//m_owner->RotateToNewForward(m_ownerForward);
			}
		}

		// ターゲットまでの距離を変更 (backボタンを押している時)
		if (CONTROLLER_INSTANCE.IsPressed(SDL_CONTROLLER_BUTTON_BACK))
		{
			// 距離を右スティックの入力値から算出
			m_distance += axisR.y * 500.0f * in_deltaTime;
			// フリーカメラ時はピッチを変更しない
			axisR.y = 0.0f;

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

	}

	//-----------------------------------------------------+
    //
    // コントローラ未接続時の処理 (マウス操作)
    //
    //-----------------------------------------------------+
	else
	{
		//SDL_SetRelativeMouseMode(SDL_TRUE);

		// マウスの位置からカメラのヨーとピッチを取得
		m_mousePos = MOUSE_INSTANCE.GetPosition();

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





		// ターゲットまでの距離を変更 (Fキーを押している時)
		if (INPUT_INSTANCE.IsKeyPressed(SDL_SCANCODE_F))
		{
			// 距離を右スティックの入力値から算出
			m_distance += (m_frameMousePos.y - m_mousePos.y) / CAMERA_SENSITIVITY * in_deltaTime;
			// フリーカメラ時はピッチを変更しない


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

		// Fキーを押していない時
		if (!INPUT_INSTANCE.IsKeyPressed(SDL_SCANCODE_F))
		{
			// カメラが引きすぎている場合に自動補正
			if (m_distance > MAX_TARGET_DISTANCE / 2)
			{
				m_distance = MAX_TARGET_DISTANCE / 2;
			}

			// ピッチ計算
			float yoffset = (m_frameMousePos.y - m_mousePos.y) / CAMERA_SENSITIVITY * maxOrbitSpeed * in_deltaTime;
			m_pitch = Math::Lerp(m_pitch, -yoffset, attenRate * in_deltaTime);

			// 前進ベクトル更新を許可している時のみ
			if (m_adjustForward)
			{
				// プレイヤーの前進ベクトルを更新
			    //	m_ownerForward = m_owner->GetPosition() - m_position;
			    //	m_ownerForward.z = 0.0f;
		     	//	m_ownerForward.Normalize();
				// プレイヤーの前進ベクトル更新
			    //m_owner->RotateToNewForward(m_ownerForward);
			}
		}

		// ヨー計算
		yawSpeed -= xoffset;
		m_yaw = Math::Lerp(m_yaw, -xoffset, attenRate * in_deltaTime);

		// ピッチの最大・最小角度を調整
		if (m_offset.z + m_pitch > pitchMaxDegree)
		{
			m_pitch = 0.0f;
		}
		if (m_offset.z + m_pitch < pitchMinDegree)
		{
			m_pitch = 0.0f;
		}

		// 前フレームのマウス座標を更新
		if (m_mousePos.x != 1919.0f && m_mousePos.x > 0.0f)
		{
			m_frameMousePos.x = m_mousePos.x;
		}
		if (m_mousePos.y != 1079.0f && m_mousePos.y != 0.0f)
		{
			m_frameMousePos.y = m_mousePos.y;
		}
	}





}

// 当たり判定処理
void ThirdPersonCamera::CollisionFix(BoxCollider* in_hitCameraBox, BoxCollider* in_hitBox)
{
	// 当たったメッシュを非表示
	//in_hitBox->GetOwner()->GetMeshComponent()->SetVisible(false);

	Vector3 fix;


	//壁とぶつかったとき
	AABB bgBox = in_hitBox->GetWorldBox();
	AABB playerBox = m_hitBox->GetWorldBox();

	// めり込みを修正
	CalcCollisionFixVec(playerBox, bgBox, fix);

	if (m_position.x != fix.x || m_position.y != fix.y || m_position.z != fix.z)
	{
		//printf("当たった！\n");
	}

	// 補正ベクトル分戻す
	m_position += fix;
	m_cameraActor->SetPosition(m_position);
	m_cameraActor->ComputeWorldTransform();


	// 位置が変わったのでボックス再計算
	m_hitBox->OnUpdateWorldTransform();
}

// カメラ距離のセッター (最大・最小値を超えてしまった場合には調整を加える)
void ThirdPersonCamera::SetDistance(float in_dist)
{

	m_distance = in_dist;

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

// オーナーの後方から一定距離にカメラ位置を調整し、その座標を返す
const Vector3& ThirdPersonCamera::ComputeCameraPos() const
{
	// オーナーアクターへの座標を取得
	Vector3 cameraPos = m_owner->GetPosition();

	// 前進ベクトルの取得
	Vector3 forwardVec = m_owner->GetForward();

	// 前進ベクトルの反対側に、一定の距離・高さを保った状態に更新
	forwardVec = (DEFAULT_DISTANCE_OFFSET.x - 100.0f) * forwardVec;
	forwardVec.z = DEFAULT_DISTANCE_OFFSET.z + 50.0f;

	// 座標の更新
	cameraPos += forwardVec;

	return cameraPos;
}
