#pragma once
#include "Component.h"

class CarMeshComponent : public Component
{

public:

	CarMeshComponent(class Actor* in_owner);
	~CarMeshComponent();


	virtual void Draw(class Shader* in_shader);                             // メッシュコンポーネントの描画
	virtual void DrawShadow(class Shader* in_shader);                       // シャドウ生成用の描画

	virtual void SetMesh(class Mesh* in_mesh) { m_mesh = in_mesh; }         // メッシュコンポーネントで使用するメッシュのセット

	void SetTextureIndex(size_t in_index) { m_textureIndex = in_index; }    // テクスチャのセット

	void SetReflect(bool in_ref) { m_reflect = in_ref; }                    // 反射計算の有無

	void SetVisible(bool in_visible) { m_visible = in_visible; }            // 表示フラグのセッター
	bool GetVisible() const { return m_visible; }                           // 表示フラグのゲッター



protected:

	class Mesh* m_mesh;                                                     // メッシュオブジェクトの格納用
	size_t m_textureIndex;                                                  // テクスチャーのインデックス

	bool m_reflect;                                                         // 反射計算を行うか

	bool m_visible;                                                         // 表示フラグ
};