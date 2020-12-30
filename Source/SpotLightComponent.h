#pragma once
#include "Component.h"
#include "SpotLight.h"


class SpotLightComponent : public Component
{

public:

	SpotLightComponent(class SpotLight* in_light);
	~SpotLightComponent();

	void SetLightParam(SpotLight::LIGHT_VOLUME in_vol);    // ライトパラメータのセット

	void Draw(class Shader* in_shader);


private:

	class SpotLight* m_light;      // ポイントライトアクタ

	class Mesh* m_cornMesh;        // ライトメッシュ (コーン)

	float m_radius;                 // ライトの影響半径

	// ライト減衰パラメータ
	float m_constant;
	float m_linear;
	float m_quadratic;


};