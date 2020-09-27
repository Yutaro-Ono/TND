//-----------------------------------------------------------------------+
// ステージ用ブロッククラス.
// 
// 
// copyright (C) 2020 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#pragma once
#include "Actor.h"

class LevelBlock : public Actor
{


public:

	LevelBlock();                            // コンストラクタ
	~LevelBlock();                           // デストラクタ

	void SetMesh(class Mesh* in_mesh);       // メッシュのセット


private:

	class MeshComponent* m_meshComp;
	class BoxCollider* m_box;
};