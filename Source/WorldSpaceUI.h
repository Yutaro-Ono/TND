//------------------------------------------------------------------+
// インターフェース基底クラス(ワールド空間用のUI)
//
//------------------------------------------------------------------+
#pragma once
#include "Math.h"
#include <string>

class WorldSpaceUI
{

public:

	WorldSpaceUI(const Vector3& in_pos, const std::string& in_filePath, float in_scale = 1.0f);
	WorldSpaceUI(const Vector3& in_pos, class Texture* in_texture, float in_scale = 1.0f);
	~WorldSpaceUI();


	void SetPosition(const Vector3& in_pos) { m_position = in_pos; }
	void SetTexture(class Texture* in_tex) { m_texture = in_tex; }
	int GetTextureID() { return m_textureID; }

	void Draw(class Shader* in_shader);

	// 座標のセット
	void SetUIPosition(const Vector3& in_pos);

	// ビルボード行列を定義・取得
	Matrix4 GetBillboardMatrix();

	// 表示非表示フラグのセッター
	void SetVisible(bool in_visible) { m_isVisible = in_visible; }

	void SetScale(float in_scale) { m_scale = in_scale; }

private:


	Vector3 m_position;                      // 座標

	float m_scale;                           // 拡大率

	class Texture* m_texture;                // テクスチャ
	int m_textureID;                         // テクスチャID

	bool m_isVisible;                        // 表示(false), 非表示(true)フラグ


	Matrix4 m_worldTransform;                // ワールド変換行列
	
	static Matrix4 m_staticBillboardMat;     // ビルボード行列 (カメラと平行になるように配置するための行列)


};