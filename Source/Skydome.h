//-----------------------------------------------------------------------+
// スカイドームクラス.
// 
// 
// copyright (C) 2019 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#pragma once

// インクルードファイル
#include "Actor.h"


class Skydome : public Actor
{


public:

	Skydome();                                             // コンストラクタ
	~Skydome();                                            // デストラクタ

	void SetMesh(class Mesh* in_mesh);                     // メッシュのセッター

	void UpdateActor(float in_deltaTime) override;         // 更新処理

	void SetSpin() { m_isSpin = true; }                    // 回転処理


private:

	float m_radian;

	class MeshComponent* m_meshComp;

	bool m_isSpin;


};