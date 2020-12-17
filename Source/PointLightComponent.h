#pragma once
#include "Component.h"
#include "PointLight.h"


class PointLightComponent : public Component
{

public:

	PointLightComponent(class PointLight* in_light);
	~PointLightComponent();

	void SetLightParam(PointLight::LIGHT_VOLUME in_vol);    // ���C�g�p�����[�^�̃Z�b�g

	void Draw(class Shader* in_shader);


private:

	class PointLight* m_light;      // �|�C���g���C�g�A�N�^

	class Mesh* m_mesh;             // ���C�g���b�V�� (����)

	float m_radius;                 // ���C�g�̉e�����a

	// ���C�g�����p�����[�^
	float m_constant;
	float m_linear;
	float m_quadratic;


};