#pragma once
#include "Actor.h"

class PointLight : public Actor
{
public:

	PointLight();
	~PointLight();

	void SetLightParameter(float in_constant, float in_linear, float in_quadratic);     // ライトパラメータのセット
	void SetLightColor(const Vector3& in_diffuse, const Vector3& in_specular);                                     // ライトカラーのセット

	void SwitchLighting();                                                              // 点灯状態ON・OFF


	//-------------------------------------------+
	// Setter / Getter
	//-------------------------------------------+
	// ライトボリューム用半径の取得
	float GetLightRadius() { return m_lightRadius; }
	// 各種減衰パラメータの取得
	float GetConstant() { return m_constant; }
	float GetLinear() { return m_linear; }
	float GetQuadratic() { return m_quadratic; }
	// 各種カラーの取得
	const Vector3& GetDiffuseColor() { return m_diffuse; }
	const Vector3& GetAmbientColor() { return m_ambient; }
	const Vector3& GetSpecularColor() { return m_specular; }

	void SetLightRadius(float in_radius) { m_lightRadius = in_radius; }

private:

	void CalcLightRadius();       // ライトの減衰半径計算

	bool m_lighting;        // 点灯状態

	float m_lightRadius;    // ライト半径 (スケール値)

	// 減衰パラメータ
	float m_constant;
	float m_linear;
	float m_quadratic;

	// カラー情報
	Vector3 m_diffuse;
	Vector3 m_ambient;
	Vector3 m_specular;

	// ポイントライトコンポーネント
	class PointLightComponent* m_lightComp;
};