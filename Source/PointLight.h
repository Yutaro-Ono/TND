//-------------------------------------------------+
// ポイントライトアクター
// 2020 YutaroOno.
//-------------------------------------------------+
#pragma once
#include "Actor.h"

class PointLight : public Actor
{
public:

	// ライトボリューム (ライト影響範囲)
	enum LIGHT_VOLUME
	{
		VL_SMALL = 0,
		VL_MEDIUM,
		VL_BIG
	};

	PointLight(LIGHT_VOLUME in_vol = VL_MEDIUM);
	~PointLight();

	void SetLightColor(const Vector3& in_diffuse, const Vector3& in_specular);          // ライトカラーのセット

	void UpdateActor(float in_deltaTime) override;

	void SwitchLighting();                                                              // 点灯状態ON・OFF


	//-------------------------------------------+
	// Setter / Getter
	//-------------------------------------------+
	// 各種カラーの取得
	const Vector3& GetDiffuseColor() { return m_diffuse; }
	const Vector3& GetAmbientColor() { return m_ambient; }
	const Vector3& GetSpecularColor() { return m_specular; }
	
	LIGHT_VOLUME GetLightVolume() { return m_lightVolume; }     // ライト影響範囲のゲッター

	void SetLuminance(float in_luminance) { m_luminance = in_luminance; }
	float GetLuminance() { return m_luminance; }

	void SetFlash(bool in_flash) { m_flash = in_flash; }        // 点滅させるかさせないかの切り替え

private:


	LIGHT_VOLUME m_lightVolume;                                 // ライト影響範囲 (ポイントライトコンポーネントにて使用)

	bool m_lighting;              // 点灯状態

	// カラー情報
	Vector3 m_diffuse;
	Vector3 m_ambient;
	Vector3 m_specular;

	// 輝度情報
	float m_luminance;

	bool m_calcRadius;       // 半径を再計算するか

	bool m_flash;            // 点滅させるか
	float m_flashOffset;     // フラッシュの間隔

	// ポイントライトコンポーネント
	class PointLightComponent* m_lightComp;
};