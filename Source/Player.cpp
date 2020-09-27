//-----------------------------------------------------------------------+
// プレイヤークラス
// 
// 
// copyright (C) 2019 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#include "Player.h"
#include "GameMain.h"
#include "Input.h"
#include "InputController.h"
#include "Renderer.h"
#include "Mesh.h"
#include "MeshComponent.h"
#include "MoveComponentCar.h"
#include "Collision.h"
#include "BoxCollider.h"
#include "PhysicsWorld.h"
#include "FollowCamera.h"
#include "FlameActor.h"
#include "SandSmoke.h"
#include "FlameSpark.h"
#include "AudioManager.h"


const int Player::CHAIN_LIMIT = 100;                         // チェインのリミッター(99まで)
const int Player::RANK_MAX = 12;                             // ランクの上限(12まで)
const float Player::CHAIN_TIMER = 15.0f;                     // チェインのタイマー
const float Player::HAND_BRAKE_VALUE = 30.0f;                 // ハンドブレーキ操作時のブレーキ値
const float Player::ACCEL_LIMIT_MAX = 100.0f;                // 最大速度の上限値(現在の基準：150km/h)


// コンストラクタ
Player::Player(bool in_isTitle)
	:m_animState(AnimState::IDLE)
	,m_moveState(MoveState::DRIVE_IDLE)
	,m_radian(Math::Pi / 180.0f)
	,m_accel(0.0f)
	,m_brake(0.0f)
	,m_handBrake(0.0f)
	,m_accelLimit(ACCEL_LIMIT_MAX)
	,m_brakeLimit(30.0f)
	,m_maxSpeedRank(0)
	,m_flameChain(0)
	,m_chainTimer(0.0f)
	,m_spinRadian(Math::Pi / 180.0f)
	,m_spinAngle(0.0f)
	,m_isJump(false)
	,m_onGround(true)
	,m_isTitle(in_isTitle)
{
	// メッシュのロード
	// Mesh* mesh = RENDERER->GetMesh("Data/Meshes/FC/Actors/Player/Player.gpmesh");
	Mesh* mesh = GAME_INSTANCE.GetRenderer()->GetMesh("Data/Meshes/TND/Actors/Car/Player/Body/SM_suv_parts_LOD0_body_Internal.OBJ");
	//Mesh* mesh = RENDERER->GetMesh("Data/Meshes/TANK/tank.obj");
	m_meshComp = new MeshComponent(this);
	m_meshComp->SetMesh(mesh);

	if (m_isTitle == false)
	{
		MoveComponentCar* moveComp = new MoveComponentCar(this);
	}


	// 当たり判定セット
	AABB playerBox = mesh->GetCollisionBox();
	m_hitBox = new BoxCollider(this, PhysicsWorld::TYPE_PLAYER);
	playerBox.m_min.x *= 0.6f;
	playerBox.m_min.y *= 0.6f;
	playerBox.m_max.x *= 0.6f;
	playerBox.m_max.y *= 0.6f;
	m_hitBox->SetObjectBox(playerBox);

	// プレーヤーの足元を調べるボックスを作成　ボックス高1/4, ボックス上面が原点に来るようにする
	AABB groundBox;
	groundBox = playerBox;
	groundBox.m_min.x *= 0.8f;
	groundBox.m_min.y *= 0.8f;
	groundBox.m_max.x *= 0.8f;
	groundBox.m_max.y *= 0.8f;
	groundBox.m_min.z = -2.0f;  //ジャンプ時に引っかからない高さ
	groundBox.m_max.z *= 0.0f;
	m_hitGroundBox = new BoxCollider(this, PhysicsWorld::TYPE_PLAYER);
	m_hitGroundBox->SetObjectBox(groundBox);

	// ともし火のセット
	SetTomosuFire();
	
	// 砂煙の生成
	m_smoke = new SandSmoke(this);
	
	// 火花の生成
	m_spark = new FlameSpark(this);

	// 効果音
	m_sound["Accel_Low"] = "Data/Music/SE/FC/Actors/Player/Accel/electric-fan-low1.wav";                      // アクセル時
	m_sound["Accel_Medium"] = "Data/Music/SE/FC/Actors/Player/Accel/electric-fan-middle1.wav";
	m_sound["Accel_High"] = "Data/Music/SE/FC/Actors/Player/Accel/electric-fan-high1.wav";
	m_sound["Flame"] = "Data/Music/SE/FC/Actors/Player/Flame/se_maoudamashii_element_fire12_.wav";            // ともし火の燃え上がる効果音
	m_sound["HitWall"] = "Data/Music/SE/FC/Actors/Player/Collision/punch-real1.wav";                          // 壁に衝突時
	m_sound["Add_Chain"] = "Data/Music/SE/FC/System/Chain/decision16_.wav";


	// 効果音ファイルの取得
	AUDIO->GetSound(m_sound["Accel_Low"]);
	AUDIO->GetSound(m_sound["Accel_Medium"]);
	AUDIO->GetSound(m_sound["Accel_High"]);
	AUDIO->GetSound(m_sound["Flame"]);
	AUDIO->GetSound(m_sound["HitWall"]);
	AUDIO->GetSound(m_sound["HitFlame"]);
	AUDIO->GetSound(m_sound["Add_Chain"]);

	printf("PlayerActor作成 id:[%5d] this : (0x%p)\n", m_ID, this);
	m_jumpVec = Vector3(0, 0, 0);
}


// デストラクタ
Player::~Player()
{
	delete m_hitBox;
	delete m_hitGroundBox;
}


// 更新処理
void Player::UpdateActor(float in_deltaTime)
{
	// スピードランクの更新
	// UpdateSpeedRank();

	// タイトルシーン
	if (m_isTitle == true)
	{
		// タイトル画面での回転処理
		SpinTitle(in_deltaTime);
	}
	else
	{
		// ゲームシーン中はプレイヤーの操作
		// UpdateMovement(in_deltaTime);
	}

	// ともし火処理
	for (int i = 0; i < m_maxSpeedRank; i++)
	{
		if (m_tomosuFire[i] == nullptr)
		{
			m_tomosuFire[i] = new FlameActor();

			m_tomosuFire[i]->SetFlameLife(0.06f);
			m_tomosuFire[i]->SetScale(30.0f);
		}

		float rad = -(i + 1) / m_tomosuFire.size() * Math::TwoPi;

		Vector3 firePos = Vector3(50.0f * Math::Cos(rad * (i + 2)), -1.0f, 50.0f * Math::Sin(rad * (i + 2)));

		if ((i + 1) % 2)
		{
			firePos.y -= 13.0f;
		}
		else
		{
			firePos.y += 15.0f;
		}

		// プレイヤーのワールド行列をともし火の座標に変換
		firePos = Vector3::Transform(firePos, GetWorldTransform());

		m_tomosuFire[i]->SetPosition(firePos);


		// printf("x : %f, y : %f, z : %f\n", m_tomosuFire[i]->GetPosition().x, m_tomosuFire[i]->GetPosition().y, m_tomosuFire[i]->GetPosition().z);

	}

	// SE再生
	if (m_isTitle == false)
	{
		if (m_maxSpeedRank < 6)
		{
			if ((m_accel >= 5.0f || m_brake >= 3.0f) && AUDIO->IsPlayingSound(m_sound["Accel_Low"]) == false)
			{
				AUDIO->PlaySound(m_sound["Accel_Low"]);
			}
			else if (m_accel < 5.0f && m_brake < 3.0f)
			{
				AUDIO->StopSound(m_sound["Accel_Low"]);
			}
		}


		if (m_maxSpeedRank > 5)
		{
			if ((m_accel >= 5.0f || m_brake >= 3.0f) && AUDIO->IsPlayingSound(m_sound["Accel_Medium"]) == false)
			{
				AUDIO->PlaySound(m_sound["Accel_Medium"]);
			}
			else if (m_accel < 5.0f && m_brake < 3.0f)
			{
				AUDIO->StopSound(m_sound["Accel_Medium"]);
			}

			if (AUDIO->IsPlayingSound(m_sound["Accel_Low"]) == true)
			{
				AUDIO->StopSound(m_sound["Accel_Low"]);
			}
		}

		if (m_maxSpeedRank > 10)
		{
			if ((m_accel >= 5.0f || m_brake >= 3.0f) && AUDIO->IsPlayingSound(m_sound["Accel_High"]) == false)
			{
				AUDIO->PlaySound(m_sound["Accel_High"]);
			}
			else if (m_accel < 5.0f && m_brake < 3.0f)
			{
				AUDIO->StopSound(m_sound["Accel_High"]);
			}

			if (AUDIO->IsPlayingSound(m_sound["Accel_Medium"]) == true)
			{
				AUDIO->StopSound(m_sound["Accel_Medium"]);
			}
		}


		// 炎SE再生
		if (m_maxSpeedRank >= 1 && AUDIO->IsPlayingSound(m_sound["Flame"]) == false)
		{
			AUDIO->PlaySound(m_sound["Flame"]);
		}
	}
	else
	{
		if (AUDIO->IsPlayingSound(m_sound["Accel_Low"]) == false)
		{
			AUDIO->PlaySound(m_sound["Accel_Low"]);
		}

		if (AUDIO->IsPlayingSound(m_sound["Flame"]) == false)
		{
			AUDIO->PlaySound(m_sound["Flame"]);
		}
	}

	// チェインタイマー更新
	if (m_chainTimer > 0.0f && m_flameChain >= 1)
	{
		m_chainTimer -= in_deltaTime;
	}

	// タイマーが0になったとき、チェイン数を減らす
	if (m_chainTimer <= 0.0f && m_flameChain >= 1)
	{
		m_flameChain--;
		m_chainTimer = CHAIN_TIMER;
	}


	// 当たり判定処理で地面フラグをfalseにし、この後の当たり判定処理でtrueになるかチェックする
	m_onGround = false;



}


// ワールド変換行列を計算
void Player::ComputeWorldTransform2()
{

	// スケーリング→回転→平行移動となるように変換行列を作成
	m_worldTransform = Matrix4::CreateScale(m_scale);

	// Z軸回転
	m_worldTransform *= Matrix4::CreateRotationZ(m_radian);

	// ポジション
	m_worldTransform *= Matrix4::CreateTranslation(m_position);


	// アクターが持っている全コンポーネントの変換を行う
	for (auto comp : m_components)
	{
		comp->OnUpdateWorldTransform();
	}

}

//-----------------------------------------------------------------------------+
// チェイン関連処理
//-----------------------------------------------------------------------------+
// チェイン数の加算
void Player::AddFlameChain()
{
	// チェインのリミットより下の時チェイン更新
	if (m_flameChain < CHAIN_LIMIT)
	{
		m_flameChain++;
	}

	// SE再生
	if (m_flameChain >= 2)
	{
		AUDIO->PlaySound(m_sound["Add_Chain"]);
	}


	// タイマーを設定
	m_chainTimer = CHAIN_TIMER;
}

// チェイン数の減算
void Player::SubFlameChain()
{
	if (m_flameChain != 0)
	{
		m_flameChain--;
	}

}


// 全てのサウンドをストップ
void Player::AllStopSound()
{
	// Soundを止める
	if (AUDIO->IsPlayingSound(m_sound["Accel_Low"]))
	{
		AUDIO->StopSound(m_sound["Accel_Low"]);
	}
	if (AUDIO->IsPlayingSound(m_sound["Accel_Medium"]))
	{
		AUDIO->StopSound(m_sound["Accel_Medium"]);
	}
	if (AUDIO->IsPlayingSound(m_sound["Accel_High"]))
	{
		AUDIO->StopSound(m_sound["Accel_High"]);
	}

	if (AUDIO->IsPlayingSound(m_sound["Flame"]))
	{
		AUDIO->StopSound(m_sound["Flame"]);
	}
}


// プレイヤーのステータス初期化(主にフリーモードで取得した分の初期化)
void Player::InitStatus()
{
	m_accel = 0.0f;
	m_brake = 0.0f;

	m_accelLimit = 10.0f;
	m_brakeLimit = 5.0f;

	for (int i = 0; i < RANK_MAX; i++)
	{
		m_tomosuFire[i]->SetState(STATE_DEAD);
		m_tomosuFire[i] = nullptr;
	}

	m_maxSpeedRank = 0;
	m_flameChain = 0;
	m_chainTimer = 0.0f;
}


// ともし火のセット
void Player::SetTomosuFire()
{
	for (int i = 0; i < 12; i++)
	{
		m_tomosuFire.push_back(new FlameActor());

		float rad = (i + 1) / m_tomosuFire.size() * Math::Pi * 2 + m_spinRadian;

		Vector3 firePos = Vector3(50.0f * Math::Cos(rad * (i + 1)), 0.0f, 50.0f * Math::Sin(rad * (i + 1)));


		if (i % 2 == 0)
		{
			firePos.y = -5.0f;
		}
		else
		{
			firePos.y = +5.0f;
		}

		firePos = Vector3::Transform(firePos, GetWorldTransform());

		m_tomosuFire[i]->SetPosition(firePos);

		m_tomosuFire[i]->SetFlameLife(0.06f);
		m_tomosuFire[i]->SetScale(30.0f);
	}
}


// タイトル用プレイヤー
void Player::SetTitlePlayer()
{
	m_accelLimit = ACCEL_LIMIT_MAX;
}


// 衝突時の押し出し処理
void Player::CollisionFix(BoxCollider * in_hitPlayerBox, BoxCollider * in_hitBox)
{
	Vector3 fix;
	//足元当たり判定がヒットしたか？
	if (in_hitPlayerBox == m_hitGroundBox && m_jumpVec.z <= 0.0f)
	{
		m_onGround = true;
		// ジャンプ中なら解除
		if (m_isJump)
		{
			m_isJump = false;
			m_jumpVec = Vector3(0, 0, -1.0f);
			m_position += m_jumpVec;
			return;
		}
	}

	//壁とぶつかったとき
	AABB bgBox = in_hitBox->GetWorldBox();
	AABB playerBox = m_hitBox->GetWorldBox();

	// めり込みを修正
	CalcCollisionFixVec(playerBox, bgBox, fix);

	// 補正ベクトル分戻す
	m_position += fix;

	// 壁衝突時の処理
	if (fix.x > 5.0f || fix.x < -5.0f || fix.y > 5.0f || fix.y < -5.0f)
	{
		// 効果音再生
		AUDIO->PlaySound(m_sound["HitWall"]);

		// チェインの終了
		// RemoveFlameChain();

		// アクセル減少
		m_accel -= m_accel / 2.0f;
	}

	// ジャンプアクション中に頭が壁に当たったとき
	if (in_hitPlayerBox == m_hitHeadBox && m_isJump)
	{
		//ジャンプ上向きに行く方向を逆転させる
		m_jumpVec = Vector3(0, 0, -1.0f);
		m_position += m_jumpVec; // もう頭上に当たらない様に離す
	}

	// 位置が変わったのでボックス再計算
	m_hitBox->OnUpdateWorldTransform();

	// printf("[%f, %f, %f]\n", m_position.x, m_position.y, m_position.z);
}

void Player::UpdateMovement(float in_deltaTime)
{
	//キャラ入力
	const float speed = 8.0f;
	Vector3 forwardVec = Vector3(60.0f, 0.0f, 0.0f);
	Vector3 rightVec = Vector3(0.0f, 1.0f, 0.0f);
	Vector3 charaForwardVec = GetForward();

	// キャラクター移動
	Vector3 DirVec(0.0f, 0.0f, 0.0f);

	// アクセル限界値
	if (m_accel > ACCEL_LIMIT_MAX)
	{
		m_accel = ACCEL_LIMIT_MAX;
	}

	//---------------------------------------------------------------------------------------------+
	// プレイヤーの操作
	//---------------------------------------------------------------------------------------------+
	// コントローラー接続時と未接続時で処理を分ける
	if (CONTROLLER_INSTANCE.IsAvailable() && m_isTitle == false)
	{
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
			SetMoveState(MoveState::DRIVE_ACCEL);

			// アクセルの最大値を上回っていなければ更新し、上回っていたら最大値を正規化
			if (m_accel < m_accelLimit)
			{
				m_accel += speed * (triggerR * in_deltaTime);
			}
			else
			{
				m_accel = m_accelLimit;
			}
		}
		else if (m_accel != 0.0f)       // アクセルが掛かっている時も更新
		{
			m_accel -= (m_accel / 1.5f) * in_deltaTime;
		}

		//---------------------------------------------------------------------------------+
		// 後退
		//---------------------------------------------------------------------------------+
		if (triggerL != 0.0f)
		{
			// 移動状態をブレーキにする
			SetMoveState(MoveState::DRIVE_BRAKE);

			// ブレーキの最大値を上回っていなければ更新し、上回っていれば最大値を正規化
			if (m_brake < m_brakeLimit)
			{
				m_brake += speed * (triggerL * in_deltaTime);
			}
			else
			{
				m_brake = m_brakeLimit;
			}
		}
		else if (m_brake != 0.0f)
		{
			m_brake -= (m_brake / 1.5f) * in_deltaTime;
		}

		//-----------------------------------------------------------------------------------+
		// 加速度を0に戻す処理
		//-----------------------------------------------------------------------------------+
		// アクセル・ブレーキが一定値以下なら0に戻す
		if (m_accel < 0.2f)
		{
			m_accel = 0.0f;
		}
		if (m_brake < 0.2f)
		{
			m_brake = 0.0f;
		}
		// アクセル・ブレーキ両方とも0ならIDLE状態に
		if (m_accel == 0.0f && m_brake == 0.0f)
		{
			SetMoveState(MoveState::DRIVE_IDLE);
		}

		//----------------------------------------------------------------------------------+
		// アクセル・ブレーキの相殺処理
		//----------------------------------------------------------------------------------+
		// アクセルの値が一定以下かつアクセルとブレーキ両方入力で相殺
		if (triggerR == 1.0f && triggerL == 1.0f)
		{
			if (m_accel >= m_brake)
			{
				m_accel -= m_accel / 10.0f;
			}
			else
			{
				SetMoveState(MoveState::DRIVE_IDLE);
				m_accel = m_brake;
			}
		}

		// アクセル・バックの速度を表示 (デバッグ)
		// printf("ACCEL_SPEED : %f | BACK_SPEED : %f \n", m_accel, m_brake);

		// ハンドブレーキ処理(コントローラーAボタン押下で発動)
		if (CONTROLLER_INSTANCE.IsPressed(SDL_CONTROLLER_BUTTON_A) && m_accel > 5.0f)
		{
			m_handBrake = HAND_BRAKE_VALUE;
			m_accel -= m_handBrake;
		}
		else
		{
			m_handBrake = 0;
		}

		// 前進値を更新
		DirVec.x += forwardVec.x * (m_accel + -m_brake) * in_deltaTime;

		// 左右の回転、移動(アクセルが前後どちらかにかかっている時)
		if (m_accel != 0.0f || m_brake != 0.0f)
		{
			// アクセル時
			if (m_moveState == MoveState::DRIVE_ACCEL)
			{
				// 左右回転
				DirVec.y += rightVec.y * axisL.x * (m_accel / m_accelLimit) * (m_accelLimit / 5.0f) * in_deltaTime;
				// Z軸回転
				m_radian += axisL.x * (m_accel / m_accelLimit) * (m_accelLimit / 60.0f) * in_deltaTime;
			}
			// バック時
			if (m_moveState == MoveState::DRIVE_BRAKE)
			{
				// 左右回転
				DirVec.y += rightVec.y * axisL.x * (m_brake / m_brakeLimit) * 0.5f * in_deltaTime;
				// Z軸回転
				m_radian += axisL.x * (m_brake / m_brakeLimit) * 0.5f * in_deltaTime;
			}

			// 相殺時
			if (m_accel >= 1.0f && m_brake >= 1.0f)
			{
				// 左右回転
				DirVec.y += rightVec.y * axisL.x * 0.5f * in_deltaTime;
				// Z軸回転
				m_radian += axisL.x * 0.5f * in_deltaTime;
			}

		}


		// Y軸の回転値
		float spinValue = 6.5f * (m_accel / ACCEL_LIMIT_MAX) * in_deltaTime;

		// Y軸回転
		// アクセルとブレーキが同時にかかっていない場合のみ
		if (m_moveState != MoveState::DRIVE_IDLE)
		{
			// アクセル時は前方に回転
			if (m_moveState == MoveState::DRIVE_ACCEL)
			{
				// m_spinRadian += 6.5f * (m_accel / m_accelLimit) * in_deltaTime;
				m_spinRadian += spinValue;
			}

			// ブレーキ時は後方に回転
			if (m_moveState == MoveState::DRIVE_BRAKE)
			{
				m_spinRadian -= (m_brake / m_brakeLimit) * in_deltaTime;
			}
		}

	}
	//------------------------------------------------------------------------------------------------+
	// キーボード操作
	//------------------------------------------------------------------------------------------------+
	else if (m_isTitle == false)
	{
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
			SetMoveState(MoveState::DRIVE_ACCEL);

			// アクセルの最大値を上回っていなければ更新し、上回っていたら最大値を正規化
			if (m_accel < m_accelLimit)
			{
				m_accel += speed * (triggerR * in_deltaTime);

			}
			else
			{
				m_accel = m_accelLimit;
			}
		}
		else if (m_accel != 0.0f)       // アクセルが掛かっている時も更新
		{
			m_accel -= (m_accel / 1.5f) * in_deltaTime;
		}

		//---------------------------------------------------------------------------------+
		// 後退
		//---------------------------------------------------------------------------------+
		if (triggerL != 0.0f)
		{
			// 移動状態をブレーキにする
			SetMoveState(MoveState::DRIVE_BRAKE);

			// ブレーキの最大値を上回っていなければ更新し、上回っていれば最大値を正規化
			if (m_brake < m_brakeLimit)
			{
				m_brake += 8.0f * (triggerL * in_deltaTime);
			}
			else
			{
				m_brake = m_brakeLimit;
			}

		}
		else if (m_brake != 0.0f)
		{
			m_brake -= (m_brake / 1.5f) * in_deltaTime;
		}


		//-----------------------------------------------------------------------------------+
		// 加速度を0に戻す処理
		//-----------------------------------------------------------------------------------+
		// アクセル・ブレーキが一定値以下なら0に戻す
		if (m_accel < 0.2f)
		{
			m_accel = 0.0f;
		}
		if (m_brake < 0.2f)
		{
			m_brake = 0.0f;
		}
		// アクセル・ブレーキ両方とも0ならIDLE状態に
		if (m_accel == 0.0f && m_brake == 0.0f)
		{
			SetMoveState(MoveState::DRIVE_IDLE);
		}

		//----------------------------------------------------------------------------------+
		// アクセル・ブレーキの相殺処理
		//----------------------------------------------------------------------------------+
		// アクセルの値が一定以下かつアクセルとブレーキ両方入力で相殺
		if (triggerR == 1.0f && triggerL == 1.0f)
		{
			if (m_accel >= m_brake)
			{
				m_accel -= m_accel / 10.0f;
			}
			else
			{
				SetMoveState(MoveState::DRIVE_IDLE);
				m_accel = m_brake;
			}
		}

		// アクセル・バックの速度を表示 (デバッグ)
		// printf("ACCEL_SPEED : %f | BACK_SPEED : %f \n", m_accel, m_brake);

		// 前進値を更新
		DirVec.x += forwardVec.x * (m_accel + -m_brake) * in_deltaTime;


		// 左右の回転、移動(アクセルが前後どちらかにかかっている時)
		if (m_accel != 0.0f || m_brake != 0.0f)
		{
			// アクセル時
			if (m_moveState == MoveState::DRIVE_ACCEL)
			{
				// 左右回転
				DirVec.y += rightVec.y * vertAxis * (m_accel / m_accelLimit) * (m_accelLimit / 5.0f) * in_deltaTime;
				// Z軸回転
				m_radian += vertAxis * (m_accel / m_accelLimit) * (m_accelLimit / 60.0f) * in_deltaTime;
			}
			// バック時
			if (m_moveState == MoveState::DRIVE_BRAKE)
			{
				// 左右回転
				DirVec.y += rightVec.y * vertAxis * (m_brake / m_brakeLimit) * 0.5f * in_deltaTime;
				// Z軸回転
				m_radian += vertAxis * (m_brake / m_brakeLimit) * 0.5f * in_deltaTime;
			}

			// 相殺時
			if (m_accel >= 1.0f && m_brake >= 1.0f)
			{
				// 左右回転
				DirVec.y += rightVec.y * vertAxis * 0.5f * in_deltaTime;
				// Z軸回転
				m_radian += vertAxis * 0.5f * in_deltaTime;
			}

		}

		// Y軸の回転値
		float spinValue = 6.5f * (m_accel / ACCEL_LIMIT_MAX) * in_deltaTime;

		// Y軸回転
		// アクセルとブレーキが同時にかかっていない場合のみ
		if (m_moveState != MoveState::DRIVE_IDLE)
		{
			// アクセル時は前方に回転
			if (m_moveState == MoveState::DRIVE_ACCEL)
			{
				// m_spinRadian += 6.5f * (m_accel / m_accelLimit) * in_deltaTime;
				m_spinRadian += spinValue;
			}

			// ブレーキ時は後方に回転
			if (m_moveState == MoveState::DRIVE_BRAKE)
			{
				m_spinRadian -= (m_brake / m_brakeLimit) * in_deltaTime;
			}
		}

	}

	// 前進または後退時のみ前進ベクトルを更新
	if (DirVec.x > 0.5f || DirVec.x < -0.5f)
	{
		// 方向キー入力の値を前進ベクトルにセット
		charaForwardVec = DirVec;

		// 状態を"スピン"に
		m_animState = AnimState::SPIN;

		// 現在のスピードを保存
		m_speed = speed * in_deltaTime;

	}
	else
	{
		// 状態を"Idle"に
		m_animState = AnimState::IDLE;

		charaForwardVec = Vector3(0, 0, 0);
	}

	// Y軸Rotation回転
	// m_rotation = Quaternion::Quaternion(Vector3::UnitY, m_spinRadian);
	// Z軸Rotation回転
	m_rotation = Quaternion::Quaternion(Vector3::UnitZ, m_radian);

	// Rotationから前進ベクトルを更新し、実際のポジションに足す
	charaForwardVec = Vector3::Transform(charaForwardVec, m_rotation);
	m_position += m_speed * charaForwardVec - Vector3(0.0f, 0.0f, 0.0f);

	// ワールド座標変換
	ComputeWorldTransform2();

}


// スピードランクの更新(全5段階)
void Player::UpdateSpeedRank()
{

	if (m_accelLimit >= ACCEL_LIMIT_MAX * 0.132f)
	{
		m_maxSpeedRank = 1;
	}

	if (m_accelLimit >= ACCEL_LIMIT_MAX * 0.2f)
	{
		m_maxSpeedRank = 2;
	}
	if (m_accelLimit >= ACCEL_LIMIT_MAX * 0.3f)
	{
		m_maxSpeedRank = 3;
	}
	if (m_accelLimit >= ACCEL_LIMIT_MAX * 0.4f)
	{
		m_maxSpeedRank = 4;
	}
	if (m_accelLimit >= ACCEL_LIMIT_MAX * 0.5f)
	{
		m_maxSpeedRank = 5;
	}
	if (m_accelLimit >= ACCEL_LIMIT_MAX * 0.6f)
	{
		m_maxSpeedRank = 6;
	}
	if (m_accelLimit >= ACCEL_LIMIT_MAX * 0.65f)
	{
		m_maxSpeedRank = 7;
	}
	if (m_accelLimit >= ACCEL_LIMIT_MAX * 0.7f)
	{
		m_maxSpeedRank = 8;
	}
	if (m_accelLimit >= ACCEL_LIMIT_MAX * 0.75f)
	{
		m_maxSpeedRank = 9;
	}

	if (m_accelLimit >= ACCEL_LIMIT_MAX * 0.85f)
	{
		m_maxSpeedRank = 10;
	}

	if (m_accelLimit >= ACCEL_LIMIT_MAX * 0.90f)
	{
		m_maxSpeedRank = 11;
	}

	if (m_accelLimit >= ACCEL_LIMIT_MAX * 0.98f)
	{
		m_maxSpeedRank = 12;
	}

}

void Player::SpinTitle(float in_deltaTime)
{
	m_spinRadian += 1.0f * in_deltaTime;
	// Y軸Rotation回転
	m_rotation.y = m_spinRadian;

	ComputeWorldTransform2();

}
