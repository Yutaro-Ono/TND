//-----------------------------------------------------------------------+
// メッシュコンポーネントクラス.
// 
// 
// copyright (C) 2019 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#pragma once
#include "Component.h"
#include <cstddef>

class MeshComponent : public Component
{
public:

	MeshComponent(class Actor* in_owner, bool in_isSkeletal = false);       // コンストラクタ(親アクターが必要)
	~MeshComponent();                                                       // デストラクタ

	virtual void Draw(class Shader* in_shader);                             // メッシュコンポーネントの描画
	virtual void DrawShadow(class Shader* in_shader);                       // シャドウ生成用の描画
	virtual void SetMesh(class Mesh* in_mesh) { m_mesh = in_mesh; }         // メッシュコンポーネントで使用するメッシュのセット

	void SetTextureIndex(size_t in_index) { m_textureIndex = in_index; }    // テクスチャのセット

	void SetVisible(bool in_visible) { m_visible = in_visible; }            // 表示フラグのセッター
	bool GetVisible() const { return m_visible; }                           // 表示フラグのゲッター

	bool GetIsSkeletal() const { return m_isSkeletal; }                     // スケルタルかどうかのゲッター


protected:

	class Mesh* m_mesh;                                                     // メッシュオブジェクトの格納用
	size_t m_textureIndex;                                                  // テクスチャーのインデックス

	bool m_visible;                                                         // 表示フラグ
	bool m_isSkeletal;                                                      // スケルタルモデルかどうか

};