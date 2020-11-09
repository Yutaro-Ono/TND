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

	void SetIsVisible(bool in_visible) { m_isVisible = in_visible; }


private:

	class Texture* m_texture;

	bool m_isVisible;                // •`‰æ‚·‚é‚©‚µ‚È‚¢‚©‚Ìƒtƒ‰ƒO

};