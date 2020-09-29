//-----------------------------------------------------------------------+
// アクタークラス
// 
// 
// copyright (C) 2019 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#include "Actor.h"
#include "GameMain.h"
#include "Component.h"
#include <algorithm>
#include <typeinfo>

int Actor::m_globalActorNo = 0;

// コンストラクタ
Actor::Actor()
	:m_state(STATE_ACTIVE)
	,m_position(Vector3::Zero)
	,m_rotation(Quaternion::Identity)
	,m_scale(1.0f)
	,m_speed(0.0f)
	,m_recomputeWorldTransform(true)
	,m_ID(m_globalActorNo)
{
	// ゲームメインにこのアクターを追加登録
	GAME_INSTANCE.AddActor(this);
	// ゲーム全体のアクター番号を更新
	m_globalActorNo++;
}

// デストラクタ
Actor::~Actor()
{
	// アクターが所持するコンポーネントの削除
	while (!m_components.empty())
	{
		delete m_components.back();
	}

	// ゲームメインにこのアクターの削除を依頼
	GAME_INSTANCE.RemoveActor(this);
}

// 更新処理 (Gameからコール)
// 引数 : in_deltaTime => 1フレーム分の経過時間
void Actor::Update(float in_deltaTime)
{
	// アクターがアクティブなら
	if (m_state == STATE_ACTIVE)
	{
		// ワールド変換行列を計算
		ComputeWorldTransform();

		// コンポーネントの更新処理
		UpdateComponents(in_deltaTime);
		// アクター固有の更新処理
		UpdateActor(in_deltaTime);

		// ワールド変換行列の再計算
		ComputeWorldTransform();
	}
}

// 更新処理 (コンポーネント)
// 引数 : in_deltaTime => 1フレーム分の経過時間
void Actor::UpdateComponents(float in_deltaTime)
{
	for (auto comp : m_components)
	{
		comp->Update(in_deltaTime);
	}
}

// 更新処理 (アクター固有処理:オーバーライド可能)
// 引数 : in_deltaTime => 1フレーム分の経過時間
void Actor::UpdateActor(float in_deltaTime)
{
}

// 入力処理
void Actor::ProcessInput(float in_deltaTime)
{
	if (m_state == STATE_ACTIVE)
	{
		// 入力処理を受け取るコンポーネントを優先して実行
		for (auto comp : m_components)
		{
			comp->ProcessInput(in_deltaTime);
		}
	}
}

// ワールド変換行列を計算
void Actor::ComputeWorldTransform()
{
	// ワールド変換の再計算が必要なら実行
	if (m_recomputeWorldTransform)
	{

		// スケーリング→回転→平行移動となるように変換行列を作成
		m_worldTransform = Matrix4::CreateScale(m_scale);
		m_worldTransform *= Matrix4::CreateFromQuaternion(m_rotation);
		m_worldTransform *= Matrix4::CreateTranslation(m_position);

		// アクターが持っている全コンポーネントの変換を行う
		for (auto comp : m_components)
		{
			comp->OnUpdateWorldTransform();
		}

		m_recomputeWorldTransform = false;
	}
}


// 前進ベクトルの更新
// in_forward => 向かせたい前方方向ベクトル
void Actor::RotateToNewForward(const Vector3 & in_forward)
{
	// X軸ベクトル(1, 0, 0)とforward間の角度を求める
	float dot = Vector3::Dot(Vector3::UnitX, in_forward);
	float angle = Math::Acos(dot);

	// 下向きだった場合
	if (dot > 0.9999f)
	{
		SetRotation(Quaternion::Identity);
	}
	// 上向きだった場合
	else if (dot < -0.9999f)
	{
		SetRotation(Quaternion(Vector3::UnitZ, Math::Pi));
	}
	else
	{
		// 軸ベクトルとforwardとの外積から回転軸を求めて回転させる
		Vector3 axis = Vector3::Cross(Vector3::UnitX, in_forward);
		axis.Normalize();
		SetRotation(Quaternion(axis, angle));
	}
}


// コンポーネントの追加
void Actor::AddComponent(Component * in_comp)
{
	// コンポーネントをソートして追加
	// 自分のオーダー番号よりも大きい挿入点を見つける
	int myOrder = in_comp->GetUpdateOrder();
	auto iter = m_components.begin();

	for (; iter != m_components.end(); ++iter)
	{
		if (myOrder < (*iter)->GetUpdateOrder())
		{
			break;
		}
	}

	// 要素を見つけたポイントの手前に挿入する
	m_components.insert(iter, in_comp);
}

// コンポーネントの削除
void Actor::RemoveComponent(Component * in_comp)
{
	// 引数のコンポーネントをコンポーネント配列から検索し、そのイテレータを削除
	auto iter = std::find(m_components.begin(), m_components.end(), in_comp);
	if (iter != m_components.end())
	{
		m_components.erase(iter);
	}
}
