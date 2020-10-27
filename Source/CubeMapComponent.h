#pragma once
#include "Component.h"
#include <string>
class CubeMapComponent : public Component
{

public:

	CubeMapComponent(class Actor* in_owner);
	~CubeMapComponent();

	void CreateTexture(const std::string& in_filePath);

	virtual void Draw(class Shader* in_shader);

	void SetIsVisible(bool in_visible) { m_isVisible = in_visible; }


private:


	class Texture* m_texture;

	bool m_isVisible;                // •`‰æ‚·‚é‚©‚µ‚È‚¢‚©‚Ìƒtƒ‰ƒO

};