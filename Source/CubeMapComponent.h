#pragma once
#include "MeshComponent.h"
#include <string>
#include <vector>
class CubeMapComponent : public Component
{

public:

	CubeMapComponent(class Actor* in_owner);
	~CubeMapComponent();

	void CreateTexture(const std::string& in_filePath);

	void Draw(class Shader* in_shader);

	// 環境マップオブジェクトの描画処理
	void DrawEnvironmentMap(std::vector<class MeshComponent*> in_envMeshes);

	void SetIsVisible(bool in_visible) { m_isVisible = in_visible; }


private:


	class Texture* m_texture;

	bool m_isVisible;                // 描画するかしないかのフラグ

};