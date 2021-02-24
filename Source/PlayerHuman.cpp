#include "PlayerHuman.h"
#include "PlayerManager.h"
#include "GameMain.h"
#include "Renderer.h"
#include "Mesh.h"
#include "Skeleton.h"
#include "SkeletalMeshComponent.h"
#include "HumanCamera.h"
#include "ThirdPersonCamera.h"
#include "MoveComponentHuman.h"
#include "PhysicsWorld.h"
#include "BoxCollider.h"
#include "Collision.h"
#include "Input.h"
#include "InputController.h"
#include "PointLight.h"

const float cAnimationSpeed = 14.0f;          // アニメーションの速度

PlayerHuman::PlayerHuman(class PlayerManager* in_manager)
	:m_manager(in_manager)
	,m_pov(POV_THIRD_PERSON)
	,m_jumpVec(Vector3(0.0f, 0.0f, 0.0f))
	,m_faceLight(nullptr)
	,m_isActive(true)
{
	// カメラの生成
	m_cameraComp = new ThirdPersonCamera(this);
	m_cameraComp->SetAdjustForward(true);
	m_cameraComp->SetChaseOwnerForward(false);

	// MoveComponentの生成
	m_moveComp = new MoveComponentHuman(this);
	
	// スケルタルメッシュのロード
	Mesh* mesh = RENDERER->GetMesh("Data/Meshes/TND/Actors/Player/rp_nathan_rigged_003_ue4.gpmesh");
	m_skelMeshComp = new SkeletalMeshComponent(this);
	m_skelMeshComp->SetMesh(mesh);
	m_skelMeshComp->SetSkeleton(RENDERER->GetSkeleton("Data/Meshes/TND/Actors/Player/rp_nathan_rigged_003_ue4.gpskel"));

	// フェイスライト
	m_faceLight = new PointLight(PointLight::VL_SMALL);
	m_faceLight->SetPosition(m_position + Vector3(0.0f, 0.0f, 32.0f));
	m_faceLight->SetLightColor(Vector3(1.0f, 1.0f, 1.0f), Vector3(1.0f, 1.0f, 1.0f));
	m_faceLight->SetLuminance(1.0f);
	// アニメーション取得
	m_animTypes.resize(PLAYER_ANIM::ANIM_ALL_NUM);
	// 対応するアニメーション配列にセット
	m_animTypes[ANIM_IDLE] = RENDERER->GetAnimation("Data/Animation/Player/ThirdPersonIdle.gpanim", true);
	m_animTypes[ANIM_IDLE_LOOKAROUND] = RENDERER->GetAnimation("Data/Animation/Player/ThirdPersonIdle_Around.gpanim", true);
	m_animTypes[ANIM_WALKING_FWD] = RENDERER->GetAnimation("Data/Animation/Player/ThirdPersonWalk_Fwd.gpanim", true);
	m_animTypes[ANIM_WALKING_BWD] = RENDERER->GetAnimation("Data/Animation/Player/ThirdPersonWalk_Bwd.gpanim", true);
	m_animTypes[ANIM_WALKING_LEFT] = RENDERER->GetAnimation("Data/Animation/Player/ThirdPersonWalk_Left.gpanim", true);
	m_animTypes[ANIM_WALKING_RIGHT] = RENDERER->GetAnimation("Data/Animation/Player/ThirdPersonWalk_Right.gpanim", true);
	m_animTypes[ANIM_RUNNING] = RENDERER->GetAnimation("Data/Animation/Player/ThirdPersonRun.gpanim", true);
	m_animTypes[ANIM_JUMPSTART] = RENDERER->GetAnimation("Data/Animation/Player/ThirdPersonJump_Start.gpanim", false);
	m_animTypes[ANIM_JUMPLOOP] = RENDERER->GetAnimation("Data/Animation/Player/ThirdPersonJump_Loop.gpanim", true);
	m_animTypes[ANIM_LANDING] = RENDERER->GetAnimation("Data/Animation/Player/ThirdPersonJump_End.gpanim", false);

	// アイドル時のアニメーションをセットしておく
	m_skelMeshComp->PlayAnimation(m_animTypes[ANIM_IDLE], cAnimationSpeed * GAME_INSTANCE.GetDeltaTime());

	// 当たり判定ボックスのセット
	AABB playerBox = mesh->GetCollisionBox();
	m_hitBox = new BoxCollider(this, PhysicsWorld::TYPE_PLAYER_HUMAN);
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
	m_hitGroundBox = new BoxCollider(this, PhysicsWorld::TYPE_PLAYER_HUMAN);
	m_hitGroundBox->SetObjectBox(groundBox);

	// プレーヤーの頭上を調べるボックスを作成 ボックス底面が頭上に来るようにする
	AABB headBox;
	headBox = groundBox;
	headBox.m_min.z = playerBox.m_max.z;
	headBox.m_max.z = headBox.m_min.z + 2.0f;
	m_hitHeadBox = new BoxCollider(this, PhysicsWorld::TYPE_PLAYER_HUMAN);
	m_hitHeadBox->SetObjectBox(headBox);

}

PlayerHuman::~PlayerHuman()
{
	m_animTypes.clear();
}

void PlayerHuman::UpdateActor(float in_deltaTime)
{

	// プレイヤー操作時の更新処理
	if (m_manager->GetPlayerMode() == PlayerManager::MODE_HUMAN)
	{
		// カメラをアクティブにする
		GAME_INSTANCE.SetCamera(m_cameraComp);
		m_moveComp->SetActive(true);
		m_skelMeshComp->SetVisible(true);

		// 重力処理
		//m_position.z -= 1.0f;
		if (m_position.z < -20.0f)
		{
			m_position.z = 0.0f;
		}

		//キャラ状態遷移
		ChangeState(in_deltaTime);

		// ジャンプ終了時はキー入力受け付けない
		if (m_animState == ANIM_LANDING)
		{
			return;
		}

		m_recomputeWorldTransform = true;

		// 当たり判定処理で地面フラグをfalseにし、この後の当たり判定処理でtrueになるかチェックする
		m_onGround = false;
	}

	else
	{
		m_moveComp->SetActive(false);
		m_skelMeshComp->SetVisible(false);
		// アイドル時のアニメーションをセット
		m_skelMeshComp->PlayAnimation(m_animTypes[ANIM_IDLE], cAnimationSpeed * in_deltaTime);
	}

	// ライトの追従
	m_faceLight->SetPosition(m_position + Vector3(0.0f, 0.0f, 32.0f));

	printf("Player x : %f, y : %f, z : %f\n", m_position.x, m_position.y, m_position.z);
}

void PlayerHuman::ChangeState(float in_deltaTime)
{
	//前回の当たり判定処理で下に地面がなかったらジャンプ中へ移行する
	if (!m_isJump && !m_onGround && m_position.z > 0.0)
	{
		m_isJump = true;
		if (m_animState != ANIM_JUMPLOOP)
		{
			m_skelMeshComp->PlayAnimation(m_animTypes[ANIM_JUMPLOOP], cAnimationSpeed * in_deltaTime);
			m_animState = ANIM_JUMPLOOP;
		}
		return;
	}
	if (m_isJump)
	{
		// JUMP開始からJumpLoopへ
		if (m_animState == ANIM_JUMPSTART)
		{
			if (!(m_skelMeshComp->IsPlaying()))
			{
				m_skelMeshComp->PlayAnimation(m_animTypes[ANIM_JUMPLOOP], cAnimationSpeed * in_deltaTime);
				m_animState = ANIM_JUMPLOOP;
			}
		}
		return;
	}

	// 移動コマンドの入力値取得
	Vector2 axisL = Vector2::Zero;

	if (CONTROLLER_INSTANCE.IsAvailable())
	{
		// コントローラ接続時、左スティックの入力値を取得
		axisL = CONTROLLER_INSTANCE.GetLAxisVec();
	}
	else
	{
		// コントローラ未接続時、キーボード(WASD)の入力値を取得
		if (INPUT_INSTANCE.IsKeyPressed(SDL_SCANCODE_W))
		{
			axisL.y += -1.0f;
		}
		if (INPUT_INSTANCE.IsKeyPressed(SDL_SCANCODE_S))
		{
			axisL.y += 1.0f;
		}
		if (INPUT_INSTANCE.IsKeyPressed(SDL_SCANCODE_A))
		{
			axisL.x += -1.0f;
		}
		if (INPUT_INSTANCE.IsKeyPressed(SDL_SCANCODE_D))
		{
			axisL.x += 1.0f;
		}
	}


	// キー入力からアニメーション状態へ移行
	bool IsIdleStart = INPUT_INSTANCE.IsKeyOff(SDL_SCANCODE_W) &
		INPUT_INSTANCE.IsKeyOff(SDL_SCANCODE_D) &
		INPUT_INSTANCE.IsKeyOff(SDL_SCANCODE_S) &
		INPUT_INSTANCE.IsKeyOff(SDL_SCANCODE_A) &
		!(axisL.y != 0.0f) & !(axisL.x != 0.0f);

	// 待機アニメ開始
	if (IsIdleStart)
	{
		if (m_animState == ANIM_LANDING)
		{
			if (!m_skelMeshComp->IsPlaying())
			{
				m_skelMeshComp->PlayAnimation(m_animTypes[ANIM_IDLE], cAnimationSpeed * in_deltaTime);
				m_animState = ANIM_IDLE;
			}
		}
		else if ((m_animState != ANIM_IDLE))
		{
			m_skelMeshComp->PlayAnimation(m_animTypes[ANIM_IDLE], cAnimationSpeed * in_deltaTime);
			m_animState = ANIM_IDLE;
		}
	}
	else
	{
		// ジャンプ終了アニメからのRUN開始
		if (m_animState == ANIM_LANDING)
		{
			if (!m_skelMeshComp->IsPlaying())
			{
				m_skelMeshComp->PlayAnimation(m_animTypes[ANIM_RUNNING], cAnimationSpeed * in_deltaTime);
				m_animState = ANIM_RUNNING;
			}
		}

		//-----------------------------------------------------------------------------------+
		// 歩行アニメーション
		//-----------------------------------------------------------------------------------+
		if ((axisL.y < -0.1f || axisL.y > 0.1f) && axisL.x < 0.1f && axisL.x > -0.1f)
		{
			// 正面
			if (m_animState != ANIM_WALKING_FWD && axisL.y < -0.1f && axisL.y > -0.65f)
			{
				m_skelMeshComp->PlayAnimation(m_animTypes[ANIM_WALKING_FWD], cAnimationSpeed * in_deltaTime);
				m_animState = ANIM_WALKING_FWD;
			}
			// RUNアニメ開始
			if (m_animState != ANIM_RUNNING && axisL.y <= -0.65f)
			{
				m_skelMeshComp->PlayAnimation(m_animTypes[ANIM_RUNNING], cAnimationSpeed * in_deltaTime);
				m_animState = ANIM_RUNNING;
			}
		}
		
		// 後方
		if (m_animState != ANIM_WALKING_BWD && axisL.y > 0.1f)
		{
			m_skelMeshComp->PlayAnimation(m_animTypes[ANIM_WALKING_BWD], cAnimationSpeed * in_deltaTime);
			m_animState = ANIM_WALKING_BWD;
		}

		if ((axisL.x < -0.1f || axisL.x > 0.1f) && axisL.y <= 0.1f && axisL.y >= -0.1f)
		{
			// 左側
			if (m_animState != ANIM_WALKING_LEFT && axisL.x < -0.1f)
			{
				m_skelMeshComp->PlayAnimation(m_animTypes[ANIM_WALKING_LEFT], cAnimationSpeed * in_deltaTime);
				m_animState = ANIM_WALKING_LEFT;
			}
			// 右側
			if (m_animState != ANIM_WALKING_RIGHT && axisL.x > 0.1f)
			{
				m_skelMeshComp->PlayAnimation(m_animTypes[ANIM_WALKING_RIGHT], cAnimationSpeed * in_deltaTime);
				m_animState = ANIM_WALKING_RIGHT;
			}
		}

	}


}


void PlayerHuman::CollisionFix(BoxCollider* in_hitPlayerBox, BoxCollider* in_hitBox)
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
			if (m_animState == ANIM_JUMPLOOP)
			{
				m_skelMeshComp->PlayAnimation(m_animTypes[ANIM_LANDING], cAnimationSpeed);
				m_animState = ANIM_LANDING;
			}
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

	// ジャンプアクション中に頭が壁に当たったとき
	if (in_hitPlayerBox == m_hitHeadBox && m_isJump)
	{
		//ジャンプ上向きに行く方向を逆転させる
		m_jumpVec = Vector3(0, 0, -1.0f);
		m_position += m_jumpVec; // もう頭上に当たらない様に離す
	}

	// 位置が変わったのでボックス再計算
	m_hitBox->OnUpdateWorldTransform();
}

// プレイヤー操作の有効化
void PlayerHuman::SetActive(bool in_active)
{
	m_isActive = in_active;

	if (m_isActive)
	{
		delete m_cameraComp;
		m_cameraComp = new ThirdPersonCamera(this);
		m_cameraComp->SetAdjustForward(true);
		m_cameraComp->SetChaseOwnerForward(false);
		GAME_INSTANCE.SetCamera(m_cameraComp);
	}

}
