#pragma once
#include "Component.h"
#include "SpotLight.h"


class SpotLightComponent : public Component
{

public:

	SpotLightComponent(class SpotLight* in_light);
	~SpotLightComponent();

	void SetLightParam(SpotLight::LIGHT_VOLUME in_vol);    // ���C�g�p�����[�^�̃Z�b�g

	void Draw(class Shader* in_shader);


private:

	class SpotLight* m_light;      // �|�C���g���C�g�A�N�^

	class Mesh* m_cornMesh;        // ���C�g���b�V�� (�R�[��)

	float m_radius;                 // ���C�g�̉e�����a

	// ���C�g�����p�����[�^
	float m_constant;
	float m_linear;
	float m_quadratic;


};