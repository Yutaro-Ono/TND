//-----------------------------------------------------------------------+
// メッシュコンポーネントクラス.
// 
// 
// copyright (C) 2019 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#pragma once
#include "Component.h"
#include <cstddef>
#include "Math.h"

class MeshComponent : public Component
{
public:

	MeshComponent(class Actor* in_owner, bool in_isSkeletal = false);       // コンストラクタ(親アクターが必要)
	~MeshComponent();                                                       // デストラクタ

	virtual void Draw(class Shader* in_shader);                             // メッシュコンポーネントの描画
	virtual void DrawShadow(class Shader* in_shader);                       // シャドウ生成用の描画
	void DrawMap(class Shader* in_shader);                                  // マップHUD描画用

	virtual void SetMesh(class Mesh* in_mesh) { m_mesh = in_mesh; }         // メッシュコンポーネントで使用するメッシュのセット

	void SetTexturesToUniform(class Shader* in_shader);                     // シェーダuniformにテクスチャをセットする関数
	void SetTextureIndex(size_t in_index) { m_textureIndex = in_index; }    // テクスチャのセット

	void SetVisible(bool in_visible) { m_visible = in_visible; }            // 表示フラグのセッター
	bool GetVisible() const { return m_visible; }                           // 表示フラグのゲッター

	bool GetIsSkeletal() const { return m_isSkeletal; }                     // スケルタルかどうかのゲッター

	void SetIntensityVal(float in_val) { m_intensity = in_val; }            // 輝度強度のセッター

	void SetDrawMap(bool in_flg) { m_isDrawMap = in_flg; }
	void SetMapColor(const Vector3& in_color) { m_mapColor = in_color; }    // マップ描画用のカラーセッター

protected:

	class Mesh* m_mesh;                                                     // メッシュオブジェクトの格納用
	size_t m_textureIndex;                                                  // テクスチャーのインデックス

	bool m_visible;                                                         // 表示フラグ
	bool m_isDrawMap;                                                       // マップ画面に表示するか
	bool m_isSkeletal;                                                      // スケルタルモデルかどうか

	float m_intensity;                                                      // 輝度の強度情報 (エミッシブマップを適用しているメッシュのみ)

	Vector3 m_mapColor;                                                     // マップ描画時のカラー情報
};