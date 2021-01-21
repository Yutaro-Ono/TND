#pragma once
#include "Component.h"
#include "Math.h"

class LightGlassComponent : public Component
{
public:

	LightGlassComponent(class Actor* in_owner);
	~LightGlassComponent();

	// メッシュのセット
	void SetMesh(class Mesh* in_mesh) { m_mesh = in_mesh; }

	// 描画処理
	void Draw(class Shader* in_shader);


	//----------------------------------------------+
	// Setter / Getter
	//----------------------------------------------+
	void SetLightColor(const Vector3& in_color) { m_lightColor = in_color; }         // ライトカラーのセット
	void SetLightLuminance(float in_luminance) { m_luminance = in_luminance; }       // ライト輝度のセット
	void SetVisible(bool in_visible) { m_isVisible = in_visible; }

private:

	class Mesh* m_mesh;

	Vector3 m_lightColor;           // ライトカラー
	float m_luminance;              // ライトの輝度

	bool m_isVisible;               // 描画するかしないか


};