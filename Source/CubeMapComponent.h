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

private:


	class Texture* m_texture;


};