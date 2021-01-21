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

	class Texture* GetSkyBoxTexture() { return m_texture; }

	
	void SetIsVisible(bool in_visible) { m_isVisible = in_visible; }         // キューブを表示するかしないかのセット
	void SetLuminance(float in_luminance) { m_luminance = in_luminance; }    // 輝度情報のセット


private:

	class Texture* m_texture;

	float m_luminance;               // 輝度情報

	bool m_isVisible;                // 描画するかしないかのフラグ

};