#include "PlayerHuman.h"
#include "PlayerManager.h"
#include "GameMain.h"
#include "Renderer.h"
#include "Mesh.h"
#include "Skeleton.h"
#include "SkeletalMeshComponent.h"
#include "HumanCamera.h"
#include "PhysicsWorld.h"
#include "BoxCollider.h"
#include "Collision.h"
#include "Input.h"
#include "InputController.h"

const float cAnimationSpeed = 0.5f;

PlayerHuman::PlayerHuman(class PlayerManager* in_manager)
	:m_manager(in_manager)
	,m_jumpVec(Vector3(0.0f, 0.0f, 0.0f))
{


	// カメラの生成
	m_camera = new HumanCamera(this);

	// スケルタルメッシュのロード
	Mesh* mesh = GAME_INSTANCE.GetRenderer()->GetMesh("Data/Meshes/TND/Actors/Player/rp_eric_rigged_001_ue4.gpmesh");
	m_skelMeshComp = new SkeletalMeshComponent(this);
	m_skelMeshComp->SetMesh(mesh);
	m_skelMeshComp->SetSkeleton(RENDERER->GetSkeleton("Data/Meshes/TND/Actors/Player/rp_eric_rigged_001_ue4.gpskel"));

	// アニメーション取得
	m_animTypes.resize(PLAYER_ANIM::ANIM_ALL_NUM);
	// 対応するアニメーション配列にセット
	m_animTypes[ANIM_IDLE] = RENDERER->GetAnimation("Data/Animation/Player/ThirdPersonIdle.gpanim", true);
	m_animTypes[ANIM_WALKING] = RENDERER->GetAnimation("Data/Animation/Player/ThirdPersonIdle.gpanim", true);
	m_animTypes[ANIM_RUNNING] = RENDERER->GetAnimation("Data/Animation/Player/ThirdPersonRun.gpanim", true);
	m_animTypes[ANIM_JUMPSTART] = RENDERER->GetAnimation("Data/Animation/Player/ThirdPersonJump_Start.gpanim", true);
	m_animTypes[ANIM_JUMPLOOP] = RENDERER->GetAnimation("Data/Animation/Player/ThirdPersonJump_Loop.gpanim", true);
	m_animTypes[ANIM_LANDING] = RENDERER->GetAnimation("Data/Animation/Player/ThirdPersonJump_End.gpanim", true);

	// アイドル時のアニメーションをセットしておく
	m_skelMeshComp->PlayAnimation(m_animTypes[ANIM_RUNNING], cAnimationSpeed);


	// 当たり判定ボックスのセット
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

	// プレーヤーの頭上を調べるボックスを作成 ボックス底面が頭上に来るようにする
	AABB headBox;
	headBox = groundBox;
	headBox.m_min.z = playerBox.m_max.z;
	headBox.m_max.z = headBox.m_min.z + 2.0f;
	m_hitHeadBox = new BoxCollider(this, PhysicsWorld::TYPE_PLAYER);
	m_hitHeadBox->SetObjectBox(headBox);

}

PlayerHuman::~PlayerHuman()
{
	m_animTypes.clear();
}

void PlayerHuman::UpdateActor(float in_deltaTime)
{
	if (m_manager->GetPlayerMode() == PlayerManager::MODE_HUMAN)
	{
		//キャラ状態遷移
		ChangeState();

		// ジャンプ終了時はキー入力受け付けない
		if (m_animState == ANIM_LANDING)
		{
			return;
		}

		//キャラ入力
		const float speed = 200.0f;
		Vector3 forwardVec;
		Vector3 rightVec = Vector3(1.0f, 0.0f, 0.0f);
		Vector3 charaForwardVec = GetForward();

		// キャラクター移動
		Vector3 DirVec(0.0f, 0.0f, 0.0f);

		if (INPUT_INSTANCE.IsKeyPressed(SDL_SCANCODE_RIGHT))
		{
			DirVec = rightVec;
		}

		if (INPUT_INSTANCE.IsKeyPressed(SDL_SCANCODE_LEFT))
		{
			DirVec = -1.0f * rightVec;
		}

		//ジャンプ
		if (INPUT_INSTANCE.IsKeyPushDown(SDL_SCANCODE_SPACE) && !m_isJump)
		{
			m_jumpVec.z += 320.0f * in_deltaTime;
			m_isJump = true;
			m_skelMeshComp->PlayAnimation(m_animTypes[ANIM_JUMPSTART], cAnimationSpeed);
			m_animState = ANIM_JUMPSTART;
		}

		if (m_isJump)
		{
			m_jumpVec.z -= 1.0f * in_deltaTime;
		}

		if (DirVec.LengthSq() > 0.5f)
		{
			// 方向キー入力
			charaForwardVec = DirVec;

			// 進行方向に向けて回転
			charaForwardVec.Normalize();
			RotateToNewForward(charaForwardVec);

			// 現在のスピードを保存
			m_speed = speed * in_deltaTime;
		}
		else
		{
			charaForwardVec = Vector3(0, 0, 0);
		}

		// 進行方向に移動
		m_position += m_speed * charaForwardVec + m_jumpVec;
		m_recomputeWorldTransform = true;

		// 当たり判定処理で地面フラグをfalseにし、この後の当たり判定処理でtrueになるかチェックする
		m_onGround = false;
	}

	else
	{
		// アイドル時のアニメーションをセット
		//m_skelMeshComp->PlayAnimation(m_animTypes[ANIM_IDLE], cAnimationSpeed);
	}
}

void PlayerHuman::ChangeState()
{
	//前回の当たり判定処理で下に地面がなかったらジャンプ中へ移行する
	if (!m_isJump && !m_onGround && m_position.z > 0.0)
	{
		m_isJump = true;
		if (m_animState != ANIM_JUMPLOOP)
		{
			m_skelMeshComp->PlayAnimation(m_animTypes[ANIM_JUMPLOOP], cAnimationSpeed);
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
				m_skelMeshComp->PlayAnimation(m_animTypes[ANIM_JUMPLOOP], cAnimationSpeed);
				m_animState = ANIM_JUMPLOOP;
			}
		}
		return;
	}

	// キー入力からアニメーション状態へ移行
	bool IsIdleStart = INPUT_INSTANCE.IsKeyOff(SDL_SCANCODE_UP) &
		INPUT_INSTANCE.IsKeyOff(SDL_SCANCODE_RIGHT) &
		INPUT_INSTANCE.IsKeyOff(SDL_SCANCODE_DOWN) &
		INPUT_INSTANCE.IsKeyOff(SDL_SCANCODE_LEFT);

	// 待機アニメ開始
	if (IsIdleStart)
	{
		if (m_animState == ANIM_LANDING)
		{
			if (!m_skelMeshComp->IsPlaying())
			{
				m_skelMeshComp->PlayAnimation(m_animTypes[ANIM_IDLE], cAnimationSpeed);
				m_animState = ANIM_IDLE;
			}
		}
		else if ((m_animState != ANIM_IDLE))
		{
			m_skelMeshComp->PlayAnimation(m_animTypes[ANIM_IDLE], cAnimationSpeed);
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
				m_skelMeshComp->PlayAnimation(m_animTypes[ANIM_RUNNING], cAnimationSpeed);
				m_animState = ANIM_RUNNING;
			}
		}

		// RUNアニメ開始
		if (m_animState != ANIM_RUNNING)
		{
			m_skelMeshComp->PlayAnimation(m_animTypes[ANIM_RUNNING], cAnimationSpeed);
			m_animState = ANIM_RUNNING;
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
