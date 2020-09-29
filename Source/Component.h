//-----------------------------------------------------------------------+
// コンポーネントクラス.
// 
// 
// copyright (C) 2019 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#pragma once
#include <cstdint>


class Component
{
public:

	Component(class Actor* in_owner, int in_updateOrder = 100);     // コンストラクタ(updateOrderが小さいほど早く実行される)

	virtual ~Component();                                           // 仮想デストラクタ

	virtual void Update(float in_deltaTime);                        // デルタタイムによるコンポーネントの更新(オーバーライド可)

	virtual void ProcessInput(float in_deltaTime) {}                                  // コンポーネントの入力処理(オーバーライド可)

	virtual void OnUpdateWorldTransform() {}                        // ワールド変換が必要なときに呼ばれる関数

	int GetID() { return m_ID; }                                    // IDの取得

	class Actor* GetOwner() { return m_owner; }                     // オーナーアクターの取得

	int GetUpdateOrder() const { return m_updateOrder; }            // updateOrderの取得



protected:

	class Actor* m_owner;           // オーナーアクターへのポインタ

	int m_updateOrder;              // コンポーネントの更新順序

	int m_ID;                       // コンポーネント管理ID

	static int m_globalID;          // コンポーネント管理IDの連番用

};