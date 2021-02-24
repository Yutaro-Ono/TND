#include "MoveComponentCar.h"
#include "InputController.h"
#include "Input.h"
#include "Actor.h"
#include "PlayerCar.h"

const float MoveComponentCar::HAND_BRAKE_VALUE = 8.0f;            // ハンドブレーキ操作時のブレーキ値
const float MoveComponentCar::ACCEL_LIMIT = 150.0f;                // 最大速度の上限値(現在の基準：150km/h)
const float MoveComponentCar::BRAKE_LIMIT = 35.0f;

// コンストラクタ
MoveComponentCar::MoveComponentCar(PlayerCar* in_owner)
	:MoveComponent(in_owner)
	,m_accelValue(0.0f)
	,m_brakeValue(0.0f)
	,m_handBrake(0.0f)
	,m_accelLimit(ACCEL_LIMIT)
	,m_brakeLimit(BRAKE_LIMIT)
	,m_radian(0.0f)
	,m_pastRadian(m_radian)
{
	m_playerCar = in_owner;
}

// デストラクタ
MoveComponentCar::~MoveComponentCar()
{
}

void MoveComponentCar::Update(float in_deltaTime)
{
	if (m_isActive)
	{
		if (m_padIsActive)
		{
			MovementByController(in_deltaTime);
		}
		else
		{
			MovementByKeyboard(in_deltaTime);
		}
	}

	// 重力処理
	// m_owner->SetPosition(m_owner->GetPosition() - Vector3(0.0f, 0.0f, 1.0f));
	if (m_owner->GetPosition().z <= -20.0f)
	{
		m_owner->SetPosition(m_owner->GetPosition() + Vector3(0.0f, 0.0f, 20.0f));
	}
}


void MoveComponentCar::MovementByController(float in_deltaTime)
{
	//キャラ入力
	const float speed = 2.0f;
	Vector3 forwardVec = Vector3(60.0f, 0.0f, 0.0f);
	Vector3 rightVec = Vector3(0.0f, 1.0f, 0.0f);
	Vector3 charaForwardVec = m_owner->GetForward();

	// キャラクター移動
	Vector3 DirVec(0.0f, 0.0f, 0.0f);

	// アクセル限界値
	if (m_accelValue > ACCEL_LIMIT * m_playerCar->GetFrictionVal())
	{
		m_accelValue = ACCEL_LIMIT * m_playerCar->GetFrictionVal();
	}
	//---------------------------------------------------------------------------------------------+
	// プレイヤーの操作
	//---------------------------------------------------------------------------------------------+
	// コントローラー接続時と未接続時で処理を分ける
			// 右トリガー、左トリガー取得
	float triggerR = CONTROLLER_INSTANCE.GetAxisValue(SDL_CONTROLLER_AXIS_TRIGGERRIGHT);
	float triggerL = CONTROLLER_INSTANCE.GetAxisValue(SDL_CONTROLLER_AXIS_TRIGGERLEFT);
	// 左スティック入力値を取得
	Vector2 axisL = CONTROLLER_INSTANCE.GetLAxisVec();

	// トリガー入力値を表示
	// printf("R_TRIGGER : %f | L_TRIGGER : %f \n", triggerR, triggerL);

	//------------------------------------------------------------------------------+
	// 前進
	//------------------------------------------------------------------------------+
	if (triggerR != 0.0f)
	{
		// アクセル状態にする
		m_playerCar->SetDriveState(PlayerCar::DRIVE_STATE::DRIVE_ACCEL);

		// アクセルの最大値を上回っていなければ更新し、上回っていたら最大値を正規化
		if (m_accelValue < m_accelLimit)
		{
			//m_accelValue += speed * (triggerR * in_deltaTime);
			m_accelValue += speed * triggerR;
		}
		else
		{
			m_accelValue = m_accelLimit;
		}
	}
	else if (m_accelValue != 0.0f)       // アクセルが掛かっている時も更新
	{
		m_accelValue -= (m_accelValue / 1.5f) * in_deltaTime;
	}

	//---------------------------------------------------------------------------------+
	// 後退
	//---------------------------------------------------------------------------------+
	if (triggerL != 0.0f)
	{
		// 移動状態をブレーキにする
		m_playerCar->SetDriveState(PlayerCar::DRIVE_STATE::DRIVE_BRAKE);

		// ブレーキの最大値を上回っていなければ更新し、上回っていれば最大値を正規化
		if (m_brakeValue < m_brakeLimit)
		{
			//m_brakeValue += speed * (triggerL * in_deltaTime);
			m_brakeValue += speed * triggerL;
		}
		else
		{
			m_brakeValue = m_brakeLimit;
		}
	}
	else if (m_brakeValue != 0.0f)
	{
		m_brakeValue -= (m_brakeValue / 1.5f) * in_deltaTime;
	}

	//-----------------------------------------------------------------------------------+
	// 加速度を0に戻す処理
	//-----------------------------------------------------------------------------------+
	// アクセル・ブレーキが一定値以下なら0に戻す
	if (m_accelValue < 0.2f)
	{
		m_accelValue = 0.0f;
	}
	if (m_brakeValue < 0.2f)
	{
		m_brakeValue = 0.0f;
	}
	// アクセル・ブレーキ両方とも0ならIDLE状態に
	if (m_accelValue == 0.0f && m_brakeValue == 0.0f)
	{
		m_playerCar->SetDriveState(PlayerCar::DRIVE_STATE::DRIVE_IDLE);
	}

	//----------------------------------------------------------------------------------+
	// アクセル・ブレーキの相殺処理
	//----------------------------------------------------------------------------------+
	// アクセルの値が一定以下かつアクセルとブレーキ両方入力で相殺
	if (triggerR == 1.0f && triggerL == 1.0f)
	{
		if (m_accelValue >= m_brakeValue)
		{
			m_accelValue -= m_accelValue / 10.0f;
		}
		else
		{
			m_playerCar->SetDriveState(PlayerCar::DRIVE_STATE::DRIVE_IDLE);
			m_accelValue = m_brakeValue;
		}
	}

	// アクセル・バックの速度を表示 (デバッグ)
	// printf("ACCEL_SPEED : %f | BACK_SPEED : %f \n", m_accel, m_brake);

	// ハンドブレーキ処理(コントローラーAボタン押下で発動)
	if (CONTROLLER_INSTANCE.IsPressed(SDL_CONTROLLER_BUTTON_A) && m_accelValue > 5.0f)
	{
		m_handBrake = HAND_BRAKE_VALUE;
		m_accelValue -= m_handBrake;
	}
	else
	{
		m_handBrake = 0;
	}

	// 前進値を更新
	DirVec.x += forwardVec.x * (m_accelValue + -m_brakeValue) * in_deltaTime;

	// 左右の回転、移動(アクセルが前後どちらかにかかっている時)
	if (m_accelValue >= 5.0f || m_brakeValue >= 5.0f)
	{
		// アクセル時
		if (m_playerCar->GetDriveState() == PlayerCar::DRIVE_STATE::DRIVE_ACCEL)
		{
			// 左右回転
			DirVec.y += rightVec.y * axisL.x * (m_accelValue / m_accelLimit) * (m_accelLimit / 5.0f) * in_deltaTime;
			// Z軸回転
			//m_radian += axisL.x * (m_accelValue / m_accelLimit) * (m_accelLimit / 150.0f) * in_deltaTime;
			m_radian += axisL.x * (m_accelValue / m_accelLimit) * 0.5f * in_deltaTime;

		}
		// バック時
		if (m_playerCar->GetDriveState() == PlayerCar::DRIVE_STATE::DRIVE_BRAKE)
		{
			// 左右回転
			DirVec.y -= rightVec.y * -axisL.x * (m_brakeValue / m_brakeLimit) * 0.5f * in_deltaTime;
			// Z軸回転
			m_radian -= -axisL.x * (m_brakeValue / m_brakeLimit) * 0.5f * in_deltaTime;
		}

		// 相殺時
		if (m_accelValue >= 1.0f && m_brakeValue >= 1.0f)
		{
			// 左右回転
			DirVec.y += rightVec.y * axisL.x * 0.5f * in_deltaTime;
			// Z軸回転
			m_radian += axisL.x * 0.5f * in_deltaTime;
		}

	}


	// 前進または後退時のみ前進ベクトルを更新
	if (DirVec.x > 0.5f || DirVec.x < -0.5f)
	{
		// 方向キー入力の値を前進ベクトルにセット
		charaForwardVec = DirVec;
	}
	else
	{
		// 前進ベクトルを更新させない
		charaForwardVec = Vector3(0, 0, 0);
	}

	// 前フレームのラジアンと現在のラジアンを比較し、プレイヤーがどちらに旋回しようとしているかを更新
	if (m_radian < m_pastRadian)
	{
		m_playerCar->SetTurnState(PlayerCar::TURNING_STATE::TURN_LEFT);
	}
	if (m_radian > m_pastRadian)
	{
		m_playerCar->SetTurnState(PlayerCar::TURNING_STATE::TURN_RIGHT);
	}
	if (m_radian == m_pastRadian)
	{
		m_playerCar->SetTurnState(PlayerCar::TURNING_STATE::TURN_IDLE);
	}
	// ラジアンを更新
	m_pastRadian = m_radian;


	// 現在のスピードを保存
	m_forwardSpeed = speed * in_deltaTime;

	// Z軸の回転を更新
	Quaternion rotation = Quaternion::Quaternion(Vector3::UnitZ, m_radian);
	m_owner->SetRotation(rotation);
	rotation = m_owner->GetRotation();

	// Rotationから前進ベクトルを更新し、結果の座標を算出
	charaForwardVec = Vector3::Transform(charaForwardVec, rotation);
	Vector3 resultPos = m_owner->GetPosition();
	resultPos += m_forwardSpeed * charaForwardVec;

	// オーナーへ渡す
	m_owner->SetPosition(resultPos);
}

void MoveComponentCar::MovementByKeyboard(float in_deltaTime)
{

	//キャラ入力
	const float speed = 5.0f;
	Vector3 forwardVec = Vector3(60.0f, 0.0f, 0.0f);
	Vector3 rightVec = Vector3(0.0f, 1.0f, 0.0f);
	Vector3 charaForwardVec = m_owner->GetForward();

	// キャラクター移動
	Vector3 DirVec(0.0f, 0.0f, 0.0f);

	//------------------------------------------------------------------------------------------------+
	// キーボード操作
	//------------------------------------------------------------------------------------------------+
	// 右トリガー、左トリガー取得
	float triggerR = 0.0f;
	float triggerL = 0.0f;
	// 左スティック入力値を取得
	float vertAxis = 0.0f;

	// キー入力取得(アクセル)
	if (INPUT_INSTANCE.IsKeyPressed(SDL_SCANCODE_W) || INPUT_INSTANCE.IsKeyPressed(SDL_SCANCODE_UP))
	{
		triggerR = 1.0f;
	}
	else
	{
		triggerR = 0.0f;
	}

	// キー入力取得(バック)
	if (INPUT_INSTANCE.IsKeyPressed(SDL_SCANCODE_S) || INPUT_INSTANCE.IsKeyPressed(SDL_SCANCODE_DOWN))
	{
		triggerL = 1.0f;
	}
	else
	{
		triggerL = 0.0f;
	}

	// キー入力取得(左右移動)
	if (INPUT_INSTANCE.IsKeyPressed(SDL_SCANCODE_A) || INPUT_INSTANCE.IsKeyPressed(SDL_SCANCODE_LEFT))
	{
		vertAxis = -1.0f;
	}
	else if (INPUT_INSTANCE.IsKeyPressed(SDL_SCANCODE_D) || INPUT_INSTANCE.IsKeyPressed(SDL_SCANCODE_RIGHT))
	{
		vertAxis = 1.0f;
	}
	else
	{
		vertAxis = 0.0f;
	}

	// トリガー入力値を表示
	// printf("R_TRIGGER : %f | L_TRIGGER : %f \n", triggerR, triggerL);

	//------------------------------------------------------------------------------+
	// 前進
	//------------------------------------------------------------------------------+
	if (triggerR != 0.0f)
	{
		// アクセル状態にする
		m_playerCar->SetDriveState(PlayerCar::DRIVE_STATE::DRIVE_ACCEL);

		// アクセルの最大値を上回っていなければ更新し、上回っていたら最大値を正規化
		if (m_accelValue < m_accelLimit)
		{
			m_accelValue += speed * (triggerR * in_deltaTime);

		}
		else
		{
			m_accelValue = m_accelLimit;
		}
	}
	else if (m_accelValue != 0.0f)       // アクセルが掛かっている時も更新
	{
		m_accelValue -= (m_accelValue / 1.5f) * in_deltaTime;
	}

	//---------------------------------------------------------------------------------+
	// 後退
	//---------------------------------------------------------------------------------+
	if (triggerL != 0.0f)
	{
		// アクセル状態にする
		m_playerCar->SetDriveState(PlayerCar::DRIVE_STATE::DRIVE_BRAKE);

		// ブレーキの最大値を上回っていなければ更新し、上回っていれば最大値を正規化
		if (m_brakeValue < m_brakeLimit)
		{
			m_brakeValue += 8.0f * (triggerL * in_deltaTime);
		}
		else
		{
			m_brakeValue = m_brakeLimit;
		}

	}
	else if (m_brakeValue != 0.0f)
	{
		m_brakeValue -= (m_brakeValue / 1.5f) * in_deltaTime;
	}


	//-----------------------------------------------------------------------------------+
	// 加速度を0に戻す処理
	//-----------------------------------------------------------------------------------+
	// アクセル・ブレーキが一定値以下なら0に戻す
	if (m_accelValue < 0.2f)
	{
		m_accelValue = 0.0f;
	}
	if (m_brakeValue < 0.2f)
	{
		m_brakeValue = 0.0f;
	}
	// アクセル・ブレーキ両方とも0ならIDLE状態に
	if (m_accelValue == 0.0f && m_brakeValue == 0.0f)
	{
		m_playerCar->SetDriveState(PlayerCar::DRIVE_STATE::DRIVE_IDLE);
	}

	//----------------------------------------------------------------------------------+
	// アクセル・ブレーキの相殺処理
	//----------------------------------------------------------------------------------+
	// アクセルの値が一定以下かつアクセルとブレーキ両方入力で相殺
	if (triggerR == 1.0f && triggerL == 1.0f)
	{
		if (m_accelValue >= m_brakeValue)
		{
			m_accelValue -= m_accelValue / 10.0f;
		}
		else
		{
			m_playerCar->SetDriveState(PlayerCar::DRIVE_STATE::DRIVE_IDLE);
			m_accelValue = m_brakeValue;
		}
	}

	// アクセル・バックの速度を表示 (デバッグ)
	// printf("ACCEL_SPEED : %f | BACK_SPEED : %f \n", m_accel, m_brake);

	// 前進値を更新
	DirVec.x += forwardVec.x * (m_accelValue + -m_brakeValue) * in_deltaTime;


	// 左右の回転、移動(アクセルが前後どちらかにかかっている時)
	if (m_accelValue != 0.0f || m_brakeValue != 0.0f)
	{
		// アクセル時
		if (m_playerCar->GetDriveState() == PlayerCar::DRIVE_STATE::DRIVE_ACCEL)
		{
			// 左右回転
			DirVec.y += rightVec.y * vertAxis * (m_accelValue / m_accelLimit) * (m_accelLimit / 5.0f) * in_deltaTime;
			// Z軸回転
			m_radian += vertAxis * (m_accelValue / m_accelLimit) * (m_accelLimit / 60.0f) * in_deltaTime;
		}
		// バック時
		if (m_playerCar->GetDriveState() == PlayerCar::DRIVE_STATE::DRIVE_BRAKE)
		{
			// 左右回転
			DirVec.y += rightVec.y * vertAxis * (m_brakeValue / m_brakeLimit) * 0.5f * in_deltaTime;
			// Z軸回転
			m_radian += vertAxis * (m_brakeValue / m_brakeLimit) * 0.5f * in_deltaTime;
		}

		// 相殺時
		if (m_accelValue >= 1.0f && m_brakeValue >= 1.0f)
		{
			// 左右回転
			DirVec.y += rightVec.y * vertAxis * 0.5f * in_deltaTime;
			// Z軸回転
			m_radian += vertAxis * 0.5f * in_deltaTime;
		}

	}

	// 前フレームのラジアンと現在のラジアンを比較し、プレイヤーがどちらに旋回しようとしているかを更新
	if (m_radian < m_pastRadian)
	{
		m_playerCar->SetTurnState(PlayerCar::TURNING_STATE::TURN_LEFT);
	}
	if (m_radian > m_pastRadian)
	{
		m_playerCar->SetTurnState(PlayerCar::TURNING_STATE::TURN_RIGHT);
	}
	if (m_radian == m_pastRadian)
	{
		m_playerCar->SetTurnState(PlayerCar::TURNING_STATE::TURN_IDLE);
	}
	// ラジアンを更新
	m_pastRadian = m_radian;


	// 前進または後退時のみ前進ベクトルを更新
	if (DirVec.x > 0.5f || DirVec.x < -0.5f)
	{
		// 方向キー入力の値を前進ベクトルにセット
		charaForwardVec = DirVec;

	}
	else
	{

		charaForwardVec = Vector3(0, 0, 0);
	}

	// 現在のスピードを保存
	m_forwardSpeed = speed * in_deltaTime;

	// Z軸の回転を更新
	Quaternion rotation = Quaternion::Quaternion(Vector3::UnitZ, m_radian);
	m_owner->SetRotation(rotation);
	rotation = m_owner->GetRotation();

	// Rotationから前進ベクトルを更新し、結果の座標を算出
	charaForwardVec = Vector3::Transform(charaForwardVec, rotation);
	Vector3 resultPos = m_owner->GetPosition();
	resultPos += m_forwardSpeed * charaForwardVec * m_playerCar->GetFrictionVal();

	// オーナーへ渡す
	m_owner->SetPosition(resultPos);

}
