//-----------------------------------------------------------------------+
// シーンベースクラス.(抽象クラス)
// 
// 
// copyright (C) 2019 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#pragma once

// インクルードファイル


class SceneBase
{
public:

	SceneBase() {};                               // コンストラクタ
	virtual ~SceneBase() {};                      // デストラクタ

	virtual SceneBase* Update() = 0;         // シーンの更新処理(現在のシーンのポインタを返す)

	virtual void Initialize() = 0;

	virtual void Draw() = 0;                      // 描画処理

};