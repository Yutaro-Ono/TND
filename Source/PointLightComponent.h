#pragma once
#include "Component.h"
#include "PointLight.h"


class PointLightComponent : public Component
{

public:

	PointLightComponent(class PointLight* in_light);
	~PointLightComponent();

	void SetLightParam(PointLight::LIGHT_VOLUME in_vol);    // ライトパラメータのセット

	void Draw(class Shader* in_shader);


private:

	class PointLight* m_light;      // ポイントライトアクタ

	class Mesh* m_mesh;             // ライトメッシュ (球体)

	float m_radius;                 // ライトの影響半径

	// ライト減衰パラメータ
	float m_constant;
	float m_linear;
	float m_quadratic;


};