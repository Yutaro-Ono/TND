#pragma once
#include "Component.h"


class PointLightComponent : public Component
{

public:

	PointLightComponent(class PointLight* in_light);
	~PointLightComponent();

	void Draw(class Shader* in_shader);


private:

	class PointLight* m_light;      // �|�C���g���C�g�A�N�^

	class Mesh* m_mesh;

};