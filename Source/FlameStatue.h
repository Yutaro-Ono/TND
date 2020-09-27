//-----------------------------------------------------------------------+
// ともし火の燭台クラス(リザルト画面で使用).
// 
// 
// copyright (C) 2019 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#pragma once
#include "Actor.h"


class FlameStatue : public Actor
{


public:

	FlameStatue();                                           // コンストラクタ
	~FlameStatue();                                          // デストラクタ

	void SetMesh(class Mesh* in_mesh);                       // メッシュのセット

	void UpdateActor(float in_deltaTime) override;           // 更新処理


private:

	class MeshComponent* m_meshComp;


};