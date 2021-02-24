//
// 環境マップオブジェクトコンポーネント
//
#pragma once
#include "Component.h"
#include <vector>

class EnvironmentMapComponent : public Component
{

public:

	EnvironmentMapComponent(class Actor* in_owner);     // コンストラクタ
	~EnvironmentMapComponent();                         // デストラクタ

	void SetMesh(class Mesh* in_mesh) { m_mesh = in_mesh; }

	// 環境マップオブジェクトの描画処理
	void DrawEnvironmentMap(class Shader* in_envShader);

	void SetLuminance(float in_lum) { m_luminance = in_lum; }
	void SetAlpha(float in_alpha) { m_alpha = in_alpha; }

private:

	bool m_isVisible;

	float m_luminance;       // 輝度
	float m_alpha;           // 透過度

	class Mesh* m_mesh;

};