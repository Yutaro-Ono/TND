//------------------------------------------------------------------+
// インターフェース基底クラス(ワールド空間用のUI)
//
//------------------------------------------------------------------+
#pragma once
#include "Math.h"
#include <string>

class UIWorld
{

public:

	UIWorld(const Vector3& in_pos, const std::string& in_filePath, float in_scale = 1.0f);
	~UIWorld();


	void SetPosition(const Vector3& in_pos) { m_position = in_pos; }
	int GetTextureID() { return m_textureID; }

	void Draw(class Shader* in_shader);

private:


	Vector3 m_position;            // 座標

	float m_scale;                 // 拡大率

	class Texture* m_texture;      // テクスチャ
	int m_textureID;               // テクスチャID

	bool m_isVisible;              // 表示(false), 非表示(true)フラグ

	static Matrix4 m_staticBillboardMat;
	static Vector3 m_cameraWorldPos;


};