#include "PatrolComponent.h"
#include "GameWorld.h"
#include "PatrolPoint.h"
#include "Helicopter.h"
#include "PlayerManager.h"

// コンストラクタ
PatrolComponent::PatrolComponent(Helicopter* in_owner, GameWorld* in_world)
	:Component(in_owner)
	,m_patrolState(PATROL_STATE::PATROL)
	,m_heli(in_owner)
	,m_world(in_world)
	,m_targetPoint(nullptr)
{
	// 巡回地点を走査して巡回先の座標としてセット
	for (auto p : m_world->GetPatrolPoint())
	{
		// すでに巡回地点として登録されていなかったら
		if (!p->GetIsSelected())
		{
			// 巡回先として保存
			m_targetPoint = p;
			break;
		}
	}

	// 巡回地点として選択されたことを伝える
	m_targetPoint->SetIsSelected(true);
}

// デストラクタ
PatrolComponent::~PatrolComponent()
{
}

// 更新処理
void PatrolComponent::Update(float in_deltaTime)
{

	// ヘリがプレイヤーを見つけたら
	if (m_heli->GetFoundPlayer())
	{
		m_patrolState = PATROL_STATE::CHASE;
	}

	// 巡回状態時処理
	if (m_patrolState == PATROL_STATE::PATROL)
	{
		// ターゲット座標を更新
		m_targetPos = m_targetPoint->GetPosition();

		MoveToPatrolPos(in_deltaTime);
	}

	// プレイヤー追跡処理
	if (m_patrolState == PATROL_STATE::CHASE)
	{
		m_targetPos = m_world->GetPlayer()->GetPosition() + Vector3(0.0f, 300.0f * m_heli->GetNumber(), 600.0f + 100.0f * m_heli->GetNumber()) + m_world->GetPlayer()->GetForward() * Vector3(-300.0f, 0.0f, 0.0f);
		ChasePlayer(in_deltaTime);
	}
}

// 巡回地点への移動処理
void PatrolComponent::MoveToPatrolPos(float in_deltaTime)
{
	// 巡回地点変更時に用いるポインタ
	PatrolPoint* tempPoint = m_targetPoint;

	// ヘリコプターの現在地点座標
	Vector3 heliPos = m_owner->GetPosition();

	// (ターゲット座標 - ヘリ座標)から前進ベクトルを割り出す
	Vector3 forwardVec = m_targetPos - heliPos;
	forwardVec.Normalize();
	// ヘリを巡回地点へ向かせる
	m_owner->RotateToNewForward(forwardVec);

	// 移動量を計算
	Vector3 move = forwardVec * 800.0f * in_deltaTime;

	// ヘリを移動させる
	m_owner->SetPosition(heliPos + move);
	//m_owner->SetPosition(Vector3::Lerp(playerPos, m_targetPos, 0.03f * in_deltaTime));


	// 目標地点へ接近したら巡回地点を更新
	if (Vector3::Distance(heliPos, m_targetPos) < 100.0f)
	{
		// 巡回地点を走査して巡回先の座標としてセット
		for (auto p : m_world->GetPatrolPoint())
		{
			// 巡回地点として登録されていなかったら
			if (!p->GetIsSelected())
			{
				// 巡回先として保存
				tempPoint = p;
				break;
			}
		}
	}



	// 巡回地点が更新されたら
	if (m_targetPoint != tempPoint)
	{
		// 巡回地点をランダムソート
		m_world->ShufflePatrolPoint();

		tempPoint->SetIsSelected(true);
		// 巡回先だった地点の選択状態解除
		m_targetPoint->SetIsSelected(false);

		// 次の巡回地点として更新
		m_targetPoint = tempPoint;
	}
}

void PatrolComponent::ChasePlayer(float in_deltaTime)
{

	// ヘリコプターの現在地点座標
	Vector3 heliPos = m_owner->GetPosition();
	// (ターゲット座標 - ヘリ座標)から前進ベクトルを割り出す
	Vector3 forwardVec = m_targetPos - heliPos;
	forwardVec.Normalize();

	float distance = Vector3::Distance(heliPos, m_targetPos);
	// プレイヤーから一定距離離れたらポイント巡回に戻る
	if (distance > 5001.0f)
	{
		m_patrolState = PATROL_STATE::PATROL;
		return;
	}
	
	if (distance > 100.0f)
	{
		// 移動量を計算
		Vector3 move = forwardVec * 300.0f * in_deltaTime;
		// ヘリを移動させる
		m_owner->SetPosition(heliPos + move);

		// ヘリを巡回地点へ向かせる
		forwardVec.z = 0.0f;
		m_owner->RotateToNewForward(forwardVec);

	}
	else
	{
		
	}

}
