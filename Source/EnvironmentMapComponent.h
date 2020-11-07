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



private:

	bool m_isVisible;

	class Mesh* m_mesh;

};